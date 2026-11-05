/// origin guess
#pragma once

class FGXVector3;
class FloodplainsDepthVert;

/// size ?
class FloodplainsDepthRasterFunc {
public:
    /// 0x08661928
    UNK_RET operator()(int a1, int a2, FGXVector3 const& a3, FloodplainsDepthVert const* a4, FloodplainsDepthVert const* a5, FloodplainsDepthVert const* a6, unsigned int a7);
};
