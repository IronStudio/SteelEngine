#include "se/renderer/vulkan/attributeBufferView.hpp"

#include <map>

#include "se/assertion.hpp"
#include "se/exceptions.hpp"
#include "se/logger.hpp"
#include "se/renderer/vulkan/bufferViewUsage.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/renderer/vulkan/shader.hpp"



namespace se::renderer::vulkan {

	struct TypeInfos {
		se::ByteCount alignment;
		se::ByteCount size;
	};


	AttributeBufferView::AttributeBufferView(const se::renderer::AttributeBufferViewInfos &infos) :
		se::renderer::AttributeBufferView(infos),
		m_layoutBinding {}
	{
		static const std::map<se::renderer::AttributeType, TypeInfos> typeInfosMap {
			{se::renderer::AttributeType::eFloat32, {4,  4}},
			{se::renderer::AttributeType::eInt32,   {4,  4}},
			{se::renderer::AttributeType::eMat2,    {16, 4*8}},
			{se::renderer::AttributeType::eMat3,    {16, 4*12}},
			{se::renderer::AttributeType::eMat4,    {16, 4*16}},
			{se::renderer::AttributeType::eVec2,    {8,  8}},
			{se::renderer::AttributeType::eVec2i,   {8,  8}},
			{se::renderer::AttributeType::eVec3,    {16, 16}},
			{se::renderer::AttributeType::eVec3i,   {16, 16}},
			{se::renderer::AttributeType::eVec4,    {16, 16}},
			{se::renderer::AttributeType::eVec4i,   {16, 16}}
		};

		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};

		m_layoutBinding.binding = m_infos.binding;
		m_layoutBinding.descriptorCount = 1;
		m_layoutBinding.descriptorType = bufferUsageSeToVk(m_infos.usage);
		m_layoutBinding.pImmutableSamplers = nullptr;
		m_layoutBinding.stageFlags = se::renderer::vulkan::shaderTypeMaskSeToVk(m_infos.shaderTypes);

		SE_INFO("Attribute access (se) : {}, (vk) : {}", m_infos.shaderTypes.content, (size_t)m_layoutBinding.stageFlags);

		se::ByteCount currentOffset {0};
		for (const auto &attribute : m_infos.attributes) {
			se::renderer::AttributeInfos attrInfos {};
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


	AttributeBufferView::~AttributeBufferView() {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (m_infos.context)->getDevice()->getDevice()};
	}


} // namespace se::renderer::vulkan
