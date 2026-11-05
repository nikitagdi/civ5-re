/// origin guess
#pragma once
#include <vector>

// A class template specialization that only a signature takes or returns by value is still a complete record.
/// size 0x4
class FHolder {
public:
    typedef int Count;
    typedef std::vector<int> Ints;
    /// absent
    std::vector<int> Make() const;
    /// absent
    void Take(std::vector<char> kValue);
    /// absent
    void ByRef(const std::vector<long>& kRef, std::vector<double>* pkPtr);
private:
    int m_iCount;  ///< +0x0
};

/// absent
std::vector<short> FreeMake(std::vector<unsigned char> kValue);

// Templates: types depend on the parameters, so they stay null and are flagged.
/// size 0x4
template <class T>
class FBox {
public:
    typedef T value_type;
    typedef std::vector<T> Items;
    /// absent
    T Get() const;
    /// absent
    void Put(const T& kValue);
private:
    T m_kValue;  ///< +0x0
};

template <class T> using FItems = std::vector<T>;

/// absent
template <class T> T FreeTemplate(T kValue);
