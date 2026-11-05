/// origin guess
// $Header
//------------------------------------------------------------------------------
#pragma once 

#include <Windows.h>
#include "Civ5/Src/CvGameDatabase/Database.h"
#ifndef _CVLOCALIZATION_H
#define _CVLOCALIZATION_H

/*
	This header represents a public amalgamation of the CvLocalization library.
	It is the combination of public interfaces from many files simplified into
	a single file.
*/

#include <iostream>		//! Needed for ostream helper 
#include <memory>		//! Needed for std::tr1::shared_ptr
#include <string>		//! Needed for std::string
#include <vector>		//! Needed for std::vector

#ifndef CvLocalizationAPI
#define CvLocalizationAPI __declspec(dllimport)
#endif

#include "Civ5/Src/CvLocalization/ILocalizationLogger.h"

namespace Localization
{

	//Public Enumerations
	enum Gender{
		GENDER_NEUTER = 0,
		GENDER_MASCULINE,
		GENDER_FEMININE,
	};

	//Forward Declarations
	class String;
	class StringDictionary;

	/// size 0x4
	/// vtable ?
	class IPluralityRule
	{
	public:

		/// pure
		virtual unsigned int GetMaxPluralForms() const = 0;
		/// pure
		virtual unsigned int GetPluralForm(int nElements) const = 0;
	};

	//TODO: Document Localization::String::Parameter.
	/// size 0x28
	class CvLocalizationAPI StringParameter
	{
	public:
		/// 0x08c66e20
		StringParameter();
		/// 0x08c66ec2
		StringParameter(const StringParameter& other);

		int ArgumentIndex; ///< +0x0
		size_t uiTextKeyStart; ///< +0x4
		size_t lenTextKey; ///< +0x8

		size_t uiParameterStart; ///< +0xc
		size_t uiParameterEnd; ///< +0x10

		size_t uiTransformTagStart; ///< +0x14
		size_t uiTransformTagEnd; ///< +0x18

		std::vector<StringParameter> m_injected_parameters; ///< +0x1c
	};

	//TODO: Document Localization::String::Argument.
	/// size 0x14
	class CvLocalizationAPI ParameterArgument
	{
	public:
		enum Type{
			ARGTYPE_INT,
			ARGTYPE_INT64,
			ARGTYPE_FLOAT,
			ARGTYPE_DOUBLE,
			ARGTYPE_STRING,
		};

		/// 0x08c6512a
		ParameterArgument(const ParameterArgument& other);
		/// 0x08c65226
		ParameterArgument(const String& strText);
		/// 0x08c65178
		ParameterArgument(const int val);
		/// 0x08c651cc
		ParameterArgument(const __int64 val);
		/// 0x08c651a0
		ParameterArgument(const float val);
		/// 0x08c651fa
		ParameterArgument(const double val);

		/// size 0x8
		union {
			int		i; ///< +0x0
			__int64	i64; ///< +0x0
			float	f; ///< +0x0
			double	d; ///< +0x0
		} m_value; ///< +0x0

		std::shared_ptr<String> m_str; ///< +0x8

		Type m_type; ///< +0x10
		// seed: in Civ5XP, not in SDK
	public:
		/// 0x08c65108
		ParameterArgument();
		/// 0x08c652dc
		UNK_RET operator=(Localization::ParameterArgument const& a1);
	};

	/// size 0x4
	/// vtable 0x0a300c40
	class ParameterTransforms
	{
	public:
		/// 0x08c5b21c
		ParameterTransforms(const char* szLocaleCode);
		/// 0x08c5b368
		/// deleting 0x08c5b31c
		virtual ~ParameterTransforms();

		//Numeric transforms
		/// absent
		CvLocalizationAPI bool FormatCurrency(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		/// absent
		CvLocalizationAPI bool FormatNumber(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		/// absent
		CvLocalizationAPI bool FormatPercent(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		/// absent
		CvLocalizationAPI bool FormatRoman(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		/// absent
		CvLocalizationAPI bool FormatSpellout(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;

		//String transforms
		/// absent
		CvLocalizationAPI bool FormatUppercase(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		/// absent
		CvLocalizationAPI bool FormatLowercase(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;;
		/// absent
		CvLocalizationAPI bool FormatTitlecase(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		/// absent
		CvLocalizationAPI bool FormatTextkey(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;

		//Choice transforms
		/// absent
		CvLocalizationAPI bool ChoiceFormatGender(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		/// absent
		CvLocalizationAPI bool ChoiceFormatPlurality(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		/// absent
		CvLocalizationAPI bool ChoiceFormatText(const Localization::ParameterArgument& arg, const std::string& style_args, std::string& outStr) const;
		// seed: in Civ5XP, not in SDK
	public:
		/// 0x08c5b3ae
		UNK_RET DestroyAllFormatters();
		/// 0x08c5b434
		UNK_RET FormatCurrency(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5b55a
		UNK_RET CreateCurrencyFormatter() const;
		/// 0x08c5b5fe
		UNK_RET FormatPercent(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5b724
		UNK_RET CreatePercentFormatter() const;
		/// 0x08c5b7c8
		UNK_RET FormatNumber(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5bb84
		UNK_RET CreateNumberFormatter() const;
		/// 0x08c5bc40
		UNK_RET FormatRoman(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5bf2e
		UNK_RET CreateRomanFormatter() const;
		/// 0x08c5c016
		UNK_RET FormatSpellout(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5c262
		UNK_RET CreateSpelloutFormatter() const;
		/// 0x08c5c34a
		UNK_RET FormatOrdinal(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5c596
		UNK_RET CreateOrdinalFormatter() const;
		/// 0x08c5c67e
		UNK_RET FormatUppercase(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5c750
		UNK_RET FormatLowercase(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5c822
		UNK_RET FormatTitlecase(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5c8fc
		UNK_RET FormatTextkey(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5c9f2
		UNK_RET ChoiceFormatGender(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
		/// 0x08c5ca94
		UNK_RET ChoiceFormatPlurality(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument>> const& a2, std::string const& a3, std::string& a4) const;
	};

	/*
		Represents a single supported language.
		This class will also contain any implementation details specific to a language.
	*/
	/// size 0x4
	/// vtable 0x0a300bf8
	class Language
	{
	public:
		/// 0x08c5abbc
		Language(const char* szLanguageType, Localization::IPluralityRule* pkPluralityRule);
		/// 0x08c5ae44
		Language(const Language& copy);
		/// 0x08c5adf6
		/// deleting 0x08c5adce
		virtual ~Language();

		// General information
		/// 0x08c5afb6
		CvLocalizationAPI const char* GetType() const;

		// Plurality
		/// 0x08c5afbe
		CvLocalizationAPI unsigned int GetPluralForm(int nElements) const;
		/// 0x08c5aff2
		CvLocalizationAPI Localization::IPluralityRule* GetPluralityRule() const;
		/// 0x08c5ad3c
		CvLocalizationAPI void SetPluralityRule(Localization::IPluralityRule* pkPluralityRule);

		/// 0x08c5affa
		CvLocalizationAPI bool Compare(const char* szString1, size_t lenString1, const char* szString2, size_t lenString2, int& result) const;

		// Language specific transformations
		/// 0x08c5b080
		CvLocalizationAPI const Localization::ParameterTransforms& GetParameterTransforms() const;
	};

	//Initialization/Shutdown
	/// 0x08c635b0
	CvLocalizationAPI bool Initialize(const wchar_t* wszIcuDatFile, Database::Connection& db, const char* szLanguagesTableName, const char* szLanguageType, bool bDisableFallbackLanguageSupport);
	/// 0x08c637db
	CvLocalizationAPI bool Shutdown();

	//Configuration
	/// 0x08c638bc
	CvLocalizationAPI bool SetCurrentLanguage(const char* szType);
	/// 0x08c6389b
	CvLocalizationAPI const Localization::Language& GetCurrentLanguage();
	//CvLocalizationAPI const Localization::Language& GetLanguage(const char* szType);

	/// 0x0d265c9c
	extern Localization::StringDictionary* m_pkDictionary;

	// Dictionary Lookup
	/// 0x08c63a8a
	CvLocalizationAPI const Localization::StringDictionary& GetStringDictionary();
	/// 0x08c63a9f
	CvLocalizationAPI Localization::String Lookup(const char* textKey);
	//CvLocalizationAPI Localization::String Lookup(const char* textKey, Localization::Language& language);
	/// 0x08c63836
	CvLocalizationAPI bool ShutdownDictionary();
	/// 0x08c636ff
	CvLocalizationAPI bool InitializeDictionary(Database::Connection& db, const char* szLanguagesTableName, const char* szLanguageType, bool bDisableFallbackLanguageSupport);


	//Logging
	/// 0x08c5b114
	CvLocalizationAPI void SetLogger(ILocalizationLogger* pkLogger);
	/// absent
	CvLocalizationAPI ILocalizationLogger* GetLogger();
	/// 0x08c5b147
	CvLocalizationAPI void LogMessage(const char* szMessage);
	/// 0x08c5b180
	CvLocalizationAPI void LogWarning(const char* szWarning);
	/// 0x08c5b1b9
	CvLocalizationAPI void LogError(const char* szError);

	//Helper Methods
	/// 0x08c63c17
	CvLocalizationAPI Localization::String ConvertUTCDatetoLocalTZDate(SYSTEMTIME& stUniversalTime, _In_opt_z_ const char* szDTFormat);
	/// 0x08c63dee
	CvLocalizationAPI Localization::String ConvertUTCTimetoLocalTZTime(SYSTEMTIME& stUniversalTime, _In_opt_z_ const char* szTimeFormat);
	/// 0x08c64067
	CvLocalizationAPI Localization::String ConvertUTCDateTimetoLocalTZDateTime(SYSTEMTIME& stUniversalTime, _In_opt_z_ const char* szDateFormat, _In_opt_z_ const char* szTimeFormat);


	/*
		This class represents a shared immutable raw string.
		Multiple instances of this class access the same buffer
		via a shared pointer.  Each instance also stores the original
		size of the buffer, and substring info.
	*/
	/// size 0x14
	class CvLocalizationAPI SharedString
	{
	public:
		/// 0x08c65954
		SharedString();
		/// 0x08c6597c
		SharedString(_In_bytecount_(length) const char* szString, size_t length);
		/// 0x08c659f8
		SharedString(const SharedString& copy);

		/// 0x08c65a42
		SharedString& operator = (const SharedString& other);

		//! Obtains the original buffer with a given length.
		/// 0x08c65b06
		const char* GetBuffer(size_t& length) const;

		//! Obtains the buffer of the substring w/ length.
		/// 0x08c65af2
		const char* GetString(size_t& length) const;

		//! Obtains the length of the string buffer.
		/// 0x08c65b3c
		size_t GetBufferLength() const;

		//! Obtains the offset of the substring.
		/// 0x08c65b44
		size_t GetOffset() const;

		//! Obtains the length of the substring.
		/// 0x08c65b4c
		size_t GetLength() const;

		//! Sets the offset and length for a buffer.
		/// 0x08c65b16
		bool SetOffsetAndLength(size_t offset, size_t length);

	private:
		//Shared pointer to a raw string buffer.
		std::shared_ptr<char> m_Buffer; ///< +0x0

		//Length of string buffer, used for checking per instance offsets and lengths.
		size_t m_BufferLength; ///< +0x8

		//! Per instance offset.
		size_t m_offset; ///< +0xc

		//! Per instance length.
		size_t m_length; ///< +0x10
		// seed: in Civ5XP, not in SDK
	public:
		/// 0x08c65aa4
		UNK_RET operator==(Localization::SharedString const& a1) const;
	};

	//This class is publicly accessed.
	//It's only here so that the compiler can generate a proper size for String.
	/// size 0x50
	/// vtable 0x0a301018
	class SingleFormString
	{
	public:
		/// 0x08c65b54
		SingleFormString();
		/// 0x08c65bea
		SingleFormString(const SharedString& baseString, size_t plurality = 0, Gender gender = GENDER_NEUTER);
		/// 0x08c65d76
		/// deleting 0x08c65d4e
		virtual ~SingleFormString();

		/// 0x08c65dce
		SingleFormString& operator = (const SingleFormString& other);

		/// 0x08c65e68
		const SharedString& GetBaseString() const;

		/// 0x08c65e70
		const SharedString& GetComposedString() const;

		/// absent
		void SetPlurality();
		/// 0x08c65e8a
		size_t GetPlurality() const;

		/// 0x08c65e92
		void SetGender(Gender gender);
		/// 0x08c65ea4
		Gender GetGender() const;

		/// absent
		bool HasTrait(const char* szTrait, size_t lenTrait) const;

		/// 0x08c65ff2
		bool Parse();
		/// 0x08c6607a
		bool Compose(const std::vector<ParameterArgument>& arguments);
	private:
		SharedString m_BaseString; ///< +0x4
		SharedString m_ComposedString; ///< +0x18

		size_t m_Plurality; ///< +0x2c
		int m_Gender; ///< +0x30
		std::vector<std::string> m_Traits; ///< +0x34
		std::vector<StringParameter> m_Parameters; ///< +0x40

		bool m_Composed; ///< +0x4c
		bool m_Parsed; ///< +0x4d
		// seed: in Civ5XP, not in SDK
	public:
		/// 0x08c65c88
		SingleFormString(Localization::SingleFormString const& a1);
		/// 0x08c65e78
		UNK_RET SetPlurality(unsigned int a1);
		/// 0x08c65eac
		UNK_RET AddTrait(Localization::SharedString const& a1);
		/// 0x08c65f20
		UNK_RET HasTrait(Localization::SharedString const& a1) const;
		/// 0x08c65f6a
		UNK_RET ClearTraits();
		/// 0x08c65fb0
		UNK_RET GetTraits() const;
		/// 0x08c65fb8
		UNK_RET SetTraits(std::vector<Localization::SharedString, std::allocator<Localization::SharedString>> const& a1);
	};

	/* Localization::String (NOT THREAD-SAFE) 
		This class represents a fully localized string composed from a single base string with multiple arguments.
		It has the following features:
			
			*lazy composition of strings (only composes the final string once and when requested)
			 internal string pool for most data.

			*locale-aware text transforms (upper case, lower case, title case)
			*locale-aware numeric transforms (integer, currency, percentage, roman numeral, spell-out)
			*locale-aware plurality selection
			*conditional transforms with optional support for plurality and gender

		A unique syntax is used to handle composition and conditional transforms. For more information about this 
		syntax please view CvLocalization*.doc in the project's base folder.


		This class has the following dependencies:
			*A singleton instance of Localization::System for obtaining the default language as well as locale-specific
			 data.
			
			*A singleton instance of Localization::StringDictionary for internal text-key lookups.
	*/
	/// size 0x48
	/// vtable 0x0a300cb8
	class String
	{
	public:
		/// 0x08c5cd04
		CvLocalizationAPI String();
		/// 0x08c5cd8c
		CvLocalizationAPI String(const char* szString);
		/// 0x08c5d162
		CvLocalizationAPI String(const char* szString, unsigned int uiLen);
		/// 0x08c5d3d2
		CvLocalizationAPI String(const String& other);

		/// 0x08c5d4ae
		/// deleting 0x08c5d486
		CvLocalizationAPI virtual ~String();


		//FLAGS
		//! Determine if the string is empty.
		/// 0x08c5d636
		CvLocalizationAPI bool IsEmpty() const;


		//OPERATORS
		/// 0x08c5d506
		CvLocalizationAPI String& operator = (const String& other);
		/// 0x08c5d58a
		CvLocalizationAPI String& operator = (_In_z_ const char* szBaseString);


		//ACESSORS
		//! Returns a UTF-8 formatted character array of the composed string.
		//! @param bytes [out] the size of the returned array, in bytes.
		//! @return A UTF-8 formatted character array representing the composed string.
		/// 0x08c5d676
		CvLocalizationAPI const char* toUTF8();
		/// 0x08c5d728
		CvLocalizationAPI const char* toUTF8(size_t& bytes, int nForm = 1);

		//! Returns a UTF-8 formatted character array of the base string.
		//! @param bytes [out] the size of the returned array, in bytes.
		//! @return A UTF-8 formatted character array representing the base string.
		/// 0x08c5d7f2
		CvLocalizationAPI const char* GetBaseString(size_t& bytes) const;


		// LANGUAGE METHODS
		//! Gets the currently assigned language.
		//! By default strings are not explicitly assigned a language and
		//! instead just use the currently assigned language in the system.
		/// 0x08c5d81e
		CvLocalizationAPI Localization::Language* GetLanguage() const;

		//! Assigns a specific language to a string.  Everything built from this string
		//! will thus use this language (including string lookup).
		/// 0x08c5d826
		CvLocalizationAPI bool SetLanguage(Localization::Language* Language);


		// ARGUMENT METHODS
	
		//! Obtain the number of currently pushed arguments.
		/// absent
		CvLocalizationAPI size_t GetArgumentCount() const;

		//! Inserts an argument.
		/// 0x08c5e2f2
		CvLocalizationAPI bool PushArgument(_In_z_ const char* szText);
		/// 0x08c5e3b2
		CvLocalizationAPI bool PushArgument(_In_bytecount_(length) const char* szText, size_t length);
		/// 0x08c5e47e
		CvLocalizationAPI bool PushArgument(const int val);
		/// 0x08c5e540
		CvLocalizationAPI bool PushArgument(const __int64 val);
		/// 0x08c5e5dc
		CvLocalizationAPI bool PushArgument(const float val);
		/// 0x08c5e674
		CvLocalizationAPI bool PushArgument(const double val);
		/// 0x08c5e764
		CvLocalizationAPI bool PushArgument(const String& strText);
		/// 0x08c5e70c
		bool PushArgument(const ParameterArgument& argument);

		/// inline
		template<size_t N>
		bool PushArgument(const char(&val)[N]);

		// FORMS
		//! Obtains the number of forms in a string.
		/// 0x08c5d88a
		CvLocalizationAPI size_t GetNumForms() const;

		//! Obtains the Nth form which matches the specified plurality.
		/// 0x08c5d89e
		CvLocalizationAPI size_t GetFormByPlurality(int iPlurality, int nOffset = 0) const;

		//! Obtains the Nth form which matches a specific gender or noun trait.
		/// 0x08c5d914
		CvLocalizationAPI size_t GetFormByGender(_In_bytecount_(lenGender) const char* szGender, size_t lenGender, int nOffset = 0) const;

		//! Obtains the Nth form which matches a specific plurality AND gender.
		/// 0x08c5da74
		CvLocalizationAPI size_t GetFormByPluralityAndGender(int iPlurality, _In_bytecount_(lenGender) const char* szGender, size_t lenGender, int nOffset = 0) const;

		/// inline
		template<size_t N>
		size_t GetFormByGender(const char(&szGender)[N], int nOffset = 0) const;

		/// inline
		template<size_t N>
		size_t GetFormByPluralityAndGender(int iPlurality, const char(&szGender)[N], int nOffset = 0) const;

		//! Determine if the string contains a specific 1-based index form.
		/// absent
		CvLocalizationAPI bool HasForm(int nForm) const;

		//! Determine if the string contains an Nth form which matches the specified plurality.
		/// absent
		CvLocalizationAPI bool HasFormByPlurality(int iPlurality, int nOffset = 0) const;

		//! Determine if the string contains an Nth form which matches a specified gender
		/// absent
		CvLocalizationAPI bool HasFormByGender(_In_bytecount_(lenGender) const char* szGender, size_t lenGender, int nOffset = 0) const;

		//! Determine if the string contains an Nth form which matches a specified plurality and gender.
		/// absent
		CvLocalizationAPI bool HasFormByPluralityAndGender(int iPlurality, _In_bytecount_(lenGender) const char* szGender, size_t lenGender, int nOffset = 0) const;

		/// inline
		template<size_t N>
		bool HasFormByGender(const char(&szGender)[N], int nOffset = 0) const;

		/// inline
		template<size_t N>
		bool HasFormByPluralityAndGender(int iPlurality, const char(&szGender)[N], int nOffset = 0) const;

		// NOUN TRAITS
		//! Assigns the gender and noun traits to one or many forms of the string.
		/// 0x08c5dbec
		CvLocalizationAPI bool SetGender(_In_bytecount_(lenGender) const char* szGender, size_t lenGender);

		//! Assigns a string-based gender to a specific form of the string.
		/// 0x08c5dd16
		CvLocalizationAPI bool SetGender(size_t nForm, _In_bytecount_(lenGender) const char* szGender, size_t lenGender);

		//! Assigns a single gender value to all forms of the string.
		/// 0x08c5de2c
		CvLocalizationAPI bool SetGender(Gender gender);

		//! Assigns a gender to a specific form.
		/// 0x08c5de70
		CvLocalizationAPI bool SetGender(size_t nForm, Gender gender);

		//! Obtains the gender of a specific form. Returns NULL if invalid params are used.
		/// 0x08c5dec0
		CvLocalizationAPI Gender GetGender(size_t nForm = 1) const;

		/// inline
		template<size_t N>
		bool SetGender(const char(&szGender)[N]);

		// PLURALITY
		//! Assigns the plurality to one or many forms of the string.
		//! NOTE: Plurality verification is not performed until string composition.
		/// 0x08c5df04
		CvLocalizationAPI bool SetPlurality(_In_bytecount_(lenPlurality) const char* szPlurality, size_t lenPlurality);

		//! Sets all forms of the string to a specific plurality.
		/// 0x08c5e15a
		CvLocalizationAPI bool SetPlurality(size_t iPlurality);

		//! Sets the plural form of a specific string form.
		/// 0x08c5e1da
		CvLocalizationAPI bool SetPlurality(size_t nForm, size_t iPlurality);

		//! Obtains the plurality of a specific form.  Returns -1 if invalid params are used.
		/// 0x08c5e266
		CvLocalizationAPI size_t GetPlurality(size_t nForm = 1) const;

		/// inline
		template<size_t N>
		bool SetPlurality(const char(&szPlurality)[N]);

		// BUILDING METHODS
		//! Composes the final string for the specified form.
		//! @return true on success, false on failure.
		/// 0x08c5d7a2
		CvLocalizationAPI bool Compose(size_t nForm = 1);

		//! Parses the string for parameters and other information.
		/// 0x08c5e2aa
		CvLocalizationAPI bool Parse(size_t nForm = 1);

		// STATIC CONSTANTS
		/// 0x0d265aa0
		CvLocalizationAPI static String Empty;
		
	private:
		//! Used for storing the base string.
		SharedString m_BaseString; ///< +0x4
		//! Used to store a null terminated composed version of the first form.
		SharedString m_ComposedNullTerminated; ///< +0x18

		//! Used to store metadata for passed arguments.
		std::vector<ParameterArgument>	m_arguments; ///< +0x2c
		//! Used to store individual form data.
		std::vector<SingleFormString> m_Forms; ///< +0x38

		//! Used to store the explicit language of the string.
		Localization::Language* m_pkLanguage; ///< +0x44
		// seed: in Civ5XP, not in SDK
	public:
		/// 0x08c5ceec
		UNK_RET ParseBaseString();
		/// 0x08c5d2c2
		String(Localization::SingleFormString const& a1);
		/// 0x08c5d838
		UNK_RET GetFormA(unsigned int a1, Localization::SingleFormString& a2);
		/// 0x08c5d9de
		UNK_RET GetFormByGender(Localization::Gender a1, std::vector<Localization::SharedString, std::allocator<Localization::SharedString>> const& a2) const;
		/// 0x08c5db46
		UNK_RET GetFormByPluralityAndGender(int a1, Localization::Gender a2, std::vector<Localization::SharedString, std::allocator<Localization::SharedString>> const& a3) const;
		/// 0x08c5e512
		UNK_RET PushArgument(long a1);
	};


	/// size 0x1
	class StringDictionary
	{
	public:

		/// size 0x1
		class Language
		{
		public:

			// Accessors
			/// 0x08c5fdd4
			CvLocalizationAPI const char* GetType() const;
			/// 0x08c60030
			CvLocalizationAPI const char* GetDisplayName() const;
			/// 0x08c60288
			CvLocalizationAPI int GetPluralityRule() const;
			// seed: in Civ5XP, not in SDK
		public:
			/// 0x08c60134
			Language(char const* a1, char const* a2, int a3);
			/// 0x08c601b0
			Language(Localization::StringDictionary::Language const& a1);
			/// 0x08c6024e
			~Language();
		};

		/// 0x08c5f7c8
		CvLocalizationAPI String Lookup(const char* szTextKey) const;
		/// 0x08c5f826
		CvLocalizationAPI String Lookup(const char* szTextKey, unsigned int lenTextKey) const;

		/// 0x08c5f94e
		CvLocalizationAPI bool TryLookup(const char* szTextKey, unsigned int lenTextKey, String& output) const;

		/// 0x08c5facc
		CvLocalizationAPI String LookupLanguage(const char* szTextKey, unsigned int uiLanguageIndex) const;
		/// 0x08c5f86c
		CvLocalizationAPI String LookupLanguage(const char* szTextKey, unsigned int lenTextKey, unsigned int uiLanguageIndex) const;

		/// 0x08c5f996
		CvLocalizationAPI bool TryLookupLanguage(const char* szTextKey, unsigned int lenTextKey, unsigned int uiLanguageIndex, String& output) const;

		/// 0x08c5fca6
		CvLocalizationAPI bool HasKey(const char* szTextKey, unsigned int lenTextKey) const;
		/// 0x08c5fce4
		CvLocalizationAPI bool HasKey(const char* szTextKey, unsigned int lenTextKey, unsigned int uiLanguageIndex) const;

		/// 0x08c5fde4
		CvLocalizationAPI unsigned int GetCurrentLanguageIndex() const;

		/// 0x08c5fdec
		CvLocalizationAPI bool SetCurrentLanguage(unsigned int idx);
		/// 0x08c5ff74
		CvLocalizationAPI bool SetCurrentLanguage(const char* szLanguageType);

		/// 0x08c5fdc0
		CvLocalizationAPI size_t GetLanguageCount() const;
		/// 0x08c5fff2
		CvLocalizationAPI const char* GetLanguageType(unsigned int idx) const;
		/// 0x08c60010
		CvLocalizationAPI const char* GetLanguageDisplayName(unsigned int idx) const;
		/// 0x08c60044
		CvLocalizationAPI const Language& GetLanguage(unsigned int idx) const;

		/// 0x08c60052
		CvLocalizationAPI void NotifyDatabaseUpdated() const;
		// seed: in Civ5XP, not in SDK
	public:
		/// 0x08c5f35c
		StringDictionary(Database::Connection& a1, char const* a2, bool a3, bool a4);
		/// 0x08c5f486
		UNK_RET EnumerateLanguages(Database::Connection& a1, char const* a2, bool a3);
		/// 0x08c5f728
		~StringDictionary();
		/// 0x08c5f7b4
		UNK_RET CleanLocalCache() const;
		/// 0x08c5fb2a
		UNK_RET TryLookupLanguageUncached(char const* a1, unsigned int a2, unsigned int a3, Localization::String& a4) const;
		/// 0x08c5fe44
		UNK_RET PopulateLocalCache() const;
		/// 0x08c6007a
		UNK_RET LookupLanguageUncached(char const* a1, unsigned int a2, unsigned int a3) const;
	};
}



// << operator helpers.
/// absent
std::ostream& operator << (std::ostream& out, Localization::String& s);
//------------------------------------------------------------------------------
/// inline
template<typename T>
Localization::String& operator << (Localization::String& s, const T& val)
{
	s.PushArgument(val);
	return s;
}

/// inline
template<size_t N>
Localization::String& operator << (Localization::String& s, const char(&val)[N])
{
	s.PushArgument(val, N - 1);
	return s;
}


//------------------------------------------------------------------------------
// Note: inline members
//------------------------------------------------------------------------------
/// inline
template<size_t N>
inline size_t Localization::String::GetFormByGender(const char(&szGender)[N], int nOffset) const
{
	return GetFormByGender(szGender, N - 1, nOffset);
}
//------------------------------------------------------------------------------
/// inline
template<size_t N>
inline size_t Localization::String::GetFormByPluralityAndGender(int iPlurality, const char(&szGender)[N], int nOffset) const
{
	return GetFormByPluralityAndGender(iPlurality, szGender, N - 1, nOffset);
}
//------------------------------------------------------------------------------
/// inline
template<size_t N>
bool Localization::String::HasFormByGender(const char(&szGender)[N], int nOffset) const
{
	return HasFormByGender(szGender, N - 1, nOffset);
}
//------------------------------------------------------------------------------
/// inline
template<size_t N>
bool Localization::String::HasFormByPluralityAndGender(int iPlurality, const char(&szGender)[N], int nOffset) const
{
	return HasFormByPluralityAndGender(iPlurality, szGender, N - 1, nOffset);
}
//------------------------------------------------------------------------------
/// inline
template<size_t N>
inline bool Localization::String::SetGender(const char(&szGender)[N])
{
	return SetGender(szGender, N - 1);
}
//------------------------------------------------------------------------------
/// inline
template<size_t N>
inline bool Localization::String::SetPlurality(const char(&szPlurality)[N])
{
	return SetPlurality(szPlurality, N - 1);
}
//------------------------------------------------------------------------------
/// inline
template<size_t N>
inline bool Localization::String::PushArgument(const char(&val)[N])
{
	return PushArgument(val, N - 1);
}
//------------------------------------------------------------------------------



#endif //_CVLOCALIZATION_H
