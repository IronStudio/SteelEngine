#pragma once

#include "designPattern/singleton.hpp"
#include "essentials/string.hpp"
#include "origin.hpp"
#include "core.hpp"



namespace se
{
	struct LogStart
	{

	};



	struct LogEnd {};
	inline se::LogEnd endLog {};



	template <se::Origin origin>
	class SE_CORE Logger final : public se::designPattern::Singleton<se::Logger<origin>>
	{
		friend class se::designPattern::Singleton<se::Logger<origin>>;

		public:
			void format(const se::LogStart &start, const se::UTF8String &format, ...);
			void flush();

			se::Logger<origin> &operator<<(const se::LogStart &start);
			se::Logger<origin> &operator<<(const se::LogEnd &);

			template <typename T>
			se::Logger<origin> &operator<<(const T &data);

			inline void setStream(std::ostream *stream);


		protected:
			Logger();
			~Logger();


		private:
			std::ostream *m_stream;

			Logger(const se::Logger<origin> &) = delete;
			const se::Logger<origin> &operator=(const se::Logger<origin> &) = delete;
			Logger(se::Logger<origin> &&) noexcept = delete;
			const se::Logger<origin> &operator=(se::Logger<origin> &&) noexcept = delete;
	};

} // namespace se


#define SE_CoreLogger se::Logger<se::Origin::eEngine>::get()
#define SE_AppLogger se::Logger<se::Origin::eApp>::get()
#define SE_UnknownLogger se::Logger<se::Origin::eUnknown>::get()


#include "logger.inl"