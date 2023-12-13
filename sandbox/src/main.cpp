#include <iostream>

#include <se/application.hpp>
#include <se/essentials/assertion.hpp>
#include <se/essentials/exception.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/memory/stackAllocator.hpp>
#include <se/logger.hpp>
#include <se/memory/heapAllocator.hpp>



class SandboxApp final : public se::Application
{
	public:
		SandboxApp() = default;
		~SandboxApp() = default;

		void onLoad() override
		{
			se::Application::onLoad();

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
		}


		void onUpdate() override {}
		void onError() noexcept override {}
		void onUnload() noexcept override {se::Application::onUnload();}
};



se::Application *se::createApplication()
{
	return new SandboxApp();
}



#include <se/entryPoint.inl>