#pragma once

#include <vulkan/vulkan.h>

#include "se/core.hpp"
#include "se/renderer/buffer.hpp"
#include "se/renderer/vulkan/vramAllocator.hpp"



namespace se::renderer::vulkan {
	class SE_CORE Buffer : public se::renderer::Buffer {
		public:
			Buffer(const se::renderer::BufferInfos &infos);
			~Buffer() override;

			void write(const se::renderer::BufferWriteInfos &writeInfos) override;
			void write(const se::renderer::BufferWriteAttributeInfos &writeInfos) override;

			inline VkBuffer getBuffer() const noexcept {return m_buffer;}
			inline const se::renderer::VramAllocatorHandle *getAllocatorHandle() const noexcept {return m_allocatorHandle.get();}

		private:
			VkBuffer m_buffer;
			std::unique_ptr<se::renderer::VramAllocatorHandle> m_allocatorHandle;
	};


	class SE_CORE BufferTransferor : public se::renderer::BufferTransferor {
		public:
			BufferTransferor(const se::renderer::BufferTransferorInfos &infos);
			~BufferTransferor() override;

			virtual void transfer(const se::renderer::BufferTransferInfos &infos) override;
			virtual void sync() override;


		private:
			static void s_loadCommandPool(const se::renderer::BufferTransferorInfos &infos);
			static void s_unloadCommandPool(const se::renderer::BufferTransferorInfos &infos);

			static se::Count s_instanceCount;
			static VkCommandPool s_commandPool;

			VkCommandBuffer m_commandBuffer;
			std::vector<VkFence> m_inUseFences;
			std::vector<VkFence> m_freeFences;
			std::mutex m_fenceMutex;
	};

} // namespace se::renderer::vulkan
