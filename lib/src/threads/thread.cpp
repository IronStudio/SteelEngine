#include "se/threads/thread.hpp"

#include <cstdlib>
#include <cstring>

#if defined(SE_LINUX) || defined(SE_APPLE)
	extern "C" {
		#include <sys/sysinfo.h>
	}
#endif

#include "se/assertion.hpp"
#include "se/exceptions.hpp"



namespace se::threads {
	SerializedArgs::SerializedArgs() :
		m_data {nullptr},
		m_size {0}
	{

	}



	SerializedArgs::~SerializedArgs() {
		if (m_data != nullptr)
			free(m_data);
	}



	SerializedArgs::SerializedArgs(const se::threads::SerializedArgs &args) :
		m_data {reinterpret_cast<se::Byte*> (malloc(args.m_size))},
		m_size {args.m_size}
	{
		memcpy(m_data, args.m_data, m_size);
	}



	const se::threads::SerializedArgs &SerializedArgs::operator=(const se::threads::SerializedArgs &args) {
		if (m_data != nullptr)
			free(m_data);

		m_data = reinterpret_cast<se::Byte*> (malloc(args.m_size));
		m_size = args.m_size;
		memcpy(m_data, args.m_data, m_size);

		return *this;
	}



	SerializedArgs::SerializedArgs(se::ByteCount size) :
		m_data {reinterpret_cast<se::Byte*> (malloc(size))},
		m_size {size}
	{

	}



	SerializedArgs::operator se::Byte* () const noexcept {
		return m_data;
	}



#if defined(SE_LINUX) || defined(SE_APPLE)

	void PthreadImplementation::create() {
		m_thread = 0;
		m_callback = nullptr;
	}



	void PthreadImplementation::destroy() {
		if (m_thread != 0)
			pthread_cancel(m_thread);
		m_thread = 0;
		m_callback = nullptr;
	}



	void PthreadImplementation::bind(const Callback &callback) {
		m_callback = callback;
	}



	void PthreadImplementation::launch(const se::threads::SerializedArgs &args, int coreIndex) {
		SE_ASSERT(m_thread == 0, "Can't launch thread that was already launched");
		SE_ASSERT(m_callback != nullptr, "Can't launch thread that has no callback binded");

		int cpuCount {get_nprocs()};

		pthread_attr_t threadAttr;
		(void)pthread_attr_init(&threadAttr);

		if (coreIndex >= 0) {
			cpu_set_t *cpuSet {CPU_ALLOC(cpuCount)};
			CPU_ZERO_S(cpuCount, cpuSet);
			CPU_SET_S(0, cpuCount, cpuSet);
			(void)pthread_attr_setaffinity_np(&threadAttr, sizeof(cpuSet), cpuSet);
			CPU_FREE(cpuSet);
		}

		if (pthread_create(&m_thread, &threadAttr, reinterpret_cast<void*(*)(void*)> (m_callback), static_cast<se::Byte*> (args)) != 0) {
			(void)pthread_attr_destroy(&threadAttr);
			m_thread = 0;
			throw se::exceptions::RuntimeError("Can't create a pthread");
		}

		(void)pthread_attr_destroy(&threadAttr);
	}



	void PthreadImplementation::detach() {
		if (pthread_detach(m_thread) != 0)
			throw se::exceptions::RuntimeError("Can't detach a pthread");
		m_thread = 0;
	}



	void PthreadImplementation::waitOn() {
		if (pthread_join(m_thread, nullptr) != 0)
			throw se::exceptions::RuntimeError("Can't join a pthread");
		m_thread = 0;
	}

#endif


	void STLthreadImplementation::create() {
		m_thread = {};
		m_callback = nullptr;
	}



	void STLthreadImplementation::destroy() {

	}



	void STLthreadImplementation::bind(const Callback &callback) {
		m_callback = callback;
	}



	void STLthreadImplementation::launch(const se::threads::SerializedArgs &args, int coreIndex) {
		m_thread = std::thread(m_callback, args);
	}



	void STLthreadImplementation::detach() {

	}



	void STLthreadImplementation::waitOn() {
		m_thread.join();
	}

} // namespace se::threads
