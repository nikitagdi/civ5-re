/// origin binary
//	$Revision: #4 $		$Author: mbreitkreutz $ 	$DateTime: 2005/06/13 13:35:55 $
//------------------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:    FInputDevice.h
//
//  AUTHOR:  Mustafa Thamer  --  09/10/2003
//		Revision 2 - 6/6/06, Mustafa Thamer
//
//  CLASS:      FInputDevice
//
//  PURPOSE:    Samples input device and sets mapping to indicate what input events
//		happened since the last update call.
//
//		Also provides immediate (stateless) mode functionality for queries like	isKeyDown.
//
//		HotKey mappings can be set which will map input1 to input2.  When input1 happens, input2 
//			will be reported.
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2003-2006 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef		FINPUTDEVICE_H
#define		FINPUTDEVICE_H
#pragma		once

#include <map>

#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"

/// size 0x24
/// vtable ?
class FInputDevice
{
public:

	enum
	{
		// general input type
		INPUT_NONE		= 0,

		// general state values
		UNINIT		= -1,
		UP,
		DOWN,
		DBL_CLICK
	};


	// holds the value of an input event
	/// size 0xc
	struct InputValue
	{
		// used for key state, UNINIT/UP/DOWN/DBL_CLICK
		int m_iState; ///< +0x0
		// 2D int coords, used for mouse/controller analog
		POINT m_point; ///< +0x4
		/// inline
		InputValue(int iState=UNINIT, int px=-1, int py=-1) : m_iState(iState) {m_point.x=px; m_point.y=py;  }
		/// inline
		bool operator==(const InputValue& in) const		{ return in.m_iState==m_iState && in.m_point.x==m_point.x && in.m_point.y==m_point.y; }
	};

	// holds the input type and data
	/// size 0x18
	struct InputEvent
	{
		uint m_uiTimeStamp; ///< +0x0
		// CTRL/ALT/SHIFT/ControllerNum (see FKBInputDevice.h), hit bit is reserved to indicate key repeat
		uint m_uiModifiers; ///< +0x4
		// state and location
		InputValue m_val; ///< +0x8
		// input type
		uint m_type; ///< +0x14

		/// inline
		InputEvent() : m_uiTimeStamp(0), m_uiModifiers(0), m_type(INPUT_NONE) {}
		/// inline
		InputEvent(uint uiType, InputValue iv, uint uiTime, uint uiMod=0) : 
			m_uiTimeStamp(uiTime), m_uiModifiers(uiMod), m_val(iv), m_type(uiType)  {}
		/// inline
		bool operator==(const InputEvent& in) const		// ignores time
		{ return (in.m_val==m_val && in.m_type==m_type && in.m_uiModifiers==m_uiModifiers); }
		
		/// inline
		int GetControllerNum() const { return (m_uiModifiers & 0xffff); }

		// check hi bit to indicate that the event was triggered as a key repeat
		/// inline
		bool IsKeyRepeat() const { return (m_uiModifiers & 0x8000000) != 0; }
		/// inline
		void SetKeyRepeat(bool bVal) { if (bVal) m_uiModifiers |= 0x8000000; else m_uiModifiers &= ~0x8000000; }
	};
	typedef std::vector<InputEvent, FSTL_Tagged_Allocator<InputEvent, 16,c_eMPoolTypeContainer> > InputEventsList;	

	/// absent
	FInputDevice();
	/// absent
	virtual ~FInputDevice();

	/// absent
	bool Init(unsigned long flags);		// flags are usually 0, but check with specific devices to be sure
	/// absent
	bool UnInit();									
	/// pure
	virtual bool Update(uint uiCurTime) = 0;			// if enabled, sample input and fill in InputEventList
	
	/// absent
	void SetEnabled(bool bEnabled);		// clears events when disabled
	/// inline
	bool GetEnabled() const { return m_bEnabled; }
	/// inline
	unsigned long GetFlags() const { return m_ulFlags; }		

	// access the map of buffered input events
	/// inline
	const InputEventsList& GetInputEvents() const { return m_InputEvents;	}	// return list of input events

	// immediate mode (polling) accessors
	/// pure
	virtual bool GetInputImmediate(InputValue *pResultValue, uint uiInputType, int iControllerNum=0) const = 0;

	// hot key mappings
	/// absent
	void AddHotKeyMapping(uint inputFromType, uint inputToType);	// Will overwrite any existing mapping for that input.
	/// absent
	bool RemoveHotKeyMapping(uint inputFromType);
	/// absent
	void ClearHotKeyMappings();
	/// absent
	uint TranslateHotKey(uint inputFromType) const;					// Call to use hot key mappings, return inputType
protected:
	typedef std::map<uint /*InputType*/, uint /*InputType*/,
		std::less<uint>, FSTL_Tagged_Allocator<std::pair<const uint, uint>, 16,c_eMPoolTypeContainer> > HotKeyMap;	// map of inputs to inputs (internal use only)

	/// pure
	virtual bool IInit() = 0;			// device specific init
	/// pure
	virtual bool IUnInit() = 0;			// device specific uninit

	HotKeyMap m_HotKeyMap; ///< +0x4
	InputEventsList m_InputEvents; ///< +0x10
	bool m_bInitted; ///< +0x1c
	bool m_bEnabled; ///< +0x1d
	uint m_ulFlags; ///< +0x20
};

#endif	//FINPUTDEVICE_H
