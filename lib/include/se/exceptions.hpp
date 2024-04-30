#pragma once

#include <string>

#include "se/core.hpp"



namespace se::exceptions
{
	#define SE_CREATE_EXCEPTION_TYPE(name) class SE_CORE name final : public se::exceptions::Exception {\
		public:\
			using se::exceptions::Exception::Exception;\
			~name() override = default;\
			std::string what() const noexcept override;\
	};


	class SE_CORE Exception {
		public:
			Exception(const std::string &file, const std::string &function, int line, const std::string &message);
			virtual ~Exception() = default;

			virtual std::string what() const noexcept = 0;
		
		protected:
			std::string m_content;
	};



	SE_CREATE_EXCEPTION_TYPE(RuntimeError)
	SE_CREATE_EXCEPTION_TYPE(InvalidArgument)
	SE_CREATE_EXCEPTION_TYPE(BadAllocation)

	#define RuntimeError(msg) RuntimeError(__FILE__, __func__, __LINE__, msg)
	#define InvalidArgument(msg) InvalidArgument(__FILE__, __func__, __LINE__, msg)
	#define BadAllocation(msg) BadAllocation(__FILE__, __func__, __LINE__, msg)


	#undef SE_CREATE_EXCEPTION_TYPE
} // namespace se::exceptions
