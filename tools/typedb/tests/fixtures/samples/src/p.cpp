struct Tex;
namespace Gfx { struct Tex {}; void Load(int); void Load(float, Tex*); }
template <class T> using __typedb_id = T;
struct B { virtual ~B(); virtual void g(); int m; };
struct E { virtual void OnEvent(int) = 0; };
struct D : B, E { void OnEvent(int); };
struct __typedb_P1 : D { virtual void __typedb_k(); }; void __typedb_P1::__typedb_k() {}
namespace Gfx { __typedb_id<void (float, Tex*)>* __typedb_f1 = &Load; }
unsigned __typedb_s1[3] = {sizeof(D), alignof(D), __datasizeof(D)};
void odd() asm("odd$name");
void (*pf)() = &odd;
