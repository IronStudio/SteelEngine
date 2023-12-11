#pragma once

#include "../core.hpp"
#include "../origin.hpp"
#include "intTypes.hpp"
#include "string.hpp"



namespace se
{
	class SE_CORE Exception
	{
		public:
			Exception() = default;
			virtual ~Exception() = default;

			virtual const se::UTF8String &what() const noexcept = 0;
	};



	class SE_CORE RuntimeError final : public se::Exception
	{
		public:
			RuntimeError(
				const se::UTF8String &file,
				se::Uint line,
				const se::UTF8String &func,
				const se::UTF8String &message,
				se::Origin origin = se::Origin::eUnknown
			);
			RuntimeError() = default;

			const se::UTF8String &what() const noexcept;

		
		private:
			se::UTF8String m_string;
	};



	class SE_CORE InvalidArgumentError final : public se::Exception
	{
		public:
			InvalidArgumentError(
				const se::UTF8String &file,
				se::Uint line,
				const se::UTF8String &func,
				const se::UTF8String &argument,
				const se::UTF8String &message,
				se::Origin origin = se::Origin::eUnknown
			);
			InvalidArgumentError() = default;

			const se::UTF8String &what() const noexcept;

		
		private:
			se::UTF8String m_string;
	};

} // namespace se


#define SE_RuntimeError(message, origin) se::RuntimeError(\
	__FILE__, __LINE__, __func__, message, origin)
#define SE_InvalidArgumentError(argument, message, origin) se::InvalidArgumentError(\
	__FILE__, __LINE__, __func__, #argument, message, origin)


#define SE_CoreRuntimeError(message) SE_RuntimeError(message, se::Origin::eEngine)
#define SE_CoreInvalidArgumentError(argument, message) SE_InvalidArgumentError(argument, message, origin)

#define SE_AppRuntimeError(message) SE_RuntimeError(message, se::Origin::eApp)
#define SE_AppInvalidArgumentError(argument, message) SE_InvalidArgumentError(argument, message, origin)

#define SE_UnknownRuntimeError(message) SE_RuntimeError(message, se::Origin::eUnknown)
#define SE_UnknownInvalidArgumentError(argument, message) SE_InvalidArgumentError(argument, message, origin)

