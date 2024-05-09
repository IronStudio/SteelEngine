#pragma once

#include <format>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "se/core.hpp"
#include "se/types.hpp"
#include "se/utils/bitField.hpp"



namespace se {
	SE_CREATE_BIT_FIELD(LogSeverity, LogSeverityMask,
		eVerbose = 0b0000'0001,
		eInfo    = 0b0000'0010,
		eWarning = 0b0000'0100,
		eError   = 0b0000'1000,
		eFatal   = 0b0001'0000
	)


	inline se::LogSeverityMask operator|(se::LogSeverityMask lhs, se::LogSeverityMask rhs);
	inline se::LogSeverityMask operator|(se::LogSeverity severity, se::LogSeverityMask mask);
	inline se::LogSeverityMask operator|(se::LogSeverityMask mask, se::LogSeverity severity);
	inline se::LogSeverityMask operator|(se::LogSeverity lhs, se::LogSeverity rhs);

	inline se::LogSeverityMask operator&(se::LogSeverityMask lhs, se::LogSeverityMask rhs);
	inline se::LogSeverityMask operator&(se::LogSeverity severity, se::LogSeverityMask mask);
	inline se::LogSeverityMask operator&(se::LogSeverityMask mask, se::LogSeverity severity);
	inline se::LogSeverityMask operator&(se::LogSeverity lhs, se::LogSeverity rhs);


	struct LogColor {
		static constexpr std::string red           {"\033[31m"};
		static constexpr std::string green         {"\033[32m"};
		static constexpr std::string yellow        {"\033[33m"};
		static constexpr std::string blue          {"\033[34m"};
		static constexpr std::string magenta       {"\033[35m"};
		static constexpr std::string cyan          {"\033[36m"};
		static constexpr std::string white         {"\033[37m"};
		static constexpr std::string gray          {"\033[90m"};
		static constexpr std::string brightRed     {"\033[91m"};
		static constexpr std::string brightGreen   {"\033[92m"};
		static constexpr std::string brightYellow  {"\033[93m"};
		static constexpr std::string brightBlue    {"\033[94m"};
		static constexpr std::string brightMagenta {"\033[95m"};
		static constexpr std::string brightCyan    {"\033[96m"};
		static constexpr std::string brightWhite   {"\033[97m"};
		static constexpr std::string none          {""};
		static constexpr std::string reset         {"\033[0m"};
	};


	struct LogInfos {
		LogSeverity severity;
	};

	class SE_CORE Logger {
		public:
			inline Logger(const std::string &name);
			inline ~Logger();

			inline void setOutputStream(std::ostream *outputStream);
			inline void setSeverityMask(se::LogSeverityMask severityMask);
			inline void setColorMap(const std::map<se::LogSeverity, std::string> &colorMap);
			inline void setTerminalColor(const std::string &color);

			template <typename ...Args>
			void log(const se::LogInfos &infos, std::string message, Args ...args);

			template <typename T>
			se::Logger &operator<<(const T &element);

		private:
			std::string m_name;
			std::ostream *m_outputStream;
			se::LogSeverityMask m_severityMask;
			std::mutex m_outputMutex;
			std::map<std::thread::id, std::ostringstream> m_buffers;
			std::map<std::thread::id, se::LogInfos> m_logInfos;
			std::map<se::LogSeverity, std::string> m_colorMap;
			std::string m_terminalColor;
	};

	struct LogEnd {};
	static se::LogEnd endLog {};

	SE_CORE extern se::Logger engineLogger;
	SE_CORE extern se::Logger applicationLogger;

} // namespace se


#define SE_LOGGER ::se::engineLogger
#define SE_APP_LOGGER ::se::applicationLogger


#include "se/logger.inl"