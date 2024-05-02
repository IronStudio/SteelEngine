#include "se/threads/thread.hpp"

#include "se/utils/template.hpp"



namespace se::threads {
	template <typename ...Args>
	se::threads::SerializedArgs serialize(Args ...args) {
		constexpr se::ByteCount totalSize {se::utils::sizeofVariadic<Args...> ()};
		se::threads::SerializedArgs output {totalSize};

		se::ByteCount currentSize {0};
		([&] () {
			*reinterpret_cast<Args*> (static_cast<se::Byte*> (output) + currentSize) = args;
			currentSize += sizeof(Args);
		} (), ...);

		return output;
	}



	template <typename ...Args>
	std::tuple<Args...> deserialize(const se::threads::SerializedArgs &args) {
		std::tuple<Args...> output {};

		se::ByteCount currentSize {0};
		([&] () {
			std::get<Args> (output) = *reinterpret_cast<Args*> (static_cast<se::Byte*> (args) + currentSize);
			currentSize += sizeof(Args);
		} (), ...);

		return output;
	}




	#define this reinterpret_cast<Implementation*> (this)

	template <typename Implementation>
	SE_REQUIRES(se::concepts::ThreadImplementation<Implementation>)
	ThreadBase<Implementation>::ThreadBase() {
		this->create();
	}


	
	template <typename Implementation>
	SE_REQUIRES(se::concepts::ThreadImplementation<Implementation>)
	ThreadBase<Implementation>::~ThreadBase() {
		this->destroy();
	}


	
	template <typename Implementation>
	SE_REQUIRES(se::concepts::ThreadImplementation<Implementation>)
	ThreadBase<Implementation>::ThreadBase(const Callback &callback) {
		this->create();
		this->attach(callback);
	}


	
	template <typename Implementation>
	SE_REQUIRES(se::concepts::ThreadImplementation<Implementation>)
	void ThreadBase<Implementation>::bind(const Callback &callback) {
		this->bind(callback);
	}



	template <typename Implementation>
	SE_REQUIRES(se::concepts::ThreadImplementation<Implementation>)
	void ThreadBase<Implementation>::launch(const se::threads::SerializedArgs &args, int coreIndex) {
		this->launch(args, coreIndex);
	}


	
	template <typename Implementation>
	SE_REQUIRES(se::concepts::ThreadImplementation<Implementation>)
	void ThreadBase<Implementation>::detach() {
		this->detach();
	}


	
	template <typename Implementation>
	SE_REQUIRES(se::concepts::ThreadImplementation<Implementation>)
	void ThreadBase<Implementation>::waitOn() {
		this->waitOn();
	}

	#undef this

} // namespace se::threads
