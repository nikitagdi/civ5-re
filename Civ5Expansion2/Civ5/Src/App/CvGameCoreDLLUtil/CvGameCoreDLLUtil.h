/// origin binary
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//!	 \file		CvGameCoreDLLUtil.h
//!  \brief     Public header of the CvGameCoreDLLUtil library.
//!
//!		This library includes classes utilized by CvGameCoreDLL but not fully
//!		published in the source code.
//!
//!  Key Classes:
//!	 - CvAssert*	- Civilization Assert Macros
//!	 - CvDataStream - Generic data stream
//!  - CvString		- Generic string class
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef CvGameCoreDLLUtil_h
#define CvGameCoreDLLUtil_h
#pragma once

#include <limits>

using namespace std;

// JAR : Initialize your variables, don't desync an MP game and make me cry
#pragma warning ( 1 : 4701) 

// JAR : and if you ignore the warning, I make you cry
#pragma warning ( error : 4701 )

#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDLLUtilDefines.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvStructs.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvAssert.h"


#endif	// CvGameCoreDLLUtil_h
