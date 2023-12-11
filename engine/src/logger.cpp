#include <cstdarg>
#include <ctime>

#include "essentials/exception.hpp"
#include "logger.hpp"



namespace se
{
	template <se::Origin origin>
	Logger<origin>::Logger() :
		m_stream {nullptr}
	{

	}


	
	template <se::Origin origin>
	Logger<origin>::~Logger()
	{
		m_stream->flush();
	}



	template <se::Origin origin>
	void Logger<origin>::format(const se::LogStart &start, const se::UTF8String &format, ...)
	{
		va_list args;
		va_start(args, format);

		int neededSize {vsnprintf(nullptr, 0, format.getCString(), args)};
		SE_ASSERT(neededSize > 0, "Can't format given log stream", origin);
		char *buffer {new char[neededSize + 1]};
		if (buffer == nullptr)
			throw SE_RuntimeError("Can't allocate memory for log format buffer", origin);

		vsnprintf(buffer, neededSize + 1, format.getCString(), args);
		*this << start << buffer << se::endLog;

		delete[] buffer;
		va_end(args);
	}



	template <se::Origin origin>
	void Logger<origin>::flush()
	{
		m_stream->flush();
	}



	template <se::Origin origin>
	se::Logger<origin> &Logger<origin>::operator<<(const se::LogStart &start)
	{
		SE_ASSERT(m_stream != nullptr, "Can't start log because stream is nullptr", origin);
		static se::Length count {0};
		*m_stream << count << " [" << se::convertOriginToString(origin) << "] at "
			<< static_cast<float> (clock()) / static_cast<float> (CLOCKS_PER_SEC) << " seconds > ";
		++count;
		return *this;
	}



	template <se::Origin origin>
	se::Logger<origin> &Logger<origin>::operator<<(const se::LogEnd &)
	{
		SE_ASSERT(m_stream != nullptr, "Can't end log because stream is nullptr", origin);
		*m_stream << '\n';
		return *this;
	}



} // namespace se



template class se::Logger<se::Origin::eUnknown>;
template class se::Logger<se::Origin::eApp>;
template class se::Logger<se::Origin::eEngine>;