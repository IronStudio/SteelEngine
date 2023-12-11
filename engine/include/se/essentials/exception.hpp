#pragma once

#include "../core.hpp"
#include "../origin.hpp"
#include "intTypes.hpp"
#include "string.hpp"



namespace se
{
	/**
	 * @brief Base class of all exceptions of SteelEngine
	*/
	class SE_CORE Exception
	{
		public:
			Exception() = default;
			virtual ~Exception() = default;

			/**
			 * @brief Return the formatted content of the exception
			*/
			virtual const se::UTF8String &what() const noexcept = 0;
	};



	/**
	 * @brief A class that represents a runtime-only error, like an allocation failure
	 * @warning Don't use it to handle invalid argument, instead please use `se::InvalidArgumentError` or `se::Assertion`
	*/
	class SE_CORE RuntimeError final : public se::Exception
	{
		public:
			/**
			 * @brief Constructor of a `se::RuntimeRrror`
			 * @param file The file in which the exception happened, usually `__FILE__`
			 * @param line The line at which the exception happened, usually `__LINE__`
			 * @param func The function in which the exception happened, usually `__func__`
			 * @param message The message to to describe the exception
			 * @param origin The origin of the exception
			 * @warning Please consider using macros like `SE_AppRuntimeError()` instead using directly this constructor
			*/
			RuntimeError(
				const se::UTF8String &file,
				se::Uint line,
				const se::UTF8String &func,
				const se::UTF8String &message,
				se::Origin origin = se::Origin::eUnknown
			);
			RuntimeError() = default;

			/**
			 * @brief Return the formatted content of the exception
			*/
			const se::UTF8String &what() const noexcept;

		
		private:
			se::UTF8String m_string;
	};



	/**
	 * @brief A class that represents invalid argument error, like a negative value given to `sqrt()`
	*/
	class SE_CORE InvalidArgumentError final : public se::Exception
	{
		public:
			/**
			 * @brief Constructor of a `se::RuntimeRrror`
			 * @param file The file in which the exception happened, usually `__FILE__`
			 * @param line The line at which the exception happened, usually `__LINE__`
			 * @param func The function in which the exception happened, usually `__func__`
			 * @param argument The name of the argument that is invalid
			 * @param message The message to to describe the exception
			 * @param origin The origin of the exception
			 * @warning Please consider using macros like `SE_AppInvalidArgumentError()` instead using directly this constructor
			*/
			InvalidArgumentError(
				const se::UTF8String &file,
				se::Uint line,
				const se::UTF8String &func,
				const se::UTF8String &argument,
				const se::UTF8String &message,
				se::Origin origin = se::Origin::eUnknown
			);
			InvalidArgumentError() = default;

			/**
			 * @brief Return the formatted content of the exception
			*/
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

