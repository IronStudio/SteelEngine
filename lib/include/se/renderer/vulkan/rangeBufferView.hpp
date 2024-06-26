#pragma once

#include <vulkan/vulkan.h>

#include "se/renderer/rangeBufferView.hpp"



namespace se::renderer::vulkan {
	class SE_CORE RangeBufferView final : public se::renderer::RangeBufferView {
		public:
			RangeBufferView(const se::renderer::RangeBufferViewInfos &infos);
			~RangeBufferView() override;

			inline const VkDescriptorSetLayoutBinding &getLayoutBinding() const noexcept {return m_layoutBinding;}

		private:
			VkDescriptorSetLayoutBinding m_layoutBinding;
	};

} // namespace se::renderer::vulkan
