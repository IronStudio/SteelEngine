#include "se/renderer/vulkan/uniformBufferView.hpp"

#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/shader.hpp"



namespace se::renderer::vulkan {
	struct TypeInfos {
		se::ByteCount alignment;
		se::ByteCount size;
	};


	UniformBufferView::UniformBufferView(const se::renderer::UniformBufferViewInfos &infos) :
		se::renderer::UniformBufferView(infos),
		m_layoutBinding {},
		m_layout {VK_NULL_HANDLE}
	{
		static const std::map<se::renderer::UniformType, TypeInfos> typeInfosMap {
			{se::renderer::UniformType::eFloat32, {4,  4}},
			{se::renderer::UniformType::eInt32,   {4,  4}},
			{se::renderer::UniformType::eMat2,    {16, 4*8}},
			{se::renderer::UniformType::eMat3,    {16, 4*12}},
			{se::renderer::UniformType::eMat4,    {16, 4*16}},
			{se::renderer::UniformType::eVec2,    {8,  8}},
			{se::renderer::UniformType::eVec2i,   {8,  8}},
			{se::renderer::UniformType::eVec3,    {16, 16}},
			{se::renderer::UniformType::eVec3i,   {16, 16}},
			{se::renderer::UniformType::eVec4,    {16, 16}},
			{se::renderer::UniformType::eVec4i,   {16, 16}}
		};

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

		se::ByteCount currentOffset {0};
		for (const auto &attribute : m_infos.attributes) {
			se::renderer::UniformAttributeInfos attrInfos {};
			const auto &typeInfos {typeInfosMap.find(attribute.type)->second};

			if (currentOffset % typeInfos.alignment)
				currentOffset += typeInfos.alignment - (currentOffset % typeInfos.alignment);

			attrInfos.offset = currentOffset;
			attrInfos.size = typeInfos.size;
			m_attributeInfos[attribute.name] = attrInfos;

			currentOffset += typeInfos.size;
		}

		m_totalSize = currentOffset;
	}


	UniformBufferView::~UniformBufferView() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		if (m_layout != VK_NULL_HANDLE)
			vkDestroyDescriptorSetLayout(device, m_layout, nullptr);
	}


} // namespace se::renderer::vulkan
