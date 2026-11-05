// SDK copy before markup: doxygen comments are ordinary comments with markers="off".
#pragma once

/// The bag of values.
/// 0x834 is not an address here.
struct DocBag {
    int m_iCount;  ///< number of values
    /// pointer to values
    int* m_pValues;
};

/// Sums a bag.
int DocSum(const DocBag& bag);
