#pragma once

#include "../origin.hpp"
#include "string.hpp"



namespace se
{
	class Exception
	{
		public:
			Exception() = default;
			virtual ~Exception() = default;

			virtual const se::String &what() const noexcept = 0;
	};



	class RuntimeError final : public se::Exception
	{
		public:
			RuntimeError(
				const se::String &file,
				int line,
				const se::String &func,
				const se::String &message,
				se::Origin origin = se::Origin::eUnknown
			);
			RuntimeError() = default;

			const se::String &what() const noexcept;

		
		private:
			se::String m_string;
	};



	class InvalidArgumentError final : public se::Exception
	{
		public:
			InvalidArgumentError(
				const se::String &file,
				int line,
				const se::String &func,
				const se::String &argument,
				const se::String &message,
				se::Origin origin = se::Origin::eUnknown
			);
			InvalidArgumentError() = default;

			const se::String &what() const noexcept;

		
		private:
			se::String m_string;
	};

} // namespace se


#define SE_CoreRuntimeError(message) se::RuntimeError(\
	__FILE__, __LINE__, __func__, message, se::Origin::eEngine)
#define SE_CoreInvalidArgumentError(argument, message) se::InvalidArgumentError(\
	__FILE__, __LINE__, __func__, #argument, message, se::Origin::eEngine)


#define SE_AppRuntimeError(message) se::RuntimeError(\
	__FILE__, __LINE__, __func__, message, se::Origin::eApp)
#define SE_AppInvalidArgumentError(argument, message) se::InvalidArgumentError(\
	__FILE__, __LINE__, __func__, #argument, message, se::Origin::eApp)


#define SE_UnknownRuntimeError(message) se::RuntimeError(\
	__FILE__, __LINE__, __func__, message, se::Origin::eUnknown)
#define SE_UnknownInvalidArgumentError(argument, message) se::InvalidArgumentError(\
	__FILE__, __LINE__, __func__, #argument, message, se::Origin::eUnknown)

