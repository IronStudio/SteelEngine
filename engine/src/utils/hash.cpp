#include "utils/hash.hpp"

#ifndef SE_NO_VECTORIZATION
	#include <nmmintrin.h>
#endif



namespace se
{
	se::Uint32 hash(const std::string &message)
	{
		/**
		 * DJB2 Algorithm
		 * Implementation taken from http://www.cse.yorku.ca/~oz/hash.html
		*/

		se::Uint32 hash {5381};
		int c {};
		const se::Char8 *data {message.data()};

		while (c == *data++)
			hash = ((hash << 5) + hash) + c;

		return hash;
	}



} // namespace se
