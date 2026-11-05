/// origin binary
// A cell of the map.
/// size 0x2c
/// vtable 0x0a1b2c40
class CvCell : public CvBase {
public:
	// Returns the owner.
	/// 0x0834a1c0 fastcc
	/// base 0x0834a200
	int GetOwner() const;
	int m_iX; ///< +0x10
	/// pure
	virtual void Foo() = 0;
};
