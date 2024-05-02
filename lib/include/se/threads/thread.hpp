#pragma once

#include <thread>
#include <tuple>

#if defined(SE_LINUX) || defined(SE_APPLE)
	extern "C" {
		#include <pthread.h>
	}
#endif

#include "se/core.hpp"
#include "se/concepts.hpp"
#include "se/types.hpp"


#ifndef SE_APPLE

namespace se::threads {
	struct SerializedArgs;
}

namespace se::concepts {
	template <typename T>
	concept ThreadImplementation = requires(T obj) {
		{obj.create()}  -> std::same_as<void>;
		{obj.destroy()} -> std::same_as<void>;
		{obj.bind(nullptr)} -> std::same_as<void>;
		{obj.launch(se::threads::SerializedArgs(), int(0))} -> std::same_as<void>;
		{obj.detach()}  -> std::same_as<void>;
		{obj.waitOn()}  -> std::same_as<void>;
	};
} // namespace se::concepts
		
#endif


namespace se::threads {
	class SE_CORE SerializedArgs {
		public:
			SerializedArgs();
			~SerializedArgs();

			SerializedArgs(const se::threads::SerializedArgs &args);
			const se::threads::SerializedArgs &operator=(const se::threads::SerializedArgs &args);

			SerializedArgs(se::ByteCount size);

			explicit operator se::Byte* () const noexcept;

		private:
			se::Byte *m_data;
			se::ByteCount m_size;
	};

	template <typename ...Args>
	se::threads::SerializedArgs serialize(Args ...args);

	template <typename ...Args>
	std::tuple<Args...> deserialize(const se::threads::SerializedArgs &args);



	struct ThreadInfos {
		int coreIndex {-1};
	};


	template <typename Implementation>
	SE_REQUIRES(se::concepts::ThreadImplementation<Implementation>)
	class ThreadBase final : public Implementation {
		public:
			using Callback = void(*)(se::threads::SerializedArgs);

			inline ThreadBase();
			inline ~ThreadBase();

			inline ThreadBase(const Callback &callback);
			inline void bind(const Callback &callback);

			inline void launch(const se::threads::SerializedArgs &args, int coreIndex = -1);

			inline void detach();
			inline void waitOn();
	};



#if defined(SE_LINUX) || defined(SE_APPLE)
	class SE_CORE PthreadImplementation {
		public:
			using Callback = void(*)(se::threads::SerializedArgs);

			void create();
			void destroy();

			void bind(const Callback &callback);
			void launch(const se::threads::SerializedArgs &args, int coreIndex);

			void detach();
			void waitOn();

		private:
			Callback m_callback;
			pthread_t m_thread;
	};


	//using Thread = se::threads::ThreadBase<PthreadImplementation>;
#endif


	class SE_CORE STLthreadImplementation {
		public:
			using Callback = void(*)(se::threads::SerializedArgs);

			void create();
			void destroy();

			void bind(const Callback &callback);
			void launch(const se::threads::SerializedArgs &args, int coreIndex);

			void detach();
			void waitOn();

		private:
			std::thread m_thread;
			Callback m_callback;
	};


	using Thread = se::threads::ThreadBase<STLthreadImplementation>;



} // namespace se::threads



#include "se/threads/thread.inl"