/// origin binary
#include "FirePlace/FireWorks/FAutoArchive.h"

#include <algorithm>

#include "FirePlace/FireWorks/FAutoVariableBase.h"
#include "FirePlace/FireWorks/FDataStream.h"

/// 0x08dc3564
FAutoArchive::FAutoArchive()
{
}

/// 0x08dc35f4
FAutoArchive::~FAutoArchive()
{
}

/// 0x08dc368a
void FAutoArchive::remove(FAutoVariableBase & variable)
{
	std::vector<FAutoVariableBase *>::iterator found = std::find(m_contents.begin(), m_contents.end(), &variable);
	if (found != m_contents.end())
		m_contents.erase(found);
}

/// 0x08dc3758
void FAutoArchive::load(FDataStream & loadFrom)
{
	for (FAutoVariableBase * variable : m_contents)
		variable->load(loadFrom);
}

/// 0x08dc3716
void FAutoArchive::save(FDataStream & saveTo) const
{
	for (FAutoVariableBase * variable : m_contents)
		variable->save(saveTo);
}

/// 0x08dc379a
void FAutoArchive::saveDelta(FDataStream & saveTo, std::vector<std::pair<std::string, std::string> > & callStacks) const
{
	UINT32 deltaCount = static_cast<UINT32>(m_deltas.size());
	saveTo << deltaCount;
	for (FAutoVariableBase * delta : m_deltas)
	{
		std::vector<FAutoVariableBase *>::const_iterator found = std::find(m_contents.begin(), m_contents.end(), delta);
		if (found != m_contents.end())
		{
			UINT32 index = static_cast<UINT32>(found - m_contents.begin());
			saveTo << index;
			delta->saveDelta(saveTo);
		}
	}
	m_deltas.clear();
}

/// 0x08dc387c
void FAutoArchive::clearDelta()
{
	for (FAutoVariableBase * delta : m_deltas)
		delta->clearDelta();
	m_deltas.clear();
}

/// 0x08dc38f4
void FAutoArchive::reset()
{
	for (FAutoVariableBase * variable : m_contents)
		variable->reset();
}

/// 0x08dc3a6a
bool FAutoArchive::hasDeltas() const
{
	return !m_deltas.empty();
}

/// 0x0a312f38
bool FAutoArchive::ms_bCallStacksEnabled = false;

/// 0x08dc3a7a
void FAutoArchive::EnableCallStacks(bool bState)
{
	ms_bCallStacksEnabled = bState;
}
