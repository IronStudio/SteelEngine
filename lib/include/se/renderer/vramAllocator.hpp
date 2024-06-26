#pragma once

#include <memory>

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/types.hpp"
#include "se/utils/byteSize.hpp"



namespace se::renderer {
	using namespace se::literals;

	struct VramAllocator;

	class SE_CORE VramAllocatorHandle {
		public:
			inline VramAllocatorHandle(se::renderer::VramAllocator *allocator) : m_allocator {allocator} {}
			virtual ~VramAllocatorHandle() = default;

			inline se::renderer::VramAllocator *getAllocator() const noexcept {return m_allocator;}

			virtual bool isValid() = 0;

		protected:
			se::renderer::VramAllocator *m_allocator;
	};

	enum class VramUsageNature {
		eCpuToGpu,
		eGpuToCpu,
		eGpuToGpu
	};

	enum class VramUsageFrequency {
		eForgotten,
		eStatic,
		eDynamic
	};

	struct VramAllocatorInfos {
		se::renderer::Context *context;
		se::ByteCount chunkSize {256_MiB};
		se::renderer::VramUsageNature usageNature;
		se::renderer::VramUsageFrequency usageFrequency;
	};

	struct VramAllocationInfos {
		se::ByteCount size;
		se::ByteCount alignement;
	};

	class SE_CORE VramAllocator {
		public:
			using Handle = se::renderer::VramAllocatorHandle;

			inline VramAllocator(const se::renderer::VramAllocatorInfos &infos) : m_infos {infos} {}
			virtual ~VramAllocator() = default;

			virtual std::unique_ptr<Handle> allocate(const se::renderer::VramAllocationInfos &allocationInfos) = 0;
			virtual void free(Handle *handle) = 0;

			const se::renderer::VramAllocatorInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::VramAllocatorInfos m_infos;
	};

} // namespace se::renderer
