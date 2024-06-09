#pragma once

#include <map>
#include <vector>

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/shader.hpp"



namespace se::renderer {
	enum class AttributeType {
		eFloat32,
		eInt32,
		eVec2,
		eVec3,
		eVec4,
		eMat2,
		eMat3,
		eMat4,
		eVec2i,
		eVec3i,
		eVec4i
	};

	struct Attribute {
		std::string name;
		se::renderer::AttributeType type;
	};

	enum class AttributeBufferViewUsage {
		eUniform,
		eStorage
	};

	struct AttributeBufferViewInfos {
		se::renderer::Context *context;
		std::vector<se::renderer::Attribute> attributes;
		se::ByteCount offset;
		se::renderer::ShaderTypeMask shaderTypes;
		se::Uint64 binding;
		se::renderer::AttributeBufferViewUsage usage;
	};

	struct AttributeInfos {
		se::ByteCount offset;
		se::ByteCount size;
	};

	class SE_CORE AttributeBufferView {
		public:
			inline AttributeBufferView(const se::renderer::AttributeBufferViewInfos &infos) : m_infos {infos} {}
			virtual ~AttributeBufferView() = default;

			inline const se::renderer::AttributeBufferViewInfos &getInfos() const noexcept {return m_infos;}
			inline const std::map<std::string, se::renderer::AttributeInfos> &getAttributesInfos() const noexcept {return m_attributeInfos;}
			inline const se::renderer::AttributeInfos &getAttribute(const std::string &attr) const noexcept {
				return m_attributeInfos.find(attr)->second;
			}
			inline se::ByteCount getTotalSize() const noexcept {return m_totalSize;}

		protected:
			se::renderer::AttributeBufferViewInfos m_infos;
			std::map<std::string, se::renderer::AttributeInfos> m_attributeInfos;
			se::ByteCount m_totalSize;
	};

} // namespace se::renderer
