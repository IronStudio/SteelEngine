#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/buffer.hpp"



namespace se::renderer::vulkan {
	class Buffer : public se::renderer::Buffer {
		public:
			Buffer(const se::renderer::BufferInfos &infos);
			~Buffer() override;

			inline VkBuffer getBuffer() const noexcept {return m_buffer;}

		private:
			VkBuffer m_buffer;
	};

} // namespace se::renderer::vulkan
