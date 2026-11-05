extern "C" {
void __cxa_pure_virtual() {}
int __cxa_guard_acquire(long long* g) { return *(char*)g ? 0 : 1; }
void __cxa_guard_release(long long* g) { *(char*)g = 1; }
int ext_init() { return 11; }
void ext_hook(int) {}
}

void operator delete(void*) noexcept {}
void operator delete(void*, unsigned) noexcept {}

namespace std {
struct type_info {
    virtual ~type_info();
    const char* name_;
};
type_info::~type_info() {}
}  // namespace std

namespace __cxxabiv1 {
struct __class_type_info : std::type_info {
    virtual ~__class_type_info();
};
__class_type_info::~__class_type_info() {}
struct __si_class_type_info : __class_type_info {
    virtual ~__si_class_type_info();
    const __class_type_info* base_;
};
__si_class_type_info::~__si_class_type_info() {}
struct __vmi_class_type_info : __class_type_info {
    virtual ~__vmi_class_type_info();
    unsigned flags_;
    unsigned count_;
};
__vmi_class_type_info::~__vmi_class_type_info() {}
}  // namespace __cxxabiv1

struct ExtBase {
    int x;
    ExtBase();
    virtual ~ExtBase();
    virtual void ext_virt();
};
ExtBase::ExtBase() : x(1) {}
ExtBase::~ExtBase() {}
void ExtBase::ext_virt() {}
