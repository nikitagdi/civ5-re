/// origin binary
//------------------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//!  \file		FVariableSystem.h
//!  \author	Bart Muzzin - 11/22/2004
//!	 \brief		Implementation of a runtime modifiable set of variables (header).
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2007 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef		FVARIABLESYSTEM_H
#define		FVARIABLESYSTEM_H
#pragma		once

#if defined(_MSC_VER)
#include <hash_map>
#else
#include <unordered_map>
#endif

#include "FirePlace/FireWorks/FString.h"

//! Represents the different types of data an FVariable can represent.
enum eVariableType
{
	FVARTYPE_BOOL,		//!< Boolean value.
	FVARTYPE_CHAR,		//!< One byte integer (signed).
	FVARTYPE_UCHAR,		//!< One byte integer (unsigned).
	FVARTYPE_SHORT,		//!< Two byte integer (signed).
	FVARTYPE_USHORT,	//!< Two byte integer (unsigned).
	FVARTYPE_INT,		//!< Four byte integer (signed).
	FVARTYPE_UINT,		//!< Four byte integer (unsigned).
	FVARTYPE_FLOAT,		//!< Four byte floating point number.
	FVARTYPE_DOUBLE,	//!< Eight byte floating point number.
	FVARTYPE_STRING,	//!< String data (uses FString).
	FVARTYPE_WSTRING,	//!< String data (uses FStringW).
	FVARTYPE_COUNT
};
class FDataStream;
/// absent
FDataStream & operator<<(FDataStream &, const eVariableType &);
/// absent
FDataStream & operator>>(FDataStream &, eVariableType &);

class FVariable;
class FVariableDebug;

// Determine which type of variable we will use - final_release uses FVariable, all else use FVariableDebug
#ifdef		FINAL_RELEASE
#define FVARTYPE FVariable
#else	//	FINAL_RELEASE
#define FVARTYPE FVariableDebug
#endif	//	FINAL_RELEASE

#if defined(_MSC_VER)
typedef STDEXT::hash_map< FString, FVARTYPE*, FStringHashTraits > FVariableHash;
#else
typedef std::unordered_map< FString, FVARTYPE*, FStringHashTraits, FStringHashTraits > FVariableHash;
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASS:	FVariableCallback
//
//! \brief Used with FVariableSystem and FVariable as a callback when an FVariable is affected.
//!
//!	Variables that require a callback to perform some function can derive from this class
//! and override the desired methods.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size 0x4
/// vtable ?
class FVariableCallback
{
	public:
        /// inline
        virtual ~FVariableCallback() {};
		/// inline
		virtual void VariableAttached(const FVARTYPE * pkVariable) { };
		/// inline
		virtual void VariableHasChanged(const FVARTYPE * pkVariable) { };
		/// inline
		virtual void VariableRemoved(const FVARTYPE * pkVariable) { };
		/// inline
		virtual bool FinalValueOnly() const { return false; }				// Does this callback want slider values as the slider is moving or only the final value (when the button is unpressed)?
		/// inline
		virtual bool ApplyInitialState() const { return true; }				// Should this callback apply it's initial state when the var is first registered?
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASS:	FVariable
//
//! \brief Used with FVariableSystem to create a set of run-time variables.
//!
//! Note that there are no constructors or methods for this class, and all data is public.
//! This is done intentionally to reduce overhead, and this class should rarely be accessed
//! outside of FVariableSystem code. There is a destructor however, because the data contained
//! inside may need to be freed, such as in the case of string data.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FDataStream;
/// size 0x10
/// vtable ?
class FVariable
{
	public:
		/// inline
		FVariable() : m_dValue(0) {}
		/// inline
		FVariable(const FVariable& src) { CopyFrom(src); }
		/// absent
		virtual ~FVariable();

		/// inline
		const FVariable& operator=( const FVariable& varSrc ) { CopyFrom(varSrc); return *this; }
		/// absent
		void CopyFrom(const FVariable& varSrc);
		/// absent
		void Read(FDataStream &);
		/// absent
		void Write(FDataStream &) const;

		/// size 0x8
		union
		{
			//!< Boolean data
			bool		m_bValue; ///< +0x4
			//!< One byte integer (signed) data.
			char		m_cValue; ///< +0x4
			//!< One byte integer (unsigned) data.
			byte		m_ucValue; ///< +0x4
			//!< Two byte integer (signed) data.
			short		m_wValue; ///< +0x4
			//!< Two byte integer (unsigned) data.
			word		m_uwValue; ///< +0x4
			//!< Four byte integer (signed) data.
			int			m_iValue; ///< +0x4
			//!< Four byte integer (unsigned) data.
			uint		m_uiValue; ///< +0x4
			//!< Four byte floating point data.
			float		m_fValue; ///< +0x4
			//!< Eight byte floating point data.
			double		m_dValue; ///< +0x4
			//!< String data.
			char *		m_szValue; ///< +0x4
			//!< Wide string data.
			wchar_t *	m_wszValue; ///< +0x4
		};

		//!< The type of data contained in this variable.
		eVariableType	m_eType; ///< +0xc

		/// inline
		virtual void SetCategory( const char * szCategory) { };
		/// inline
		virtual void SetEditType( const char * szEditType) { };
		/// inline
		virtual void SetCallback( FVariableCallback * pkCallback ) { };

		/// inline
		virtual const char * GetCategory() const
		{
			return "Default";
		}

		/// inline
		static const char * GetEditType( eVariableType eType )
		{
			switch( eType )
			{
			case FVARTYPE_BOOL: return "Check"; break;
			case FVARTYPE_CHAR: return "Char"; break;
			case FVARTYPE_UCHAR: return "Char"; break;
			case FVARTYPE_SHORT: return "Int"; break;
			case FVARTYPE_USHORT: return "UInt"; break;
			case FVARTYPE_INT: return "Int"; break;
			case FVARTYPE_UINT: return "UInt"; break;
			case FVARTYPE_FLOAT: return "Float"; break;
			case FVARTYPE_DOUBLE: return "Float"; break;
			default: case FVARTYPE_STRING: return "String"; break;
			case FVARTYPE_WSTRING: return "String"; break;
			}
		}

		/// inline
		static eVariableType GetEditType( const char * szType )
		{
			if ( !_stricmp(szType, "Check" ) ) return FVARTYPE_BOOL; 
			if ( !_stricmp(szType, "Char")) return FVARTYPE_CHAR; 
			if ( !_stricmp(szType, "Char")) return FVARTYPE_UCHAR; 
			if ( !_stricmp(szType, "Int")) return FVARTYPE_SHORT; 
			if ( !_stricmp(szType, "UInt")) return FVARTYPE_USHORT; 
			if ( !_stricmp(szType, "Int")) return FVARTYPE_INT; 
			if ( !_stricmp(szType, "UInt")) return FVARTYPE_UINT; 
			if ( !_stricmp(szType, "Float"))return FVARTYPE_FLOAT; 
			if ( !_stricmp(szType, "Float")) return FVARTYPE_DOUBLE; 
			if ( !_stricmp(szType, "String")) return FVARTYPE_STRING; 
			if ( !_stricmp(szType, "String")) return FVARTYPE_WSTRING; 
			return FVARTYPE_INT; // ?
		}

		/// inline
		virtual const char * GetEditType() const
		{
			return GetEditType(m_eType);
		}

		/// inline
		virtual FVariableCallback * GetCallback() const { return NULL; }

		/// inline
		virtual void SetVariableRange( const char * szMin, const char * szMax ) {};
		/// inline
		virtual const char * GetVariableMinimum() const { return NULL; };
		/// inline
		virtual const char * GetVariableMaximum() const { return NULL; };
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASS:	FVariableDebug
//
//! \brief Used with FVariableSystem. Like FVariable, but with some extra customizable information.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size 0x24
/// vtable ?
class FVariableDebug : public FVariable
{
	public:
		/// inline
		FVariableDebug() :  FVariable(),  m_szCategory(NULL),  m_szEditType(NULL),  m_pkVariableCallback(NULL),
			m_szMinimum(NULL), m_szMaximum(NULL) {}
		/// inline
		FVariableDebug(const FVariableDebug& src) : FVariable(src) 
		{  
			m_szCategory = src.m_szCategory;
			m_szEditType = src.m_szEditType;
			m_szMinimum = src.m_szMinimum;
			m_szMaximum = src.m_szMaximum;
			m_pkVariableCallback = src.m_pkVariableCallback;
		}
		/// inline
		virtual ~FVariableDebug() {}

		/// inline
		virtual const char * GetCategory() const { return m_szCategory ? m_szCategory : FVariable::GetCategory(); }
		/// inline
		virtual void SetCategory(const char * szCategory) { m_szCategory = szCategory; }
		/// inline
		virtual const char * GetEditType() const { return m_szEditType ? m_szEditType : FVariable::GetEditType(); }
		/// inline
		virtual void SetEditType(const char * szEditType) { m_szEditType = szEditType; }
		/// inline
		virtual FVariableCallback * GetCallback() const { return m_pkVariableCallback; }
		/// inline
		virtual void SetCallback( FVariableCallback * pkCallback ) { m_pkVariableCallback = pkCallback; }
		/// inline
		virtual void SetVariableRange( const char * szMin, const char * szMax )
		{
			m_szMinimum = szMin;
			m_szMaximum = szMax;
		};
		/// inline
		virtual const char * GetVariableMinimum() const { return m_szMinimum; };
		/// inline
		virtual const char * GetVariableMaximum() const { return m_szMaximum; };


	private:
		const char *		m_szCategory; ///< +0x10
		const char *		m_szEditType; ///< +0x14
        FVariableCallback * m_pkVariableCallback; ///< +0x18

		// Ranges are stored as strings - they aren't used internally, only as hints to
		// debugging applications (ie. FireTune tools). If they are NULL, they aren't used
		const char *		m_szMinimum; ///< +0x1c
		const char *		m_szMaximum; ///< +0x20
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CLASS:	FVariableSystem
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! \brief Creates a system in which variables can be added/removed/queried/modified at runtime.
//!
//! This should be used when the application is managing variable data obtained from/exposed to an external source.
//! For example, if variables are read from an XML file, and the variable names are not known beforehand, this system
//! can manage them.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size 0x1c
/// vtable ?
class FVariableSystem
{
	public:

		// Constructor/Destructor
		/// absent
		FVariableSystem( );
		/// absent
		virtual ~FVariableSystem( );

		/// absent
		void UnInit();

		// Number of variables in the system
		/// absent
		uint GetSize() const;

		// Variable accessors
		/// absent
		bool GetValue( const char * szVariable, bool & bValue ) const;
		/// absent
		bool GetValue( const char * szVariable, char & cValue ) const;
		/// absent
		bool GetValue( const char * szVariable, byte & ucValue ) const;
		/// absent
		bool GetValue( const char * szVariable, short & wValue ) const;
		/// absent
		bool GetValue( const char * szVariable, word & uwValue ) const;
		/// absent
		bool GetValue( const char * szVariable, int & iValue ) const;
		/// absent
		bool GetValue( const char * szVariable, uint & uiValue ) const;
		/// absent
		bool GetValue( const char * szVariable, float & fValue ) const;
		/// absent
		bool GetValue( const char * szVariable, double & dValue ) const;
		/// absent
		bool GetValue( const char * szVariable, const char * & pszValue ) const;
		/// absent
		bool GetValue( const char * szVariable, const wchar * & pszValue ) const;
		/// absent
		const FVARTYPE * GetVariable( const char * szVariable ) const;
		/// absent
		FVARTYPE * GetVariable( const char * szVariable );

		// Variable additions/modifiers. If a variable does not exist, it will be added.
		/// absent
		void SetValue( const char * szVariable, bool bValue );
		/// absent
		void SetValue( const char * szVariable, char cValue );
		/// absent
		void SetValue( const char * szVariable, byte ucValue );
		/// absent
		void SetValue( const char * szVariable, short wValue );
		/// absent
		void SetValue( const char * szVariable, word uwValue );
		/// absent
		void SetValue( const char * szVariable, int iValue );
		/// absent
		void SetValue( const char * szVariable, uint uiValue );
		/// absent
		void SetValue( const char * szVariable, float fValue );
		/// absent
		void SetValue( const char * szVariable, double dValue );
		/// absent
		void SetValue( const char * szVariable, const char * szValue );
		/// absent
		void SetValue( const char * szVariable, const wchar * wszValue );
		/// absent
		bool SetValueFromType( const char * szVariable, const char * szType, const char * szValue );

		// Category, Edit type and callback overridding
		/// absent
		bool SetCategoryEditTypeCallback( const char * szVariable, 
			const char * szCategory, const char * szEditType, FVariableCallback * pkCallback );
		/// absent
		bool SetVariableRange( const char * szVariable, const char * szMin, const char * szMax );

		// Variable removal
		/// absent
		bool RemValue( const char * szVariable );

		// Iteration
		/// absent
		std::string GetFirstVariableName( );
		/// absent
		std::string GetNextVariableName( );

		/// absent
		void Read(FDataStream &);
		/// absent
		void Write(FDataStream &) const;

protected:

		// Helper function for setting values
		/// absent
		FVARTYPE * PreSetValue(const char * szVariable);

		//!< Hash map of variable types
		FVariableHash				m_mapVariableMap; ///< +0x4
		//!< Current iterator used with GetFirst/NextVariableName
		FVariableHash::iterator		m_iVariableIterator; ///< +0x18


};

/// absent
FDataStream & operator<<(FDataStream &, const FVariable &);
/// absent
FDataStream & operator>>(FDataStream &, FVariable &);

// Phasing out using .inl files. VC90 projects will now use a .cpp instead.
#if _MSC_VER <= 1400
//#include "FVariableSystem.inl"
#endif

#endif	//	FVARIABLESYSTEM_H
