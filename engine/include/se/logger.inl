#include "essentials/assertion.hpp"
#include "logger.hpp"



namespace se
{
	template <se::Origin origin>
	template <typename T>
	se::Logger<origin> &Logger<origin>::operator<<(const T &data)
	{
		SE_ASSERT(m_stream != nullptr, "Can't log if no stream were specified", origin);
		*m_stream << data;
		
		return *this;
	}



	template <se::Origin origin>
	inline void Logger<origin>::setStream(std::ostream *stream)
	{
		SE_ASSERT(stream != nullptr, "Can't set logger stream to nullptr", origin);
		m_stream = stream;
	}



} // namespace se
