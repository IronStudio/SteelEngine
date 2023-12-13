#include <iostream>

#include <Python.h>

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


			Py_Initialize();

			PyObject *platformModule {PyImport_ImportModule("platform")};
			if (platformModule == nullptr)
				throw SE_AppRuntimeError("Can't load python module 'platform'");

			PyObject *systemFunction {PyObject_GetAttrString(platformModule, "system")};
			if (!systemFunction || !PyCallable_Check(systemFunction))
				throw SE_AppRuntimeError("Can't load or call 'platform.system' function");

			PyObject *systemObj {PyObject_CallObject(systemFunction, nullptr)};
			PyObject *str {PyUnicode_AsEncodedString(systemObj, "utf-8", "~E~")};
			const char *systemStr {PyBytes_AS_STRING(str)};
			SE_AppLogger << se::LogStart() << "System : " << systemStr << se::endLog;

			Py_XDECREF(systemFunction);
			Py_XDECREF(systemObj);
			Py_XDECREF(str);
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