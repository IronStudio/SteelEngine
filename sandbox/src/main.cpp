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

			windowInfos.title = "2nd";
			windowInfos.flags = se::window::WindowFlags::eResizable;
			windowInfos.minSize = {16 * 10, 9 * 10};
			windowInfos.maxSize = {16 * 100, 9 * 100};
			se::window::Window &window2 {se::window::WindowManager::createWindow(windowInfos)};


			/** @brief Context */
			se::renderer::ContextInfos contextInfos {};
			contextInfos.applicationName = "SteelEngine_sandbox";
			contextInfos.applicationVersion = "1.0.0"_v;
			contextInfos.preferredGPU = se::renderer::GPUType::eDiscret;
			contextInfos.linkedWindow = &window;
			se::renderer::vulkan::Context context {contextInfos};


			/** @brief Staging memory */
			se::renderer::VramAllocatorInfos allocatorInfos {};
			allocatorInfos.chunkSize = 256_MiB;
			allocatorInfos.context = &context;
			allocatorInfos.usageNature = se::renderer::VramUsageNature::eCpuToGpu;
			allocatorInfos.usageFrequency = se::renderer::VramUsageFrequency::eDynamic;
			se::renderer::vulkan::VramAllocator stagingAllocator {allocatorInfos};

			stagingAllocator.logAllocationTable();

			/** @brief Staging buffer */
			se::renderer::BufferInfos bufferInfos {};
			bufferInfos.context = &context;
			bufferInfos.allocator = &stagingAllocator;
			bufferInfos.usage = se::renderer::BufferUsage::eTransferSrc;
			bufferInfos.size = 10_MiB;
			se::renderer::vulkan::Buffer stagingBuffer {bufferInfos};

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
			bufferInfos.size = 10_MiB;
			se::renderer::vulkan::Buffer vertexBuffer {bufferInfos};


			/** @brief VB view */
			se::renderer::VertexBufferViewInfos vertexBufferViewInfos {};
			vertexBufferViewInfos.context = &context;
			vertexBufferViewInfos.attributes = {
				{se::renderer::VertexType::eFloat32, 0, 2, 0}
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
			se::renderer::vulkan::Pipeline pipeline {pipelineInfos};



			/********************************************/
			/******** playing a bit with vulkan *********/
			/********************************************/


			VkRenderingAttachmentInfo renderingAttachmentInfos {};
			renderingAttachmentInfos.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			renderingAttachmentInfos.clearValue.color = {0.f, 0.f, 0.f, 1.f};
			renderingAttachmentInfos.clearValue.depthStencil = {0.f, 0};
			renderingAttachmentInfos.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			renderingAttachmentInfos.imageView = context.getSwapchain()->getImageViews()[0];
			renderingAttachmentInfos.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			renderingAttachmentInfos.resolveImageView = VK_NULL_HANDLE;
			renderingAttachmentInfos.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			renderingAttachmentInfos.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			renderingAttachmentInfos.resolveMode = VK_RESOLVE_MODE_NONE;

			VkRenderingInfo renderingInfos {};
			renderingInfos.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
			renderingInfos.colorAttachmentCount = 1;
			renderingInfos.pColorAttachments = &renderingAttachmentInfos;
			renderingInfos.renderArea.offset = {0, 0};
			renderingInfos.renderArea.extent = context.getSwapchain()->getExtent();
			renderingInfos.layerCount = 1;
			renderingInfos.viewMask = 0;


			/********************************************/
			/******** playing a bit with vulkan *********/
			/********************************************/



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
		}
};


se::Application *createApplication(const std::vector<std::string> &/*args*/) {
	return new SandboxApp();
}