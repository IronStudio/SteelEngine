#pragma once

#include <vector>

#include "se/core.hpp"
#include "se/renderer/context.hpp"
#include "se/renderer/attributeBufferView.hpp"
#include "se/renderer/vramAllocator.hpp"
#include "se/types.hpp"
#include "se/utils/bitField.hpp"



namespace se::renderer {
	SE_CREATE_BIT_FIELD(BufferUsage, BufferUsageMask,
		eTransferSrc = 0b0000'0001,
		eTransferDst = 0b0000'0010,
		eUniform     = 0b0000'0100,
		eStorage     = 0b0000'1000,
		eIndex       = 0b0001'0000,
		eVertex      = 0b0010'0000,
		eIndirect    = 0b0100'0000
	);

	struct BufferInfos {
		se::renderer::Context *context;
		se::ByteCount size;
		se::renderer::BufferUsageMask usage;
		se::renderer::VramAllocator *allocator;
	};

	struct BufferWriteInfos {
		std::vector<se::Byte> value;
		se::ByteCount offset;
	};

	struct BufferWriteAttribute {
		std::string name;
		std::vector<se::Byte> value;
	};

	struct BufferWriteAttributeInfos {
		se::renderer::AttributeBufferView *attributeBufferView;
		std::vector<BufferWriteAttribute> attributes;
		se::ByteCount offset;
	};

	class SE_CORE Buffer {
		public:
			inline Buffer(const se::renderer::BufferInfos &infos) : m_infos {infos} {}
			virtual ~Buffer() = default;

			virtual void write(const se::renderer::BufferWriteInfos &writeInfos) = 0;
			virtual void write(const se::renderer::BufferWriteAttributeInfos &writeInfos) = 0;

			inline const se::renderer::BufferInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::BufferInfos m_infos;
	};

	struct BufferTransferorInfos {
		se::renderer::Context *context;
	};

	struct BufferTransferInfos {
		se::renderer::Buffer *source;
		se::renderer::Buffer *destination;
		se::ByteCount srcOffset;
		se::ByteCount dstOffset;
		se::ByteCount size;
	};

	class SE_CORE BufferTransferor {
		public:
			BufferTransferor(const se::renderer::BufferTransferorInfos &infos) : m_infos {infos} {}
			virtual ~BufferTransferor() = default;

			virtual void transfer(const se::renderer::BufferTransferInfos &infos) = 0;
			virtual void sync() = 0;

			inline const se::renderer::BufferTransferorInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::renderer::BufferTransferorInfos m_infos;
	};

} // namespace se::renderer
