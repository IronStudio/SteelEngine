#include "se/uuid.hpp"

#include <chrono>
#include <iomanip>
#include <random>
#include <sstream>



namespace se
{
	UUID::UUID() :
		m_uuid {}
	{
		constexpr se::Uint64 timestampFilter {0x0000'ffff'ffff'ffff};
		constexpr se::Uint64 randomNumberBitShift {48};

		se::Uint64 currentTime {std::chrono::duration_cast<std::chrono::duration<se::Uint64, std::micro>> (
			std::chrono::high_resolution_clock::now().time_since_epoch()
		).count()};
		se::Uint64 randomNumber {static_cast<se::Uint64> ((static_cast<se::Float> (rand()) / static_cast<se::Float> (RAND_MAX)) * 8.f)};

		m_uuid = (currentTime & timestampFilter) | (randomNumber << randomNumberBitShift);
	}



	UUID::UUID(se::Uint64 uuid) :
		m_uuid {uuid}
	{

	}



	UUID::UUID(const std::string &uuid) :
		m_uuid {}
	{
		for (se::Size i {0}; i < 4; ++i) {
			std::string numberPart {uuid, i * 5, 4};
			se::Uint64 res {std::stoull(numberPart, nullptr, 16)};
			m_uuid |= res << ((3 - i) * 16);
		}
	}



	UUID::UUID(const se::UUID &uuid) :
		m_uuid {uuid.m_uuid}
	{

	}



	const se::UUID &UUID::operator=(const se::UUID &uuid) {
		m_uuid = uuid.m_uuid;
		return *this;
	}



	UUID::operator se::Uint64() const {
		return m_uuid;
	}



	UUID::operator std::string() const {
		std::ostringstream stream {};
		stream << std::hex << std::setfill('0') << std::setw(16) << m_uuid;
		std::string str {stream.str()};
		str.insert(str.begin() + 12, '-');
		str.insert(str.begin() + 8, '-');
		str.insert(str.begin() + 4, '-');
		return str;
	}



	bool UUID::operator==(const se::UUID &uuid) const {
		return m_uuid == uuid.m_uuid;
	}



	UUID::operator bool() const {
		return m_uuid != 0;
	}



	std::ostream &operator<<(std::ostream &stream, const se::UUID uuid) {
		stream << "<" << static_cast<std::string> (uuid) << ">";
		return stream;
	}



} // namespace se
