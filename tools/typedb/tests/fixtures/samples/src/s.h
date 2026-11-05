#define M2 void a(); void b();
struct S { int x; M2
  int f() { return x + 1; }
  int y;
};
struct P { S s; P(const P&); };
static int sfun(int v) { return v; }
const int kConst = 3;
namespace { int anon(int); }
extern "C" { static int cstat(int); }
struct Outer { static int sm; };
template <class T> struct V { void push(const T& t); T* p; };
V<int>* pv;
