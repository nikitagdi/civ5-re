import struct
import unittest

from binfacts import classsize
from binfacts.classsize import decode, scan

VA = 0x1000


def rel(frm_next, target):
    return struct.pack("<i", target - frm_next)


class Decoder(unittest.TestCase):
    def test_store_immediate_to_memory(self):
        ins = decode(b"\xc7\x40\x44\x00\x00\x00\x00", 0, VA)
        self.assertEqual((ins.n, ins.kind, ins.w, ins.mem), (7, "mov", 4, (0, None, 1, 0x44)))

    def test_sib_and_widths(self):
        ins = decode(b"\x8b\x44\x24\x04", 0, VA)                   # mov eax, [esp+4]
        self.assertEqual((ins.n, ins.mem), (4, (4, None, 1, 4)))
        ins = decode(b"\x66\xc7\x46\x20\x01\x00", 0, VA)           # mov word [esi+0x20], 1
        self.assertEqual((ins.n, ins.w, ins.mem), (6, 2, (6, None, 1, 0x20)))
        ins = decode(b"\x0f\xb6\x4e\x10", 0, VA)                   # movzx ecx, byte [esi+0x10]
        self.assertEqual((ins.n, ins.w, ins.mem), (4, 1, (6, None, 1, 0x10)))
        ins = decode(b"\xf2\x0f\x11\x40\x08", 0, VA)               # movsd [eax+8], xmm0
        self.assertEqual((ins.n, ins.w), (5, 8))
        ins = decode(b"\xdd\x40\x20", 0, VA)                       # fld qword [eax+0x20]
        self.assertEqual((ins.n, ins.w), (3, 8))
        ins = decode(b"\x8d\x84\x88\x10\x00\x00\x00", 0, VA)       # lea eax, [eax+ecx*4+0x10]
        self.assertEqual((ins.n, ins.mem), (7, (0, 1, 4, 0x10)))

    def test_branch_targets_are_absolute(self):
        self.assertEqual(decode(b"\x74\x05", 0, VA).tgt, VA + 7)
        self.assertEqual(decode(b"\x0f\x84\x10\x00\x00\x00", 0, VA).tgt, VA + 0x16)
        self.assertEqual(decode(b"\xe8" + rel(VA + 5, 0x2000), 0, VA).tgt, 0x2000)

    def test_unknown_and_truncated_forms(self):
        self.assertIsNone(decode(b"\xd6", 0, VA))
        self.assertIsNone(decode(b"\xc7\x40\x44\x00", 0, VA))
        self.assertIsNone(decode(b"\x67\x8b\x00", 0, VA))


class ThisTracking(unittest.TestCase):
    def test_store_through_this(self):
        r = scan(b"\x8b\x44\x24\x04\xc7\x40\x44\x00\x00\x00\x00\xc3", VA)
        self.assertEqual((r.extent, r.complete), (0x48, True))

    def test_callee_saved_register_survives_calls_and_widths_count(self):
        code = (b"\x56" b"\x8b\x74\x24\x08" b"\xe8" + rel(VA + 10, 0x5000) + b"\xc6\x46\x10\x01"
                b"\x66\xc7\x46\x20\x01\x00" b"\x5e\xc3")
        r = scan(code, VA)
        self.assertEqual((r.extent, r.complete), (0x22, True))

    def test_caller_saved_register_is_lost_at_a_call(self):
        code = b"\x8b\x44\x24\x04" b"\xe8" + rel(VA + 9, 0x5000) + b"\xc7\x40\x08\x00\x00\x00\x00\xc3"
        self.assertEqual(scan(code, VA).extent, 0)

    def test_lea_and_add_keep_the_offset(self):
        code = b"\x8b\x44\x24\x04" b"\x8d\x48\x08" b"\x89\x41\x04" b"\xc3"      # ecx = this+8; [ecx+4] = eax
        self.assertEqual(scan(code, VA).extent, 0x10)
        code = b"\x8b\x74\x24\x04" b"\x83\xc6\x20" b"\x8b\x06" b"\xc3"          # esi += 0x20; mov eax, [esi]
        self.assertEqual(scan(code, VA).extent, 0x24)

    def test_indexed_access_is_not_counted(self):
        code = b"\x8b\x44\x24\x04" b"\x8b\x4c\x88\x40" b"\xc3"                  # mov ecx, [eax+ecx*4+0x40]
        self.assertEqual(scan(code, VA).extent, 0)

    def test_a_register_overwritten_on_one_path_is_not_this_after_the_join(self):
        code = (b"\x8b\x74\x24\x04" b"\x85\xc0" b"\x74\x05" b"\xbe\x00\x00\x00\x00" b"\x89\x46\x04" b"\xc3")
        self.assertEqual(scan(code, VA).extent, 0)

    def test_a_register_kept_on_both_paths_is_this_after_the_join(self):
        code = (b"\x8b\x74\x24\x04" b"\x85\xc0" b"\x74\x05" b"\xb8\x00\x00\x00\x00" b"\x89\x46\x04" b"\xc3")
        self.assertEqual(scan(code, VA).extent, 8)

    def test_a_loop_that_moves_the_register_discards_the_function(self):
        code = b"\x8b\x74\x24\x04" b"\x89\x06" b"\x83\xc6\x04" b"\x75\xf9" b"\xc3"
        r = scan(code, VA)
        self.assertEqual((r.extent, r.complete), (0, False))

    def test_a_loop_that_keeps_the_register_is_fine(self):
        code = b"\x8b\x74\x24\x04" b"\x89\x06" b"\x40" b"\x75\xfb" b"\xc3"         # inc eax; jne back
        self.assertEqual(scan(code, VA).extent, 4)

    def test_program_counter_idiom_keeps_the_stack_straight(self):
        code = b"\xe8\x00\x00\x00\x00" b"\x5b" b"\x8b\x44\x24\x04" b"\x89\x58\x10" b"\xc3"
        self.assertEqual(scan(code, VA).extent, 0x14)

    def test_pushes_shift_the_argument_slot(self):
        code = b"\x55\x53\x57\x56" b"\x83\xec\x0c" b"\x8b\x74\x24\x20" b"\x89\x46\x08" b"\xc3"   # this at [esp+0x20]
        self.assertEqual(scan(code, VA).extent, 0x0c)
        code = b"\x55\x53\x57\x56" b"\x83\xec\x0c" b"\x8b\x74\x24\x24" b"\x89\x46\x08" b"\xc3"
        self.assertEqual(scan(code, VA).extent, 0)

    def test_frame_pointer_form(self):
        code = b"\x55\x89\xe5\x56" b"\x8b\x75\x08" b"\xc7\x46\x0c\x00\x00\x00\x00" b"\x5e\x5d\xc3"
        self.assertEqual(scan(code, VA).extent, 0x10)

    def test_floating_point_widths(self):
        code = b"\x8b\x44\x24\x04" b"\xf2\x0f\x11\x40\x08" b"\xdd\x40\x20" b"\xc3"
        self.assertEqual(scan(code, VA).extent, 0x28)

    def test_unknown_form_keeps_what_was_proved(self):
        r = scan(b"\x8b\x44\x24\x04\x89\x48\x04\xd6\x89\x48\x40", VA)
        self.assertEqual((r.extent, r.complete), (8, False))

    def test_this_is_not_found_in_another_argument(self):
        self.assertEqual(scan(b"\x8b\x44\x24\x08\x89\x48\x40\xc3", VA).extent, 0)


class NewThenConstructor(unittest.TestCase):
    NEW, CTOR = 0x2000, 0x3000

    def test_push_form(self):
        code = (b"\x6a\x4c" b"\xe8" + rel(VA + 7, self.NEW) + b"\x83\xc4\x04" b"\x50"
                b"\xe8" + rel(VA + 16, self.CTOR) + b"\x83\xc4\x04\xc3")
        r = scan(code, VA, {self.NEW}, {self.CTOR: "K"})
        self.assertEqual(r.news, [(VA + 11, "K", 0x4c)])

    def test_store_form_with_the_pointer_kept_in_a_callee_saved_register(self):
        code = (b"\x83\xec\x0c" b"\xc7\x04\x24\x4c\x00\x00\x00" b"\xe8" + rel(VA + 15, self.NEW) +
                b"\x89\xc6" b"\x89\x04\x24" b"\xe8" + rel(VA + 25, self.CTOR) + b"\x83\xc4\x0c\xc3")
        r = scan(code, VA, {self.NEW}, {self.CTOR: "K"})
        self.assertEqual(r.news, [(VA + 20, "K", 0x4c)])

    def test_a_pointer_that_is_not_from_new_gives_nothing(self):
        code = b"\x8b\x44\x24\x04\x50" b"\xe8" + rel(VA + 10, self.CTOR) + b"\xc3"
        self.assertEqual(scan(code, VA, {self.NEW}, {self.CTOR: "K"}).news, [])

    def test_another_call_between_new_and_the_constructor_breaks_the_chain(self):
        code = (b"\x6a\x4c" b"\xe8" + rel(VA + 7, self.NEW) + b"\x83\xc4\x04" b"\x50"
                b"\xe8" + rel(VA + 16, 0x4000) + b"\xe8" + rel(VA + 21, self.CTOR) + b"\xc3")
        self.assertEqual(scan(code, VA, {self.NEW}, {self.CTOR: "K"}).news, [])


if __name__ == "__main__":
    unittest.main()
