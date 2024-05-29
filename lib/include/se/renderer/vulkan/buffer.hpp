#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/buffer.hpp"
#include "se/renderer/vulkan/vramAllocator.hpp"



namespace se::renderer::vulkan {
	class SE_CORE Buffer : public se::renderer::Buffer {
		public:
			Buffer(const se::renderer::BufferInfos &infos);
			~Buffer() override;

			inline VkBuffer getBuffer() const noexcept {return m_buffer;}

		private:
			VkBuffer m_buffer;
			std::unique_ptr<se::renderer::VramAllocatorHandle> m_allocatorHandle;
	};

} // namespace se::renderer::vulkan
