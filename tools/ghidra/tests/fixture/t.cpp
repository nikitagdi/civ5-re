// Fixture for the ghidra-sync integration test; build: build_fixture.py.
#define NOINLINE __attribute__((noinline))

struct Vec2 {
    int x;
    int y;
};

class Base {
public:
    Base();
    virtual int Update(int dt);
    virtual int Extra(int a, int b);
    int m_iCount;
};

class Derived : public Base {
public:
    Derived();
    int Update(int dt) override;
    virtual int Scale(int k);
    Vec2 GetPos() const;
    int Sum(int a, int b);
    int Peek();
    int m_iExtra;
    Vec2 m_pos;
};

NOINLINE Base::Base() : m_iCount(0) {}
NOINLINE int Base::Update(int dt) { return m_iCount + dt; }
NOINLINE int Base::Extra(int a, int b) { return a * b + m_iCount; }

NOINLINE Derived::Derived() : m_iExtra(1) {
    m_pos.x = 0;
    m_pos.y = 0;
}
NOINLINE int Derived::Update(int dt) { return dt * 2 + m_iExtra; }
NOINLINE int Derived::Scale(int k) { return k * m_iExtra - m_iCount; }
NOINLINE Vec2 Derived::GetPos() const { return m_pos; }
NOINLINE int Derived::Sum(int a, int b) { return a + b + m_iExtra; }
NOINLINE int Derived::Peek() { return m_iExtra ^ m_pos.y; }

static NOINLINE int hidden_func(Derived* d, int k) { return d->m_iExtra * 3 + d->m_iCount - k; }

NOINLINE int CallUpdate(Base* p, int dt) { return p->Update(dt) + 1; }
NOINLINE int CallExtra(Base* p) { return p->Extra(3, 4) + 2; }
NOINLINE int UsePos(Derived* d, int k) {
    Vec2 v = d->GetPos();
    return v.x * v.y + hidden_func(d, k);
}

extern "C" int g_counter;
int g_counter = 5;

extern "C" void _start() {
    Derived d;
    g_counter += CallUpdate(&d, g_counter) + CallExtra(&d) + UsePos(&d, g_counter) + d.Sum(1, 2) + d.Peek();
}
