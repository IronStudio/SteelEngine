#include "se/renderer/vulkan/uniformBufferView.hpp"

#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/shader.hpp"



namespace se::renderer::vulkan {
	UniformBufferView::UniformBufferView(const se::renderer::UniformBufferViewInfos &infos) :
		se::renderer::UniformBufferView(infos),
		m_layoutBinding {},
		m_layout {VK_NULL_HANDLE}
	{
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		m_layoutBinding.binding = m_infos.binding;
		m_layoutBinding.descriptorCount = 1;
		m_layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_layoutBinding.pImmutableSamplers = nullptr;
		m_layoutBinding.stageFlags = se::renderer::vulkan::shaderTypeMaskSeToVk(m_infos.shaderTypes);

		VkDescriptorSetLayoutCreateInfo layoutCreateInfos {};
		layoutCreateInfos.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutCreateInfos.bindingCount = 1;
		layoutCreateInfos.pBindings = &m_layoutBinding;

		if (vkCreateDescriptorSetLayout(device, &layoutCreateInfos, nullptr, &m_layout) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create ubo view layout");
	}


	UniformBufferView::~UniformBufferView() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (m_layout != VK_NULL_HANDLE)
			vkDestroyDescriptorSetLayout(device, m_layout, nullptr);
	}


} // namespace se::renderer::vulkan
