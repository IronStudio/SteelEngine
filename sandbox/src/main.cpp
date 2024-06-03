#include <iostream>
#include <thread>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

#define SE_APPLICATION_MAIN_FILE
#include <se/steelEngine.hpp>
#include <se/core.hpp>
#include <se/concepts.hpp>
#include <se/duration.hpp>
#include <se/engine.hpp>
#include <se/input/inputManager.hpp>
#include <se/math.hpp>
#include <se/memory/poolAllocator.hpp>
#include <se/threads/thread.hpp>
#include <se/threads/job.hpp>
#include <se/threads/jobScheduler.hpp>
#include <se/ecs/scene.hpp>
#include <se/utils/version.hpp>
#include <se/window/windowManager.hpp>

#include <se/renderer/vulkan/buffer.hpp>
#include <se/renderer/vulkan/context.hpp>
#include <se/renderer/vulkan/format.hpp>
#include <se/renderer/vulkan/pipeline.hpp>
#include <se/renderer/vulkan/shader.hpp>
#include <se/renderer/vulkan/vertexBufferView.hpp>
#include <se/renderer/vulkan/vramAllocator.hpp>

#include <se/renderer/opengl/context.hpp>
#include <se/renderer/opengl/vramAllocator.hpp>


using namespace se::literals;


class SandboxApp : public se::Application {
	public:
		SandboxApp() {
			this->load();
		}

		~SandboxApp() override {}

		void run() override {
			SE_LOGGER.setSeverityMask(se::LogSeverity::eError | se::LogSeverity::eFatal | se::LogSeverity::eWarning | se::LogSeverity::eInfo);

			SE_LOGGER << se::LogInfos(se::LogSeverity::eVerbose) << "Hello" << se::endLog;
			SE_LOGGER << se::LogInfos(se::LogSeverity::eInfo) << "Hello" << se::endLog;
			SE_LOGGER << se::LogInfos(se::LogSeverity::eWarning) << "Hello" << se::endLog;
			SE_LOGGER << se::LogInfos(se::LogSeverity::eError) << "Hello" << se::endLog;
			SE_LOGGER << se::LogInfos(se::LogSeverity::eFatal) << "Hello" << se::endLog;

			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eVerbose) << "Hello" << se::endLog;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eInfo) << "Hello" << se::endLog;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eWarning) << "Hello" << se::endLog;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eError) << "Hello" << se::endLog;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eFatal) << "Hello" << se::endLog;


			/** @brief Window */
			se::window::WindowInfos windowInfos {};
			windowInfos.title = "SteelEngine_sandbox";
			windowInfos.size = {16 * 70, 9 * 70};
			windowInfos.position = {se::window::centerPosition, se::window::undefinedPosition};
			windowInfos.graphicsApi = se::renderer::GraphicsApi::eVulkan;
			windowInfos.flags = se::window::WindowFlags::eResizable | se::window::WindowFlags::eMasterWindow;
			se::window::Window &window {se::window::WindowManager::createWindow(windowInfos)};

		/*	windowInfos.title = "2nd";
			windowInfos.flags = se::window::WindowFlags::eResizable;
			windowInfos.minSize = {16 * 10, 9 * 10};
			windowInfos.maxSize = {16 * 100, 9 * 100};
			se::window::Window &window2 {se::window::WindowManager::createWindow(windowInfos)};*/


			/** @brief Context */
			se::renderer::ContextInfos contextInfos {};
			contextInfos.applicationName = "SteelEngine_sandbox";
			contextInfos.applicationVersion = "1.0.0"_v;
			contextInfos.preferredGPU = se::renderer::GPUType::eDiscret;
			contextInfos.linkedWindow = &window;
			se::renderer::vulkan::Context context {contextInfos};


			/** @brief Vertices */
			std::vector<se::Float> vertices {
				0.5f, 0.5f,    1.f, 0.f, 0.f,
				0.f, -0.5f,    0.f, 1.f, 0.f,
				-0.5f, 0.5f,   0.f, 0.f, 1.f,
			};


			/** @brief Staging memory */
			se::renderer::VramAllocatorInfos allocatorInfos {};
			allocatorInfos.chunkSize = 64_MiB;
			allocatorInfos.context = &context;
			allocatorInfos.usageNature = se::renderer::VramUsageNature::eCpuToGpu;
			allocatorInfos.usageFrequency = se::renderer::VramUsageFrequency::eForgotten;
			se::renderer::vulkan::VramAllocator stagingAllocator {allocatorInfos};

			stagingAllocator.logAllocationTable();

			/** @brief Staging buffer */
			se::renderer::BufferInfos bufferInfos {};
			bufferInfos.context = &context;
			bufferInfos.allocator = &stagingAllocator;
			bufferInfos.usage = se::renderer::BufferUsage::eTransferSrc;
			bufferInfos.size = vertices.size() * sizeof(se::Float);
			se::renderer::vulkan::Buffer stagingBuffer {bufferInfos};

			se::renderer::BufferWriteInfos bufferWriteInfos {};
			bufferWriteInfos.offset = 0;
			bufferWriteInfos.value.assign((se::Byte*)vertices.data(), (se::Byte*)(vertices.data() + vertices.size()));
			stagingBuffer.write(bufferWriteInfos);

			stagingAllocator.logAllocationTable();

			/** @brief GPU Memory */
			allocatorInfos.chunkSize = 256_MiB;
			allocatorInfos.context = &context;
			allocatorInfos.usageNature = se::renderer::VramUsageNature::eGpuToGpu;
			allocatorInfos.usageFrequency = se::renderer::VramUsageFrequency::eDynamic;
			se::renderer::vulkan::VramAllocator gpuAllocator {allocatorInfos};

			/** @brief Vertex buffer */
			bufferInfos.context = &context;
			bufferInfos.allocator = &gpuAllocator;
			bufferInfos.usage = se::renderer::BufferUsage::eVertex | se::renderer::BufferUsage::eTransferDst;
			bufferInfos.size = vertices.size() * sizeof(se::Float);
			se::renderer::vulkan::Buffer vertexBuffer {bufferInfos};


			/** @brief Transfer */
			se::renderer::BufferTransferorInfos bufferTransferorInfos {};
			bufferTransferorInfos.context = &context;
			se::renderer::vulkan::BufferTransferor bufferTransferor {bufferTransferorInfos};

			se::renderer::BufferTransferInfos bufferTransferInfos {};
			bufferTransferInfos.source = &stagingBuffer;
			bufferTransferInfos.destination = &vertexBuffer;
			bufferTransferInfos.srcOffset = 0;
			bufferTransferInfos.dstOffset = 0;
			bufferTransferInfos.size = vertices.size() * sizeof(se::Float);
			bufferTransferor.transfer(bufferTransferInfos);


			/** @brief VB view */
			se::renderer::VertexBufferViewInfos vertexBufferViewInfos {};
			vertexBufferViewInfos.context = &context;
			vertexBufferViewInfos.attributes = {
				{se::renderer::VertexType::eFloat32, 0, 2, 0},
				{se::renderer::VertexType::eFloat32, 1, 3, 0}
			};
			se::renderer::vulkan::VertexBufferView vertexBufferView {vertexBufferViewInfos};


			/** @brief Shaders */
			se::renderer::ShaderInfos shaderInfos {};
			shaderInfos.context = &context;
			shaderInfos.entryPoint = "main";

			shaderInfos.type = se::renderer::ShaderType::eVertex;
			shaderInfos.file = "shaders/test.vert";
			se::renderer::vulkan::Shader vertexShader {shaderInfos};

			shaderInfos.type = se::renderer::ShaderType::eFragment;
			shaderInfos.file = "shaders/test.frag";
			se::renderer::vulkan::Shader fragmentShader {shaderInfos};


			/** @brief Pipeline */
			se::renderer::PipelineInfos pipelineInfos {};
			pipelineInfos.context = &context;
			pipelineInfos.vertexBufferView = &vertexBufferView;
			pipelineInfos.shaders = {&vertexShader, &fragmentShader};
			pipelineInfos.colorAttachmentFormats = {se::renderer::vulkan::formatVkToSe(context.getSwapchain()->getFormat().format)};
			pipelineInfos.depthAttachmentFormat = se::renderer::Format::eNone;
			pipelineInfos.stencilAttachmentFormat = se::renderer::Format::eNone;
			se::renderer::vulkan::Pipeline pipeline {pipelineInfos};


			/********************************************/
			/******** playing a bit with vulkan *********/
			/********************************************/


			VkCommandPoolCreateInfo commandPoolCreateInfos {};
			commandPoolCreateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			commandPoolCreateInfos.queueFamilyIndex = context.getDevice()->getQueueFamilyIndices().find(se::renderer::vulkan::QueueType::eGraphics)->second;
			commandPoolCreateInfos.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			VkCommandPool graphicsCommandPool {VK_NULL_HANDLE};
			(void)vkCreateCommandPool(context.getDevice()->getDevice(), &commandPoolCreateInfos, nullptr, &graphicsCommandPool);

			VkCommandBufferAllocateInfo commandBufferAllocateInfos {};
			commandBufferAllocateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			commandBufferAllocateInfos.commandBufferCount = 1;
			commandBufferAllocateInfos.commandPool = graphicsCommandPool;
			commandBufferAllocateInfos.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			VkCommandBuffer graphicsCommandBuffer {};
			(void)vkAllocateCommandBuffers(context.getDevice()->getDevice(), &commandBufferAllocateInfos, &graphicsCommandBuffer);

			commandPoolCreateInfos.queueFamilyIndex = context.getDevice()->getQueueFamilyIndices().find(se::renderer::vulkan::QueueType::ePresent)->second;



			se::Uint32 imageIndex {};

			VkSemaphore imageReadySemaphore {VK_NULL_HANDLE};
			VkSemaphore imageDrawnSemaphore {VK_NULL_HANDLE};
			VkFence previousFrameReadyFence {VK_NULL_HANDLE};

			VkSemaphoreCreateInfo semaphoreCreateInfos {};
			semaphoreCreateInfos.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semaphoreCreateInfos.flags = 0;
			(void)vkCreateSemaphore(context.getDevice()->getDevice(), &semaphoreCreateInfos, nullptr, &imageReadySemaphore);
			(void)vkCreateSemaphore(context.getDevice()->getDevice(), &semaphoreCreateInfos, nullptr, &imageDrawnSemaphore);

			VkFenceCreateInfo fenceCreateInfos {};
			fenceCreateInfos.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			//fenceCreateInfos.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			(void)vkCreateFence(context.getDevice()->getDevice(), &fenceCreateInfos, nullptr, &previousFrameReadyFence);


			VkCommandBufferBeginInfo beginInfos {};
			beginInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfos.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			beginInfos.pInheritanceInfo = nullptr;
			(void)vkBeginCommandBuffer(graphicsCommandBuffer, &beginInfos);

			for (se::Count i {0}; i < context.getSwapchain()->getImages().size(); ++i) {
				VkImageMemoryBarrier imageMemoryBarrier {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				//imageMemoryBarrier.dstAccessMask = ;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				imageMemoryBarrier.image = context.getSwapchain()->getImages()[i];
				imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageMemoryBarrier.subresourceRange.levelCount = 1;
				imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
				imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
				imageMemoryBarrier.subresourceRange.layerCount = 1;
				vkCmdPipelineBarrier(
					graphicsCommandBuffer,
					VK_PIPELINE_STAGE_HOST_BIT,
					VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
					0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier
				);
			}

			(void)vkEndCommandBuffer(graphicsCommandBuffer);

			VkSubmitInfo submitInfos {};
			VkPipelineStageFlags dstStageMask {VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT};
			submitInfos.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfos.commandBufferCount = 1;
			submitInfos.pCommandBuffers = &graphicsCommandBuffer;
			submitInfos.waitSemaphoreCount = 0;
			submitInfos.pWaitSemaphores = nullptr;
			submitInfos.pWaitDstStageMask = &dstStageMask;
			submitInfos.signalSemaphoreCount = 0;
			submitInfos.pSignalSemaphores = nullptr;
			(void)vkQueueSubmit(context.getDevice()->getQueue(se::renderer::vulkan::QueueType::eGraphics), 1, &submitInfos, previousFrameReadyFence);

			/********************************************/
			/******** playing a bit with vulkan *********/
			/********************************************/


			bufferTransferor.sync();

			bool running {true};
			se::Count frame {0};
			while (true) {
				if (se::input::InputManager::wasKeyPressed(se::input::Key::eEscape) && se::input::InputManager::getFocusedWindowUUID() != 0) {
					se::window::WindowManager::destroyWindow(se::input::InputManager::getFocusedWindowUUID());
				}
				SE_APP_INFO("Frame {}", frame++);
				se::input::InputManager::update();
				if (!se::Engine::isRunning())
					break;

				if (se::input::InputManager::getFocusedWindowUUID() == window.getUUID()) {
					if (se::input::InputManager::isKeyDown(se::input::Key::eA))
						std::cout << "A" << std::endl;
					if (se::input::InputManager::wasKeyPressed(se::input::Key::eD))
						std::cout << "D" << std::endl;
					if (se::input::InputManager::wasKeyReleased(se::input::Key::eF))
						std::cout << "F" << std::endl;
				}

				if (se::input::InputManager::hasMouseMoved()) {
					//SE_APP_LOGGER.log({se::LogSeverity::eInfo}, "Mouse moved to ({}, {})", se::input::InputManager::getMousePosition().x,
					//	se::input::InputManager::getMousePosition().y);
				}

				/*if (se::input::InputManager::wasWindowResized(window2.getUUID()))
					SE_APP_LOGGER.log({se::LogSeverity::eInfo}, "Window2 resized to {}x{}", window2.getInfos().size.x, window2.getInfos().size.y);*/


				(void)vkWaitForFences(context.getDevice()->getDevice(), 1, &previousFrameReadyFence, VK_TRUE, UINT64_MAX);
				(void)vkResetFences(context.getDevice()->getDevice(), 1, &previousFrameReadyFence);

				(void)vkAcquireNextImageKHR(
					context.getDevice()->getDevice(),
					context.getSwapchain()->getSwapChain(),
					UINT64_MAX,
					imageReadySemaphore,
					VK_NULL_HANDLE,
					&imageIndex
				);

				(void)vkResetCommandBuffer(graphicsCommandBuffer, 0);


				VkRenderingAttachmentInfo renderingAttachmentInfos {};
				renderingAttachmentInfos.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
				renderingAttachmentInfos.clearValue.color = {{0.1f, 0.1f, 0.1f, 1.f}};
				renderingAttachmentInfos.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				renderingAttachmentInfos.imageView = context.getSwapchain()->getImageViews()[imageIndex];
				renderingAttachmentInfos.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				renderingAttachmentInfos.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				renderingAttachmentInfos.resolveMode = VK_RESOLVE_MODE_NONE;
				renderingAttachmentInfos.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				renderingAttachmentInfos.resolveImageView = VK_NULL_HANDLE;

				VkRenderingInfo renderingInfos {};
				renderingInfos.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
				renderingInfos.colorAttachmentCount = 1;
				renderingInfos.pColorAttachments = &renderingAttachmentInfos;
				renderingInfos.renderArea.offset = {0, 0};
				renderingInfos.renderArea.extent = context.getSwapchain()->getExtent();
				renderingInfos.layerCount = 1;
				renderingInfos.viewMask = 0;

				VkCommandBufferBeginInfo beginInfos {};
				beginInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfos.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				beginInfos.pInheritanceInfo = nullptr;
				(void)vkBeginCommandBuffer(graphicsCommandBuffer, &beginInfos);

				VkImageMemoryBarrier imageMemoryBarrier {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				imageMemoryBarrier.image = context.getSwapchain()->getImages()[imageIndex];
				imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageMemoryBarrier.subresourceRange.levelCount = 1;
				imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
				imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
				imageMemoryBarrier.subresourceRange.layerCount = 1;
				vkCmdPipelineBarrier(
					graphicsCommandBuffer,
					VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier
				);

				vkCmdBeginRendering(graphicsCommandBuffer, &renderingInfos);

				vkCmdBindPipeline(graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

				VkViewport viewport {};
				viewport.x = 0.f;
				viewport.y = 0.f;
				viewport.width = context.getSwapchain()->getExtent().width;
				viewport.height = context.getSwapchain()->getExtent().height;
				viewport.minDepth = 0.f;
				viewport.maxDepth = 1.f;
				vkCmdSetViewport(graphicsCommandBuffer, 0, 1, &viewport);

				VkRect2D scissor {};
				scissor.offset = {0, 0};
				scissor.extent = context.getSwapchain()->getExtent();
				vkCmdSetScissor(graphicsCommandBuffer, 0, 1, &scissor);

				VkBuffer buffer {vertexBuffer.getBuffer()};
				VkDeviceSize offset {vertexBufferView.getInfos().offset};
				vkCmdBindVertexBuffers(graphicsCommandBuffer, 0, 1, &buffer, &offset);

				vkCmdDraw(graphicsCommandBuffer, vertices.size() / 5, vertices.size() / 15, 0, 0);

				vkCmdEndRendering(graphicsCommandBuffer);

				imageMemoryBarrier = {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				imageMemoryBarrier.dstAccessMask = 0;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				imageMemoryBarrier.image = context.getSwapchain()->getImages()[imageIndex];
				imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageMemoryBarrier.subresourceRange.levelCount = 1;
				imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
				imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
				imageMemoryBarrier.subresourceRange.layerCount = 1;
				vkCmdPipelineBarrier(
					graphicsCommandBuffer,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
					0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier
				);

				(void)vkEndCommandBuffer(graphicsCommandBuffer);


				VkSubmitInfo submitInfos {};
				VkPipelineStageFlags dstStageMask {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
				submitInfos.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfos.commandBufferCount = 1;
				submitInfos.pCommandBuffers = &graphicsCommandBuffer;
				submitInfos.waitSemaphoreCount = 1;
				submitInfos.pWaitSemaphores = &imageReadySemaphore;
				submitInfos.pWaitDstStageMask = &dstStageMask;
				submitInfos.signalSemaphoreCount = 1;
				submitInfos.pSignalSemaphores = &imageDrawnSemaphore;
				(void)vkQueueSubmit(context.getDevice()->getQueue(se::renderer::vulkan::QueueType::eGraphics), 1, &submitInfos, previousFrameReadyFence);

				VkSwapchainKHR swapchain {context.getSwapchain()->getSwapChain()};
				VkPresentInfoKHR presentInfos {};
				presentInfos.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				presentInfos.swapchainCount = 1;
				presentInfos.pSwapchains = &swapchain;
				presentInfos.pImageIndices = &imageIndex;
				presentInfos.pResults = nullptr;
				presentInfos.waitSemaphoreCount = 1;
				presentInfos.pWaitSemaphores = &imageDrawnSemaphore;
				(void)vkQueuePresentKHR(context.getDevice()->getQueue(se::renderer::vulkan::QueueType::ePresent), &presentInfos);
			}

			vkDeviceWaitIdle(context.getDevice()->getDevice());


			vkDestroyFence(context.getDevice()->getDevice(), previousFrameReadyFence, nullptr);
			vkDestroySemaphore(context.getDevice()->getDevice(), imageDrawnSemaphore, nullptr);
			vkDestroySemaphore(context.getDevice()->getDevice(), imageReadySemaphore, nullptr);

			vkFreeCommandBuffers(context.getDevice()->getDevice(), graphicsCommandPool, 1, &graphicsCommandBuffer);
			vkDestroyCommandPool(context.getDevice()->getDevice(), graphicsCommandPool, nullptr);
		}
};


se::Application *createApplication(const std::vector<std::string> &/*args*/) {
	return new SandboxApp();
}