#include <iostream>

#include "utils/convert.hpp"



namespace se
{
	std::string int64ToString(se::Uint64 number, se::Uint32 base)
	{
		static std::string numbers {"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
		std::string output {};

		if (number == 0)
			return "0";

		while (number != 0)
		{
			auto tmp {number % base};
			output = numbers[tmp] + output;
			number = (number - tmp) / base;
		}

		return output;
	}



} // namespace se
