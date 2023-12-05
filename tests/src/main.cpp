#include "test.hpp"

#include "string.inl"


int main()
{
	int failedTest {0};
	int testCount {0};

	for (const auto &test : functions)
	{
		++testCount;

		if (!test.second())
		{
			++failedTest;
			std::cout << "Failed test '" << test.first << "'" << std::endl;
		}
	}

	std::cout << "Passed " << testCount - failedTest << " tests of " << testCount << std::endl;
	return failedTest;
}