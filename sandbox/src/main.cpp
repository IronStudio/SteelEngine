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
#include <se/renderer/vulkan/pipeline.hpp>
#include <se/renderer/vulkan/renderPass.hpp>
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


			se::window::WindowInfos windowInfos {};
			windowInfos.title = "SteelEngine_sandbox";
			windowInfos.size = {16 * 70, 9 * 70};
			windowInfos.position = {se::window::centerPosition, se::window::undefinedPosition};
			windowInfos.graphicsApi = se::renderer::GraphicsApi::eVulkan;
			windowInfos.flags = se::window::WindowFlags::eResizable | se::window::WindowFlags::eMasterWindow;
			se::window::Window &window {se::window::WindowManager::createWindow(windowInfos)};

			windowInfos.title = "2nd";
			windowInfos.flags = se::window::WindowFlags::eResizable;
			windowInfos.minSize = {16 * 10, 9 * 10};
			windowInfos.maxSize = {16 * 100, 9 * 100};
			se::window::Window &window2 {se::window::WindowManager::createWindow(windowInfos)};


			se::renderer::ContextInfos contextInfos {};
			contextInfos.applicationName = "SteelEngine_sandbox";
			contextInfos.applicationVersion = "1.0.0"_v;
			contextInfos.preferredGPU = se::renderer::GPUType::eDiscret;
			contextInfos.linkedWindow = &window;
			se::renderer::vulkan::Context context {contextInfos};

			se::renderer::VramAllocatorInfos allocatorInfos {};
			allocatorInfos.chunkSize = 256_MiB;
			allocatorInfos.context = &context;
			allocatorInfos.usageNature = se::renderer::VramUsageNature::eCpuToGpu;
			allocatorInfos.usageFrequency = se::renderer::VramUsageFrequency::eDynamic;
			se::renderer::vulkan::VramAllocator allocator {allocatorInfos};

			allocator.logAllocationTable();

			se::renderer::BufferInfos bufferInfos {};
			bufferInfos.context = &context;
			bufferInfos.allocator = &allocator;
			bufferInfos.usage = se::renderer::BufferUsage::eVertex;
			bufferInfos.size = 10_MiB;
			se::renderer::vulkan::Buffer buffer {bufferInfos};

			allocator.logAllocationTable();

			se::renderer::VertexBufferViewInfos vertexBufferViewInfos {};
			vertexBufferViewInfos.context = &context;
			vertexBufferViewInfos.attributes = {
				{se::renderer::VertexType::eFloat32, 0, 2, 0}
			};
			se::renderer::vulkan::VertexBufferView vertexBufferView {vertexBufferViewInfos};


			se::renderer::ShaderInfos shaderInfos {};
			shaderInfos.context = &context;
			shaderInfos.entryPoint = "main";

			shaderInfos.type = se::renderer::ShaderType::eVertex;
			shaderInfos.file = "shaders/test.vert";
			se::renderer::vulkan::Shader vertexShader {shaderInfos};

			shaderInfos.type = se::renderer::ShaderType::eFragment;
			shaderInfos.file = "shaders/test.frag";
			se::renderer::vulkan::Shader fragmentShader {shaderInfos};


			se::renderer::RenderPassInfos renderPassInfos {};
			renderPassInfos.context = &context;
			se::renderer::vulkan::RenderPass renderPass {renderPassInfos};


			se::renderer::PipelineInfos pipelineInfos {};
			pipelineInfos.context = &context;
			pipelineInfos.vertexBufferView = &vertexBufferView;
			pipelineInfos.shaders = {&vertexShader, &fragmentShader};
			pipelineInfos.renderPass = &renderPass;
			se::renderer::vulkan::Pipeline pipeline {pipelineInfos};

			std::vector<VkFramebuffer> framebuffers {};
			framebuffers.reserve(context.getSwapchain()->getImageViews().size());

			VkFramebufferCreateInfo framebufferCreateInfos {};
			framebufferCreateInfos.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfos.renderPass = renderPass.getRenderPass();
			framebufferCreateInfos.attachmentCount = 1;
			framebufferCreateInfos.width = context.getSwapchain()->getExtent().width;
			framebufferCreateInfos.height = context.getSwapchain()->getExtent().height;
			framebufferCreateInfos.layers = 1;

			for (const auto &imageView : context.getSwapchain()->getImageViews()) {
				framebufferCreateInfos.pAttachments = &imageView;

				VkFramebuffer framebuffer {VK_NULL_HANDLE};
				if (vkCreateFramebuffer(context.getDevice()->getDevice(), &framebufferCreateInfos, nullptr, &framebuffer) != VK_SUCCESS)
					throw se::exceptions::RuntimeError("Can't create framebuffer");
				framebuffers.push_back(framebuffer);
			}


			VkCommandPoolCreateInfo commandPoolCreateInfos {};
			commandPoolCreateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			commandPoolCreateInfos.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			commandPoolCreateInfos.queueFamilyIndex
				= context.getDevice()->getQueues().find(se::renderer::vulkan::QueueType::eGraphics)->second.begin()->first;

			VkCommandPool commandPool {VK_NULL_HANDLE};
			if (vkCreateCommandPool(context.getDevice()->getDevice(), &commandPoolCreateInfos, nullptr, &commandPool) != VK_SUCCESS)
				throw se::exceptions::RuntimeError("Can't create command pool");

			se::Uint32 imageIndex {0};


			VkCommandBufferAllocateInfo commandBufferAllocateInfos {};
			commandBufferAllocateInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			commandBufferAllocateInfos.commandBufferCount = 1;
			commandBufferAllocateInfos.commandPool = commandPool;
			commandBufferAllocateInfos.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			VkCommandBuffer commandBuffer {VK_NULL_HANDLE};


			VkCommandBufferBeginInfo commandBufferBeginInfos {};
			commandBufferBeginInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			commandBufferBeginInfos.pInheritanceInfo = nullptr;
			(void)vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfos);

			VkClearValue clearColor {{{0, 0, 0, 0}}};

			VkRenderPassBeginInfo renderPassBeginInfos {};
			renderPassBeginInfos.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfos.renderPass = renderPass.getRenderPass();
			renderPassBeginInfos.framebuffer = framebuffers[imageIndex];
			renderPassBeginInfos.renderArea.offset = {0, 0};
			renderPassBeginInfos.renderArea.extent = context.getSwapchain()->getExtent();
			renderPassBeginInfos.clearValueCount = 1;
			renderPassBeginInfos.pClearValues = &clearColor;
			vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfos, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

			VkViewport viewport {};
			viewport.x = 0.f;
			viewport.y = 0.f;
			viewport.width = static_cast<se::Float> (context.getSwapchain()->getExtent().width);
			viewport.height = static_cast<se::Float> (context.getSwapchain()->getExtent().height);
			viewport.minDepth = 0.f;
			viewport.maxDepth = 1.f;
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor {};
			scissor.offset = {0, 0};
			scissor.extent = context.getSwapchain()->getExtent();
			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

			vkCmdDraw(commandBuffer, 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffer);
			(void)vkEndCommandBuffer(commandBuffer);


			bool running {true};
			while (se::Engine::isRunning()) {
				se::input::InputManager::update();

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

				if (se::input::InputManager::wasKeyPressed(se::input::Key::eEscape) && se::input::InputManager::getFocusedWindowUUID() != 0) {
					se::window::WindowManager::destroyWindow(se::input::InputManager::getFocusedWindowUUID());
				}

				if (se::input::InputManager::wasWindowResized(window2.getUUID()))
					SE_APP_LOGGER.log({se::LogSeverity::eInfo}, "Window2 resized to {}x{}", window2.getInfos().size.x, window2.getInfos().size.y);
			}


			vkDestroyCommandPool(context.getDevice()->getDevice(), commandPool, nullptr);

			for (const auto &framebuffer : framebuffers)
				vkDestroyFramebuffer(context.getDevice()->getDevice(), framebuffer, nullptr);
		}
};


se::Application *createApplication(const std::vector<std::string> &/*args*/) {
	return new SandboxApp();
}