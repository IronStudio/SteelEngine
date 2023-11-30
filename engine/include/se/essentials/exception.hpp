#pragma once

#include "string.hpp"



namespace se::essentials
{
	enum class ExceptionOrigin
	{
		eEngine,
		eApp,
		eUnknown
	};



	class Exception
	{
		public:
			Exception() = default;
			virtual ~Exception() = default;

			virtual const se::essentials::String &what() const noexcept = 0;
	};



	class RuntimeError final : public se::essentials::Exception
	{
		public:
			RuntimeError(
				const se::essentials::String &file,
				int line,
				const se::essentials::String &func,
				const se::essentials::String &message,
				se::essentials::ExceptionOrigin origin = se::essentials::ExceptionOrigin::eUnknown
			);
			RuntimeError() = default;

			const se::essentials::String &what() const noexcept;

		
		private:
			se::essentials::String m_string;
	};



	class InvalidArgumentError final : public se::essentials::Exception
	{
		public:
			InvalidArgumentError(
				const se::essentials::String &file,
				int line,
				const se::essentials::String &func,
				const se::essentials::String &argument,
				const se::essentials::String &message,
				se::essentials::ExceptionOrigin origin = se::essentials::ExceptionOrigin::eUnknown
			);
			InvalidArgumentError() = default;

			const se::essentials::String &what() const noexcept;

		
		private:
			se::essentials::String m_string;
	};

} // namespace se::essentials


#define SE_CoreRuntimeError(message) se::essentials::RuntimeError(\
	__FILE__, __LINE__, __func__, message, se::essentials::ExceptionOrigin::eEngine)
#define SE_CoreInvalidArgumentError(argument, message) se::essentials::InvalidArgumentError(\
	__FILE__, __LINE__, __func__, #argument, message, se::essentials::ExceptionOrigin::eEngine)


#define SE_AppRuntimeError(message) se::essentials::RuntimeError(\
	__FILE__, __LINE__, __func__, message, se::essentials::ExceptionOrigin::eApp)
#define SE_AppInvalidArgumentError(argument, message) se::essentials::InvalidArgumentError(\
	__FILE__, __LINE__, __func__, #argument, message, se::essentials::ExceptionOrigin::eApp)


#define SE_UnknownRuntimeError(message) se::essentials::RuntimeError(\
	__FILE__, __LINE__, __func__, message, se::essentials::ExceptionOrigin::eUnknown)
#define SE_UnknownInvalidArgumentError(argument, message) se::essentials::InvalidArgumentError(\
	__FILE__, __LINE__, __func__, #argument, message, se::essentials::ExceptionOrigin::eUnknown)
