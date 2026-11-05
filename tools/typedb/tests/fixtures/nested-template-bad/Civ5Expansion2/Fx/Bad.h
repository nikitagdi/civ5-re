/// origin guess
#pragma once

/// size ?
template <class T>
class CvEvent {
public:
    /// size 0x4 CvEvent<int>
    class Handle {
    public:
        /// 0x0a000100 CvEvent<int>
        void Open();
        int m_n;  ///< +0x0
    };
    /// 0x0a000200 CvEvent<int>::Handle
    void Fire();
};

/// inline
inline void UseBad() {
    CvEvent<int>::Handle h;
    (void)h;
}
