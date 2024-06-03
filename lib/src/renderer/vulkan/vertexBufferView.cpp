#include "se/renderer/vulkan/vertexBufferView.hpp"

#include <algorithm>
#include <map>



namespace se::renderer::vulkan {
	VertexBufferView::VertexBufferView(const se::renderer::VertexBufferViewInfos &infos) :
		se::renderer::VertexBufferView(infos),
		m_attributeDescriptions {},
		m_bindingDescriptions {}
	{
		static std::map<se::renderer::VertexType, se::ByteCount> vertexTypeToSizeMap {
			{se::renderer::VertexType::eFloat32, sizeof(se::Float32)},
			{se::renderer::VertexType::eUint32, sizeof(se::Uint32)},
			{se::renderer::VertexType::eInt32, sizeof(se::Int32)},
		};

		static std::map<se::renderer::VertexType, std::map<se::Uint64, VkFormat>> formatMap {
			{se::renderer::VertexType::eFloat32, {
				{1, VK_FORMAT_R32_SFLOAT},
				{2, VK_FORMAT_R32G32_SFLOAT},
				{3, VK_FORMAT_R32G32B32_SFLOAT},
				{4, VK_FORMAT_R32G32B32A32_SFLOAT},
			}},
			{se::renderer::VertexType::eUint32, {
				{1, VK_FORMAT_R32_UINT},
				{2, VK_FORMAT_R32G32_UINT},
				{3, VK_FORMAT_R32G32B32_UINT},
				{4, VK_FORMAT_R32G32B32A32_UINT},
			}},
			{se::renderer::VertexType::eInt32, {
				{1, VK_FORMAT_R32_SINT},
				{2, VK_FORMAT_R32G32_SINT},
				{3, VK_FORMAT_R32G32B32_SINT},
				{4, VK_FORMAT_R32G32B32A32_SINT},
			}}
		};

		static std::map<se::renderer::VertexRate, VkVertexInputRate> rateMap {
			{se::renderer::VertexRate::eVertex,   VK_VERTEX_INPUT_RATE_VERTEX},
			{se::renderer::VertexRate::eInstance, VK_VERTEX_INPUT_RATE_INSTANCE}
		};

		se::Uint64 stride {};
		m_attributeDescriptions.reserve(m_infos.attributes.size());

		std::vector<se::Uint64> bindings {};
		std::map<se::Uint64, se::ByteCount> strides {};
		std::map<se::Uint64, VkVertexInputRate> rates {};

		for (const auto &attribute : m_infos.attributes) {
			if (std::find(bindings.begin(), bindings.end(), attribute.binding) == bindings.end()) {
				bindings.push_back(attribute.binding);
				strides[attribute.binding] = 0;
				rates[attribute.binding] = VK_VERTEX_INPUT_RATE_VERTEX;
			}

			VkVertexInputAttributeDescription description {};
			description.binding = attribute.binding;
			description.location = attribute.location;
			description.offset = strides[attribute.binding];
			description.format = formatMap[attribute.type][attribute.dimension];

			m_attributeDescriptions.push_back(description);
			strides[attribute.binding] += attribute.dimension * vertexTypeToSizeMap[attribute.type];
			rates[attribute.binding] = rateMap[attribute.rate];
		}
		
		m_bindingDescriptions.reserve(bindings.size());

		for (const auto &binding : bindings) {
			VkVertexInputBindingDescription inputBindingDescription {};
			inputBindingDescription.binding = binding;
			inputBindingDescription.stride = strides[binding];
			inputBindingDescription.inputRate =  rates[binding];

			m_bindingDescriptions.push_back(inputBindingDescription);
		}
	}


	VertexBufferView::~VertexBufferView() {

	}


} // namespace se::renderer::vulkan
