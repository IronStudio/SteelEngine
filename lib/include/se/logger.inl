#include "se/logger.hpp"

#include <iostream>



namespace se {
	LogSeverityMask::LogSeverityMask() :
		content {0}
	{

	}

	LogSeverityMask::LogSeverityMask(se::LogSeverity severity) :
		content {static_cast<se::Uint8> (severity)}
	{

	}

	const se::LogSeverityMask &LogSeverityMask::operator=(se::LogSeverity severity) {
		content = static_cast<se::Uint8> (severity);
		return *this;
	}

	LogSeverityMask::LogSeverityMask(const se::LogSeverityMask &mask) :
		content {mask.content}
	{

	}

	const se::LogSeverityMask &LogSeverityMask::operator=(const se::LogSeverityMask &mask) {
		content = mask.content;
		return *this;
	}


	se::LogSeverityMask operator|(se::LogSeverityMask lhs, se::LogSeverityMask rhs) {
		se::LogSeverityMask output {};
		output.content = lhs.content | rhs.content;
		return output;
	}

	se::LogSeverityMask operator|(se::LogSeverity severity, se::LogSeverityMask mask) {
		se::LogSeverityMask output {};
		output.content = static_cast<se::Uint8> (severity) | mask.content;
		return output;
	}

	se::LogSeverityMask operator|(se::LogSeverityMask mask, se::LogSeverity severity) {
		se::LogSeverityMask output {};
		output.content = static_cast<se::Uint8> (severity) | mask.content;
		return output;
	}

	se::LogSeverityMask operator|(se::LogSeverity lhs, se::LogSeverity rhs) {
		se::LogSeverityMask output {};
		output.content = static_cast<se::Uint8> (lhs) | static_cast<se::Uint8> (rhs);
		return output;
	}

	se::LogSeverityMask operator&(se::LogSeverityMask lhs, se::LogSeverityMask rhs) {
		se::LogSeverityMask output {};
		output.content = lhs.content & rhs.content;
		return output;
	}

	se::LogSeverityMask operator&(se::LogSeverity severity, se::LogSeverityMask mask) {
		se::LogSeverityMask output {};
		output.content = static_cast<se::Uint8> (severity) & mask.content;
		return output;
	}

	se::LogSeverityMask operator&(se::LogSeverityMask mask, se::LogSeverity severity) {
		se::LogSeverityMask output {};
		output.content = static_cast<se::Uint8> (severity) & mask.content;
		return output;
	}

	se::LogSeverityMask operator&(se::LogSeverity lhs, se::LogSeverity rhs) {
		se::LogSeverityMask output {};
		output.content = static_cast<se::Uint8> (lhs) | static_cast<se::Uint8> (rhs);
		return output;
	}




	Logger::Logger(const std::string &name) :
		m_name {name},
		m_outputStream {&std::clog},
		m_severityMask {
			se::LogSeverity::eError
			| se::LogSeverity::eWarning
			| se::LogSeverity::eInfo
			| se::LogSeverity::eVerbose
			| se::LogSeverity::eFatal
		},
		m_outputMutex {},
		m_buffers {},
		m_logInfos {},
		m_colorMap {
			{se::LogSeverity::eVerbose, se::LogColor::magenta},
			{se::LogSeverity::eInfo,    se::LogColor::brightCyan},
			{se::LogSeverity::eWarning, se::LogColor::yellow},
			{se::LogSeverity::eError,   se::LogColor::red},
			{se::LogSeverity::eFatal,   se::LogColor::brightRed}
		},
		m_terminalColor {se::LogColor::none}
	{

	}

	Logger::~Logger() {

	}

	void Logger::setOutputStream(std::ostream *outputStream) {
		m_outputStream = outputStream;
	}

	void Logger::setSeverityMask(se::LogSeverityMask severityMask) {
		m_severityMask = severityMask;
	}

	void Logger::setColorMap(const std::map<se::LogSeverity, std::string> &colorMap) {
		m_colorMap = colorMap;
	}

	void Logger::setTerminalColor(const std::string &color) {
		m_terminalColor = color;
	}

	template <typename ...Args>
	void Logger::log(const se::LogInfos &infos, std::string message, Args ...args) {
		static const std::map<se::LogSeverity, std::string> severityToStringMap {
			{se::LogSeverity::eVerbose, "verbose"},
			{se::LogSeverity::eInfo,    "info"},
			{se::LogSeverity::eWarning, "warning"},
			{se::LogSeverity::eError,   "error"},
			{se::LogSeverity::eFatal,   "fatal"}
		};

		if (!(infos.severity & m_severityMask))
			return;

		if (sizeof...(Args) != 0)
			message = std::vformat(message, std::make_format_args(args...));

		std::string defaultColor {m_terminalColor};
		std::string severityColor {m_colorMap[infos.severity]};
		if (infos.severity == se::LogSeverity::eFatal)
			defaultColor = severityColor;

		m_outputMutex.lock();
		*m_outputStream << defaultColor << m_name << " [" << severityColor << severityToStringMap.find(infos.severity)->second
			<< defaultColor << "] > " << message << se::LogColor::reset << std::endl;
		m_outputMutex.unlock();
	}

	template <typename T>
	se::Logger &Logger::operator<<(const T &element) {
		if constexpr (std::is_same_v<T, se::LogInfos>) {
			m_logInfos[std::this_thread::get_id()] = element;
		}

		else if constexpr (std::is_same_v<T, se::LogEnd>) {
			this->log(m_logInfos[std::this_thread::get_id()], m_buffers[std::this_thread::get_id()].str());
			m_buffers[std::this_thread::get_id()].str("");
			m_buffers[std::this_thread::get_id()].clear();
		}

		else {
			m_buffers[std::this_thread::get_id()] << element;
		}

		return *this;
	}


} // namespace se
