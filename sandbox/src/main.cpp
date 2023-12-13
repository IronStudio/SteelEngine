#include <iostream>

#include <se/essentials/assertion.hpp>
#include <se/essentials/exception.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/memory/stackAllocator.hpp>
#include <se/logger.hpp>
#include <se/memory/heapAllocator.hpp>



int main(int, char **)
{
	try
	{
		se::Logger<se::Origin::eApp>::load();
		se::Logger<se::Origin::eEngine>::load();
		se::Logger<se::Origin::eUnknown>::load();
		SE_AppLogger.setStream(&std::cout);
		SE_CoreLogger.setStream(&std::cout);
		SE_UnknownLogger.setStream(&std::cout);


		se::HeapAllocator allocator {1024};
		auto handle = allocator.allocate<int> ();
		auto handle2 = allocator.allocate<int> (3);
		auto handle3 = allocator.allocate<int> (1);
		auto handle4 = allocator.allocate<int> ();

		SE_AppLogger << se::LogStart() << "Usage : " << allocator.getUsage() << se::endLog;
		SE_AppLogger << se::LogStart() << "Fragmentation : " << allocator.getFragmentationSpaceSize() << se::endLog;

		*handle3 = 42;

		allocator.free(handle2);

		SE_AppLogger << se::LogStart() << "Usage : " << allocator.getUsage() << se::endLog;
		SE_AppLogger << se::LogStart() << "Fragmentation : " << allocator.getFragmentationSpaceSize() << se::endLog;

		allocator.defragment();

		SE_AppLogger << se::LogStart() << "Usage : " << allocator.getUsage() << se::endLog;
		SE_AppLogger << se::LogStart() << "Fragmentation : " << allocator.getFragmentationSpaceSize() << se::endLog;

		SE_AppLogger << se::LogStart() << "Test : " << *handle3 << se::endLog;


		se::Logger<se::Origin::eEngine>::unload();
		se::Logger<se::Origin::eUnknown>::unload();
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