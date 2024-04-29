#pragma once

#include <ostream>

#include "se/types.hpp"



namespace se
{
	/**
	 * @brief Represent a unique identifier. The UUID is generated by taking the 48 first bits of the timestamp in μs, and generating a random 8 bits number to complete the UUID. Collision should be avoided, but they may occure (though they are really unlikly).
	 *        The UUID can be represented as a string with format "0000-0000-0000-0000" where 0's are to be replaced by the UUID in hex.
	 */
	class UUID {
		public:
			/**
			 * @brief Generate a new UUID
			 */
			UUID();
			~UUID() = default;

			/**
			 * @brief Use a user-specified uuid number
			 * @warning A newly-generated UUID can clash with your UUID, or your's can clash with one already generated
			 */
			UUID(se::Uint64 uuid);
			/**
			 * @brief Use a user-specified uuid string
			 * @warning A newly-generated UUID can clash with your UUID, or your's can clash with one already generated. No checks are made on the validity of the string. Passing an invalid UUID is undefined behavior
			 */
			UUID(const std::string &uuid);

			/**
			 * @brief Copy a UUID
			 */
			UUID(const se::UUID &uuid);
			/**
			 * @brief Copy a UUID
			 */
			const se::UUID &operator=(const se::UUID &uuid);

			/**
			 * @brief Get the internal UUID code
			 */
			explicit operator se::Uint64() const;
			/**
			 * @brief Get the internal UUID code as a string
			 */
			explicit operator std::string() const;

		private:
			se::Uint64 m_uuid;
	};



	/**
	 * @brief Log a UUID's internal code
	 * @warning If you only need the code, please use `stream << static_cast<std::string> (uuid)` instead because of formatting artefact
	 */
	std::ostream &operator<<(std::ostream &stream, const se::UUID uuid);



} // namespace se
