#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/depthBuffer.hpp"



namespace se::renderer::vulkan {
	class SE_CORE DepthBuffer final : public se::renderer::DepthBuffer {
		public:
			DepthBuffer(const se::renderer::DepthBufferInfos &infos);
			~DepthBuffer() override;

			inline VkImage getImage() const noexcept {return m_image;}
			inline VkImageView getImageView() const noexcept {return m_imageView;}

		private:
			VkImage m_image;
			VkImageView m_imageView;
			std::unique_ptr<se::renderer::VramAllocatorHandle> m_memoryHandle;
	};

} // namespace se::renderer::vulkan
