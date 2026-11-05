/// origin binary
#pragma once

/// size 0x8
class Plain {
public:
    int m_a;  ///< +0x0
    int m_b;  ///< +0x4
};

/// size ?
class Unknown {
public:
    int m_a;  ///< +0x0
};

/// size 0xc
class Roomy {
public:
    int m_a;  ///< +0x0
    int m_b;  ///< +0x4
    int m_c;  ///< +0x8
};

/// size 0x4
class NoEvidence {
public:
    int m_a;  ///< +0x0
};
