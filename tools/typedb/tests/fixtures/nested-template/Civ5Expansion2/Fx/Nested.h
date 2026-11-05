/// origin guess
#pragma once

// A class nested in a class template: SPEC names the instantiated class that holds the entity.
/// size ?
template <class T>
class CvEvent {
public:
    /// 0x0a000010 CvEvent<int>
    /// 0x0a000020 CvEvent<char>
    void Fire();

    /// size 0x8 CvEvent<int>::Handle
    /// size 0x8 CvEvent<char>::Handle
    class Handle {
    public:
        /// 0x0a000100 CvEvent<int>::Handle
        /// 0x0a000110 CvEvent<char>::Handle
        void Open();
        /// inline
        T Get() const { return m_value; }

        /// size 0x1 CvEvent<int>::Handle::Tag
        struct Tag {};

        T m_value;  ///< +0x0
        int m_tag;  ///< +0x4
    };

    /// size ?
    /// vtable ?
    struct Sink {
        /// 0x0a000200 CvEvent<int>::Sink
        /// deleting 0x0a000210 CvEvent<int>::Sink
        virtual ~Sink();
        /// pure
        virtual void Push(T v) = 0;
    };
};

/// inline
inline int UseNested() {
    CvEvent<int>::Handle a;
    CvEvent<char>::Handle b;
    CvEvent<int>::Handle::Tag c;
    (void)c;
    CvEvent<int>::Sink* k = 0;
    (void)k;
    return a.m_tag + b.m_tag;
}
