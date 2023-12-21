#include "utils/hash.hpp"

#ifndef SE_NO_VECTORIZATION
	#include <nmmintrin.h>
#endif



namespace se
{
	se::Uint32 hash(const std::string &message)
	{
		#ifndef SE_NO_VECTORIZATION
			se::Uint32 output {0};
			se::Length length {message.size()};
			const se::Char8 *data {message.data()};

			if (length == 0)
				return 0;

			if (length == 1)
				return _mm_crc32_u8(output, *data);

			if (length == 2)
				return _mm_crc32_u16(output, *(se::Uint16*)data);

			while (length >= 4)
			{
				output = _mm_crc32_u32(output, *(se::Uint32*)data);
				data += 4;
				length -= 4;
			}

			if (length >= 2)
			{
				output = _mm_crc32_u16(output, *(se::Uint16*)data);
				data += 2;
				length -= 2;
			}

			if (length == 1)
				return _mm_crc32_u8(output, *data);

			return output;

		#else

		#endif
	}



} // namespace se
