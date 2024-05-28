#pragma once

#include <vector>

#include <glad/glad.h>

#include "se/core.hpp"
#include "se/renderer/vramAllocator.hpp"



namespace se::renderer::opengl {
	class SE_CORE VramAllocatorHandle final : public se::renderer::VramAllocatorHandle {
		public:
			VramAllocatorHandle(se::renderer::VramAllocator *allocator, se::ByteCount start, se::ByteCount size);
			~VramAllocatorHandle() override;

			bool isValid() override;

			inline se::ByteCount getStart() const noexcept {return m_start;}
			inline se::ByteCount getSize() const noexcept {return m_size;}
			inline se::ByteCount getEnd() const noexcept {return m_start + m_size;}

		private:
			se::ByteCount m_start;
			se::ByteCount m_size;
	};


	class SE_CORE VramAllocator final : public se::renderer::VramAllocator {
		public:
			using Handle = se::renderer::VramAllocatorHandle;

			VramAllocator(const se::renderer::VramAllocatorInfos &infos);
			~VramAllocator() noexcept override;

			std::unique_ptr<Handle> allocate(const se::renderer::VramAllocationInfos &allocationInfos) override;
			void free(Handle *handle) override;

			inline GLenum getBuffer() const noexcept {return m_memory;};

		private:
			struct Allocation {
				se::ByteCount start;
				se::ByteCount end;
			};

			std::vector<Allocation> m_allocationTable;
			GLenum m_memory;
	};

} // namespace se::renderer::opengl
