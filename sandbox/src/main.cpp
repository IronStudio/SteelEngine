#include <se/test.hpp>


#ifdef WIN32
	#define main WinMain
#endif


int main(int, char **)
{
	se::test();

	return 0;
}