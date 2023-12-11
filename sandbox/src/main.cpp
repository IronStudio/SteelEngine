#include <iostream>

#include <se/essentials/assertion.hpp>
#include <se/essentials/exception.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/memory/stackAllocator.hpp>
#include <se/logger.hpp>



int main(int, char **)
{
	try
	{
		se::Logger<se::Origin::eApp>::load();
		SE_AppLogger.setStream(&std::cout);
		SE_AppLogger << se::LogStart() << "I am an increadible log !" << se::endLog;
		SE_AppLogger.format(se::LogStart(), "I'm a formated %d log", 12);
		SE_AppLogger.flush();
		se::Logger<se::Origin::eApp>::unload();
	}

	catch (const se::Exception &exception)
	{
		std::cout << exception.what() << std::endl;
	}

	catch (const se::Assertion &assertion)
	{
		std::cout << assertion.what() << std::endl;
	}

	return 0;
}