#include "se/renderer/vulkan/rangeBufferView.hpp"

#include <map>

#include "se/assertion.hpp"
#include "se/exceptions.hpp"
#include "se/logger.hpp"
#include "se/renderer/vulkan/bufferViewUsage.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/shader.hpp"



namespace se::renderer::vulkan {
	RangeBufferView::RangeBufferView(const se::renderer::RangeBufferViewInfos &infos) :
		se::renderer::RangeBufferView(infos),
		m_layoutBinding {}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		m_layoutBinding.binding = m_infos.binding;
		m_layoutBinding.descriptorCount = 1;
		m_layoutBinding.descriptorType = bufferUsageSeToVk(m_infos.usage);
		m_layoutBinding.pImmutableSamplers = nullptr;
		m_layoutBinding.stageFlags = se::renderer::vulkan::shaderTypeMaskSeToVk(m_infos.shaderTypes);
	}


	RangeBufferView::~RangeBufferView() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};
	}


} // namespace se::renderer::vulkan
