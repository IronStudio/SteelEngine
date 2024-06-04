#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/uniformBufferView.hpp"



namespace se::renderer::vulkan {
	class SE_CORE UniformBufferView final : public se::renderer::UniformBufferView {
		public:
			UniformBufferView(const se::renderer::UniformBufferViewInfos &infos);
			~UniformBufferView() override;

			inline const VkDescriptorSetLayoutBinding &getLayoutBinding() const noexcept {return m_layoutBinding;}
			inline VkDescriptorSetLayout getLayout() const noexcept {return m_layout;}

		private:
			VkDescriptorSetLayoutBinding m_layoutBinding;
			VkDescriptorSetLayout m_layout;
	};

} // namespace se::renderer::vulkan
