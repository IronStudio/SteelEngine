#include "se/exceptions.hpp"



namespace se::exceptions
{
	#define SE_CREATE_EXCEPTION_TYPE(name) std::string name::what() const noexcept {\
		return #name " > " + m_content;\
	}



	Exception::Exception(const std::string &file, const std::string &function, int line, const std::string &message) :
		m_content {"in file " + file + ":" + std::to_string(line) + " in function '" + function + "' > " + message}
	{

	}



	SE_CREATE_EXCEPTION_TYPE(RuntimeError)
	SE_CREATE_EXCEPTION_TYPE(InvalidArgument)
	SE_CREATE_EXCEPTION_TYPE(BadAllocation)



} // namespace se::exceptions
