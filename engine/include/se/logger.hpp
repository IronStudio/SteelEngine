#pragma once

#include "designPattern/singleton.hpp"
#include "essentials/string.hpp"
#include "origin.hpp"
#include "core.hpp"



namespace se
{
	/**
	 * @brief A structure that indicates the start of a log and contains its metadatas
	*/
	struct LogStart
	{

	};



	struct LogEnd {};
	/**
	 * @brief Indicate the end of a log, like `std::cout` indicates the end of a line, but it **doesn't** flush the logger
	*/
	inline se::LogEnd endLog {};



	/**
	 * @brief A singleton class that handle logging capabilities
	 * @tparam origin The origin of the logger. There is one instance of the logger per origin
	*/
	template <se::Origin origin>
	class SE_CORE Logger final : public se::designPattern::Singleton<se::Logger<origin>>
	{
		friend class se::designPattern::Singleton<se::Logger<origin>>;

		public:
			/**
			 * @brief Ouput a C-style formatted log
			*/
			void format(const se::LogStart &start, const se::UTF8String &format, ...);
			/**
			 * @brief Flush the buffer, that is synchronise changes to the underlying logging facilities (like a file)
			 * @warning This operation can have high costs. Please minimise flushing to, for example, the start of a 1/4 frames
			*/
			void flush();

			se::Logger<origin> &operator<<(const se::LogStart &start);
			se::Logger<origin> &operator<<(const se::LogEnd &);

			/**
			 * @brief A C++ style way to log informations. Log **must** starts with `se::LogStart` and end with `se::endLog`
			*/
			template <typename T>
			se::Logger<origin> &operator<<(const T &data);

			/**
			 * @brief Set the underlying stream. This must be set before attempting logging
			*/
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