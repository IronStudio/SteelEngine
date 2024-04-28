#pragma once



namespace se::memory {
	class Allocator {
		public:
			Allocator() = default;
			virtual ~Allocator() = default;

			virtual void *allocate();
	};
} // namespace se::memory
