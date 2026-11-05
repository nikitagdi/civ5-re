/// origin guess

#pragma once

namespace Database
{
	/// size 0x4
	/// vtable ?
	class IDatabaseLogger
	{
	public:

		enum LogTypes
		{
			TYPE_MESSAGE = 0,
			TYPE_WARNING = 1,
			TYPE_ERROR = 2
		};

		//This is necessary in order for concrete classes to correctly override new and delete.
		/// inline
		virtual ~IDatabaseLogger(){};

		/// pure
		virtual void Log(const char* szMessage, LogTypes eType = TYPE_MESSAGE) = 0;
	};

}
