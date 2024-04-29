#pragma once

#include <ostream>

#include "se/types.hpp"



namespace se
{
	class UUID {
		public:
			UUID();
			~UUID() = default;

			UUID(se::Uint64 uuid);
			UUID(const std::string &uuid);

			UUID(const se::UUID &uuid);
			const se::UUID &operator=(const se::UUID &uuid);

			explicit operator se::Uint64() const;
			explicit operator std::string() const;

		private:
			se::Uint64 m_uuid;
	};


	
	std::ostream &operator<<(std::ostream &stream, const se::UUID uuid);



} // namespace se
