#include "se/utils/string.hpp"

#include <bitset>
#include <cstdlib>
#include <cstring>

#include "se/assert.hpp"

#define UTF8 se::StringEncoding::eUtf8
#define UTF16 se::StringEncoding::eUtf16



namespace se {
	template <>
	template <>
	se::String<UTF8> String<UTF8>::useEncoding<UTF8> () const noexcept {
		return *this;
	}


	template <>
	template <>
	se::String<UTF16> String<UTF16>::useEncoding<UTF16> () const noexcept {
		return *this;
	}


	template <typename T>
	struct Buffer {
		T *buffer {nullptr};
		~Buffer() {
			if (buffer != nullptr)
				free(buffer);
		}
	};


	template <>
	template <>
	se::String<UTF8> String<UTF16>::useEncoding<UTF8> () const noexcept {
		using UTF8Char = se::EncodingChar<UTF8>::Char;

		if (m_data == nullptr || m_size == 0)
			return se::String<UTF8> ();

		Buffer<UTF8Char> buffer {reinterpret_cast<UTF8Char*> (malloc(3 * m_size))};

		se::Count outputStringSize {0};

		for (const _Char *it {m_data}; *it != '\0'; ++it) {
			se::Uint32 code {};

			if ((*it & 0b1101'1000'0000'0000) == 0b1101'1000'0000'0000) {
				se::Uint16 firstPart {*it};
				se::Uint16 secondPart {*(++it)};

				SE_SLOW_ASSERT((secondPart & 0b1101'1100'0000'0000) == 0b1101'1100'0000'0000, "Invalid UTF16 character");

				se::Uint32 wwww {((firstPart & (se::Uint32)0b0000'0011'0000'0000) >> 6) + ((firstPart & (se::Uint32)0b0000'0000'1100'0000) >> 6)};
				se::Uint32 uuuuu {wwww + 1};

				se::Uint32 xxxxxx {firstPart & (se::Uint32)0b0000'0000'0011'1111};
				se::Uint32 yyyyyyyyyy {secondPart & (se::Uint32)0b0000'0011'1111'1111};

				code = yyyyyyyyyy + (xxxxxx << 10) + (uuuuu << 16);
			}

			else
				code = *it;


			if (code <= 0x7F) {
				UTF8Char character {(UTF8Char)(code & 0x00'00'00'7F)};
				buffer.buffer[outputStringSize] = character;
				++outputStringSize;
				continue;
			}

			if (code <= 0x7FF) {
				UTF8Char part1 {(UTF8Char)((code >> 6) & 0b0001'1111 | 0b1100'0000)};
				UTF8Char part2 {(UTF8Char)(code & 0b0011'1111 | 0b1000'0000)};

				buffer.buffer[outputStringSize] = part1;
				buffer.buffer[++outputStringSize] = part2;
				++outputStringSize;
				continue;
			}

			if (code <= 0xFFFF) {
				UTF8Char part1 {(UTF8Char)((code >> 12) & 0b0000'1111 | 0b1110'0000)};
				UTF8Char part2 {(UTF8Char)((code >> 6) & 0b0011'1111 | 0b1000'0000)};
				UTF8Char part3 {(UTF8Char)(code & 0b0011'1111 | 0b1000'0000)};
				buffer.buffer[outputStringSize] = part1;
				buffer.buffer[++outputStringSize] = part2;
				buffer.buffer[++outputStringSize] = part3;
				++outputStringSize;
				continue;
			}

			UTF8Char part1 {(UTF8Char)((code >> 18) & 0b0000'0111 | 0b1111'0000)};
			UTF8Char part2 {(UTF8Char)((code >> 12) & 0b0011'1111 | 0b1000'0000)};
			UTF8Char part3 {(UTF8Char)((code >> 6) & 0b0011'1111 | 0b1000'0000)};
			UTF8Char part4 {(UTF8Char)(code & 0b0011'1111 | 0b1000'0000)};
			buffer.buffer[outputStringSize] = part1;
			buffer.buffer[++outputStringSize] = part2;
			buffer.buffer[++outputStringSize] = part3;
			buffer.buffer[++outputStringSize] = part4;
			++outputStringSize;
		}

		return se::String<UTF8> (buffer.buffer, outputStringSize);
	}


	template <>
	template <>
	se::String<UTF16> String<UTF8>::useEncoding<UTF16> () const noexcept {
		using UTF16Char = se::EncodingChar<UTF16>::Char;

		if (m_data == nullptr || m_size == 0)
			return se::String<UTF16> ();

		Buffer<UTF16Char> buffer {reinterpret_cast<UTF16Char*> (malloc(3 * m_size))};

		se::Count outputStringSize {0};

		for (const _Char *it {m_data}; *it != '\0'; ++it) {
			se::Uint32 code {};

			if ((*it & 0b1000'0000) == 0b0000'0000)
				code = *it;

			else if ((*it & 0b1110'0000) == 0b1100'0000) {
				se::Uint8 firstPart {(se::Uint8)*it};
				se::Uint8 secondPart {(se::Uint8)*(++it)};

				SE_SLOW_ASSERT((secondPart & 0b1100'0000) == 0b1000'0000, "Invalid UTF8 character");

				code = ((firstPart & 0b0001'1111) << 6) | (secondPart & 0b0011'1111);
			}

			else if ((*it & 0b1111'0000) == 0b1110'0000) {
				se::Uint8 firstPart {(se::Uint8)*it};
				se::Uint8 secondPart {(se::Uint8)*(++it)};
				se::Uint8 thirdPart {(se::Uint8)*(++it)};

				SE_SLOW_ASSERT((secondPart & 0b1100'0000) == 0b1000'0000, "Invalid UTF8 character");
				SE_SLOW_ASSERT((thirdPart  & 0b1100'0000) == 0b1000'0000, "Invalid UTF8 character");

				code = ((firstPart & 0b0001'1111) << 12) | ((secondPart & 0b0011'1111) << 6) | (thirdPart & 0b0011'1111);
			}

			else if ((*it & 0b1111'1000) == 0b1111'0000) {
				se::Uint8 firstPart {(se::Uint8)*it};
				se::Uint8 secondPart {(se::Uint8)*(++it)};
				se::Uint8 thirdPart {(se::Uint8)*(++it)};
				se::Uint fourthPart {(se::Uint8)*(++it)};

				SE_SLOW_ASSERT((secondPart & 0b1100'0000) == 0b1000'0000, "Invalid UTF8 character");
				SE_SLOW_ASSERT((thirdPart  & 0b1100'0000) == 0b1000'0000, "Invalid UTF8 character");
				SE_SLOW_ASSERT((fourthPart & 0b1100'0000) == 0b1000'0000, "Invalid UTF8 character");

				code = ((firstPart & 0b0001'1111) << 18) | ((secondPart & 0b0011'1111) << 12)
					 | ((thirdPart & 0b0011'1111) << 6)  | (fourthPart & 0b0011'1111);
			}



			se::Uint32 uuuuu {(code >> 16) & 0b0001'1111};

			if (uuuuu == 0) {
				UTF16Char character {(UTF16Char)(code & 0x00'00'FF'FF)};
				buffer.buffer[outputStringSize] = character;
				++outputStringSize;
				continue;
			}

			se::Uint32 xxxxxx {(code >> 10) & 0b0011'1111};
			se::Uint32 yyyyyyyyyy {code & 0b0011'1111'1111};
			se::Uint32 wwww {uuuuu - 1};

			UTF16Char part1 {(UTF16Char)(0b1101'1000'0000'0000 | (wwww << 6) | xxxxxx)};
			UTF16Char part2 {(UTF16Char)(0b1101'1100'0000'0000 | yyyyyyyyyy)};
			buffer.buffer[outputStringSize]   = part1;
			buffer.buffer[++outputStringSize] = part2;
			++outputStringSize;
		}


		return se::String<UTF16> (buffer.buffer, outputStringSize);
	}


} // namespace se
