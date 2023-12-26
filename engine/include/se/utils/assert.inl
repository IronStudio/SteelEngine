#include "assert.hpp"



namespace se
{
	Assert::Assert(const std::string &message) :
		m_message {message}
	{

	}



	const std::string &Assert::getMessage() const noexcept
	{
		return m_message;
	}



} // namespace se
