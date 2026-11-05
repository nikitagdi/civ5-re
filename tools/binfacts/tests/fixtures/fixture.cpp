extern "C" {
void __cxa_pure_virtual();
int ext_init();
void ext_hook(int);

int lua_version = 51;
int fixture_c_data[4] = {1, 2, 3, 4};

int fixture_c_func(int x) { return ext_init() + x * 2; }
void fixture_c_hook() { ext_hook(1); }
void _start() {}
}

struct ExtBase {
    int x;
    ExtBase();
    virtual ~ExtBase();
    virtual void ext_virt();
};

int g_counter = 3;
int g_dynamic = ext_init();

namespace std {
namespace __1 {
struct fake_ios {
    virtual ~fake_ios();
    virtual void sync();
    int state;
};
fake_ios::~fake_ios() {}
void fake_ios::sync() { state = 0; }
}  // namespace __1
}  // namespace std

namespace ns {

int g_ns = 5;

struct Stat {
    static int count;
    static int bump();
};
int Stat::count = 7;
int Stat::bump() { return ++count; }

struct Shape {
    Shape();
    virtual ~Shape();
    virtual int area() const = 0;
    virtual void touch();
    int id;
};
Shape::Shape() : id(0) {}
Shape::~Shape() {}
void Shape::touch() { id++; }

struct Square : Shape {
    Square(int s);
    ~Square() override;
    int area() const override;
    int side;
};
Square::Square(int s) : side(s) {}
Square::~Square() {}
int Square::area() const { return side * side; }

struct Vec {
    int x, y;
    bool operator<(const Vec& o) const;
    int operator()(int k) const;
    static Vec make(int a);
};
bool Vec::operator<(const Vec& o) const { return x < o.x; }
int Vec::operator()(int k) const { return x * k; }
Vec Vec::make(int a) { return Vec{a, a}; }

struct M1 {
    virtual ~M1();
    virtual int a();
    int m1;
};
struct M2 {
    virtual ~M2();
    virtual int b();
    int m2;
};
M1::~M1() {}
int M1::a() { return 1; }
M2::~M2() {}
int M2::b() { return 2; }

struct Mix : M1, M2 {
    Mix();
    ~Mix() override;
    int a() override;
    int b() override;
};
Mix::Mix() {}
Mix::~Mix() {}
int Mix::a() { return 10; }
int Mix::b() { return 20; }

struct VB {
    VB();
    virtual ~VB();
    virtual int vbf();
    int vb;
};
VB::VB() : vb(1) {}
VB::~VB() {}
int VB::vbf() { return vb; }

struct DV : virtual VB {
    DV();
    ~DV() override;
    int vbf() override;
    virtual int dvf();
    int dv;
};
DV::DV() : dv(2) {}
DV::~DV() {}
int DV::vbf() { return vb + dv; }
int DV::dvf() { return dv; }

struct DD : DV {
    DD();
    ~DD() override;
    int dvf() override;
};
DD::DD() {}
DD::~DD() {}
int DD::dvf() { return 9; }

struct UsesExt : ExtBase {
    UsesExt();
    virtual int own();
};
UsesExt::UsesExt() {}
int UsesExt::own() { return x; }

struct AslHelper {
    AslHelper();
    int run() const;
    int v;
};
AslHelper::AslHelper() : v(4) {}
int AslHelper::run() const { return v; }

template <class T>
struct Box {
    T v;
    Box(T x) : v(x) {}
    virtual ~Box() {}
    virtual T get() const { return v; }
};
template struct Box<int>;

template <class T>
T tmax(T a, T b) { return a < b ? b : a; }
template int tmax<int>(int, int);

inline int& counter() {
    static int c = ext_init();
    return c;
}
int use_counter() { return counter()++; }

}  // namespace ns

int fixture_use(ns::Square* s, ns::Mix* m, ns::DD* d, ns::UsesExt* u) {
    return s->area() + m->a() + m->b() + d->dvf() + u->own() + ns::use_counter() + ns::tmax(1, 2);
}
