#pragma once

#include "../core.hpp"
#include "../origin.hpp"
#include "intTypes.hpp"
#include "string.hpp"



namespace se
{
	/**
	 * @brief A class to throw and handle assertation in a memory-safe way
	*/
	class SE_CORE Assertion final
	{
		public:
			/**
			 * @brief Use with throw to stop the program
			 * @param file The file in which the assert happened, usually `__FILE__`
			 * @param line The line at which the assert happened, usually `__LINE__`
			 * @param func The function in which the assert happened, usually `__func__`
			 * @param msg The message to to describe the assert
			 * @param origin The origin of the assert
			 * @warning Please consider using macros like `SE_APP_ASSERT()` instead using directly this constructor
			*/
			Assertion(
				const se::UTF8String &file,
				se::Uint line,
				const se::UTF8String &func,
				const se::UTF8String &msg,
				se::Origin origin = se::Origin::eUnknown
			);
			~Assertion() = default;

			/**
			 * @brief Return the formatted content of the assert
			*/
			inline const char *what() const noexcept;
			inline se::Origin getOrigin() const noexcept;


		private:
			se::UTF8String m_msg;
			se::Origin m_origin;
	};

} // namespace se



#ifndef NDEBUG
	#define SE_ASSERT(cond, msg, origin) if (!(cond)) throw ::se::Assertion(__FILE__, __LINE__, __func__, se::UTF8String(#cond) + " | " + msg, origin)
#else
	#define SE_ASSERT(cond, msg, origin)
#endif
#define SE_CORE_ASSERT(cond, msg) SE_ASSERT(cond, msg, se::Origin::eEngine)
#define SE_APP_ASSERT(cond, msg) SE_ASSERT(cond, msg, se::Origin::eApp)
#define SE_UNKNOWN_ASSERT(cond, msg) SE_ASSERT(cond, msg, se::Origin::eUnknown)


#include "assertion.inl"