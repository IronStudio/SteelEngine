#pragma once

#include "../core.hpp"
#include "../origin.hpp"
#include "string.hpp"



namespace se
{
	class SE_CORE Assertion final
	{
		public:
			Assertion(
				const se::UTF8String &file,
				int line,
				const se::UTF8String &func,
				const se::UTF8String &msg,
				se::Origin origin = se::Origin::eUnknown
			);
			~Assertion() = default;

			inline const char *what() const noexcept;


		private:
			se::UTF8String m_msg;
	};

} // namespace se



#ifndef NDEBUG
	#define SE_ASSERT(cond, msg, origin) if (!(cond)) throw se::Assertion(__FILE__, __LINE__, __func__, se::UTF8String(#cond) + " | " + msg, origin)
#else
	#define SE_ASSERT(cond, msg, origin)
#endif
#define SE_CORE_ASSERT(cond, msg) SE_ASSERT(cond, msg, se::Origin::eEngine)
#define SE_APP_ASSERT(cond, msg) SE_ASSERT(cond, msg, se::Origin::eApp)
#define SE_UNKNOWN_ASSERT(cond, msg) SE_ASSERT(cond, msg, se::Origin::eUnknown)


#include "assertion.inl"