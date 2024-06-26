#include <fstream>
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
#include <se/units.hpp>
#include <se/utils/converter.hpp>
#include <se/utils/version.hpp>
#include <se/window/windowManager.hpp>
#include <se/resourceManager.hpp>
#include <se/renderer/renderer.hpp>

#include <se/renderer/vulkan/attributeBufferView.hpp>
#include <se/renderer/vulkan/buffer.hpp>
#include <se/renderer/vulkan/commandBuffer.hpp>
#include <se/renderer/vulkan/context.hpp>
#include <se/renderer/vulkan/imageBuffer.hpp>
#include <se/renderer/vulkan/format.hpp>
#include <se/renderer/vulkan/imageLayoutTransfer.hpp>
#include <se/renderer/vulkan/pipeline.hpp>
#include <se/renderer/vulkan/shader.hpp>
#include <se/renderer/vulkan/synchronisation.hpp>
#include <se/renderer/vulkan/vertexBufferView.hpp>
#include <se/renderer/vulkan/vramAllocator.hpp>

#include <se/renderer/opengl/context.hpp>
#include <se/renderer/opengl/vramAllocator.hpp>


using namespace se::literals;


class SandboxApp : public se::Application {
	public:
		std::ofstream engineLogFile {"engine.log"};
		std::ofstream appLogFile {"app.log"};

		SandboxApp() :
			se::Application()
		{
			m_infos.name = "SteelEngine_sandbox";
			m_infos.version = "1.0.0"_v;
		}

		~SandboxApp() override {}

		void run() override {
			SE_LOGGER.setSeverityMask(~se::LogSeverity::eVerbose);
			/*SE_LOGGER.setOutputStream(&engineLogFile);
			SE_APP_LOGGER.setOutputStream(&appLogFile);*/

			/** @brief Window */
			se::window::WindowInfos windowInfos {};
			windowInfos.title = m_infos.name;
			windowInfos.size = {16 * 70, 9 * 70};
			windowInfos.position = {se::window::centerPosition, se::window::undefinedPosition};
			windowInfos.graphicsApi = se::renderer::GraphicsApi::eVulkan;
			windowInfos.flags = se::window::WindowFlags::eResizable | se::window::WindowFlags::eMasterWindow;
			se::window::Window &window {se::window::WindowManager::createWindow(windowInfos)};


			/** @brief Renderer */
			se::renderer::RendererInfos rendererInfos {};
			rendererInfos.api = se::renderer::GraphicsApi::eVulkan;
			rendererInfos.application = this;
			rendererInfos.window = &window;
			se::renderer::Renderer renderer {rendererInfos};
			auto &context {*reinterpret_cast<se::renderer::vulkan::Context*> (renderer.getContext())};


			/** @brief Vertices */
			std::vector<se::Float> vertices {
				0.f, 0.f, 0.f,    1.f, 0.f, 0.f, 1.f,
				1.f, 0.f, 0.f,    0.f, 1.f, 0.f, 1.f
			};

			SE_APP_INFO("Vertices count {}", vertices.size() / 7);

			/** @brief PerInstanceDatas */
			std::vector<se::Float> perInstanceDatas {
				0.f,  0.f, 1.f
				//0.5f, 1.f, 0.5f
			};


			/** @brief UB view */
			se::ResourceManager::RendererAttributeBufferView attributeBufferViewInfos {};
			attributeBufferViewInfos.context = &context;
			attributeBufferViewInfos.usage = se::renderer::BufferViewUsage::eUniform;
			attributeBufferViewInfos.shaderTypes = se::renderer::ShaderType::eVertex | se::renderer::ShaderType::eGeometry;
			attributeBufferViewInfos.binding = 0;
			attributeBufferViewInfos.attributes = {
				{"colorBias", se::renderer::AttributeType::eFloat32},
				{"camera", se::renderer::AttributeType::eMat4}
			};
			attributeBufferViewInfos.offset = 0;
			auto uniformBufferViewResource {se::ResourceManager::load(attributeBufferViewInfos)};
			auto &uniformBufferView {*reinterpret_cast<se::renderer::vulkan::AttributeBufferView*> (uniformBufferViewResource.res)};


			/** @brief Staging memory */
			se::renderer::VramAllocatorInfos allocatorInfos {};
			allocatorInfos.chunkSize = 64_MiB;
			allocatorInfos.context = &context;
			allocatorInfos.usageNature = se::renderer::VramUsageNature::eCpuToGpu;
			allocatorInfos.usageFrequency = se::renderer::VramUsageFrequency::eForgotten;
			se::renderer::vulkan::VramAllocator stagingAllocator {allocatorInfos};

			stagingAllocator.logAllocationTable();

			/** @brief Staging buffers */
			se::renderer::BufferInfos bufferInfos {};
			bufferInfos.context = &context;
			bufferInfos.allocator = &stagingAllocator;
			bufferInfos.usage = se::renderer::BufferUsage::eTransferSrc;
			bufferInfos.size = vertices.size() * sizeof(se::Float);
			se::renderer::vulkan::Buffer stagingBuffer {bufferInfos};

			bufferInfos.context = &context;
			bufferInfos.allocator = &stagingAllocator;
			bufferInfos.usage = se::renderer::BufferUsage::eTransferSrc;
			bufferInfos.size = perInstanceDatas.size() * sizeof(se::Float);
			se::renderer::vulkan::Buffer perInstanceStagingBuffer {bufferInfos};

			bufferInfos.context = &context;
			bufferInfos.allocator = &stagingAllocator;
			bufferInfos.usage = se::renderer::BufferUsage::eTransferSrc;
			bufferInfos.size = uniformBufferView.getTotalSize();
			se::renderer::vulkan::Buffer uniformStagingBuffer {bufferInfos};

			se::renderer::BufferWriteInfos bufferWriteInfos {};
			bufferWriteInfos.offset = 0;
			bufferWriteInfos.value.assign((se::Byte*)vertices.data(), (se::Byte*)(vertices.data() + vertices.size()));
			stagingBuffer.write(bufferWriteInfos);

			bufferWriteInfos.offset = 0;
			bufferWriteInfos.value.assign((se::Byte*)perInstanceDatas.data(), (se::Byte*)(perInstanceDatas.data() + perInstanceDatas.size()));
			perInstanceStagingBuffer.write(bufferWriteInfos);

			se::Float32 fov {(se::Float32)90.0_deg};
			se::Float32 ratio {(se::Float32)context.getSwapchain()->getExtent().width / (se::Float32)context.getSwapchain()->getExtent().height};
			se::Float32 near {0.01f};
			se::Float32 far {100.f};
			se::Float32 right {near * tanf(fov/2.f)};
			right = right >= 0 ? right : -right;
			se::Float32 bottom {right / ratio};

			SE_APP_INFO("fov {}, near {}, far {}, right {}, bottom {}", fov, near, far, right, bottom);

			se::Vec2f cameraOrientation {se::pi<se::Float32> / 4.f, se::pi<se::Float32> / 4.f};
			se::Vec3f cameraPosition {-2.f, 2.f, -2.f};

			se::Float32 colorBias {1.f};
			se::Mat4f position {
				1.f, 0.f, 0.f, -cameraPosition.x,
				0.f, 1.f, 0.f, -cameraPosition.y,
				0.f, 0.f, 1.f, -cameraPosition.z,
				0.f, 0.f, 0.f, 1.f
			};
			se::Mat4f rotation1 {
				cosf(cameraOrientation.x), 0.f, -sinf(cameraOrientation.x), 0.f,
				0.f, 1.f, 0.f, 0.f,
				sinf(cameraOrientation.x), 0.f, cosf(cameraOrientation.x), 0.f,
				0.f, 0.f, 0.f, 1.f
			};
			se::Mat4f rotation2 {
				1.f, 0.f, 0.f, 0.f,
				0.f, cosf(cameraOrientation.y), sinf(cameraOrientation.y), 0.f,
				0.f, -sinf(cameraOrientation.y), cosf(cameraOrientation.y), 0.f,
				0.f, 0.f, 0.f, 1.f
			};
			se::Mat4f camera {
				near/right, 0.f,         0.f,            0.f,
				0.f,        near/bottom, 0.f,            0.f,
				0.f,        0.f,         1/(far-near), -near/(far-near),
				0.f,        0.f,         1.f,            0.f
			};
			camera = camera * rotation2 * rotation1 * position;
			SE_APP_LOGGER << se::LogInfos(se::LogSeverity::eInfo) << camera << se::endLog;
			se::renderer::BufferWriteAttributeInfos attributeBufferWriteInfos {};
			attributeBufferWriteInfos.offset = 0;
			attributeBufferWriteInfos.attributeBufferView = &uniformBufferView;
			attributeBufferWriteInfos.attributes = {
				{"colorBias",  se::utils::vectorize(colorBias)},
				{"camera",     se::utils::vectorize(camera)}
			};
			uniformStagingBuffer.write(attributeBufferWriteInfos);


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

			/** @brief Per instance data buffer */
			bufferInfos.context = &context;
			bufferInfos.allocator = &gpuAllocator;
			bufferInfos.usage = se::renderer::BufferUsage::eVertex | se::renderer::BufferUsage::eTransferDst;
			bufferInfos.size = perInstanceDatas.size() * sizeof(se::Float);
			se::renderer::vulkan::Buffer perInstanceBuffer {bufferInfos};

			/** @brief Uniform buffer */
			bufferInfos.context = &context;
			bufferInfos.allocator = &gpuAllocator;
			bufferInfos.usage = se::renderer::BufferUsage::eUniform | se::renderer::BufferUsage::eTransferDst;
			bufferInfos.size = uniformBufferView.getTotalSize();
			se::renderer::vulkan::Buffer uniformBuffer {bufferInfos};
			

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

			bufferTransferInfos.source = &perInstanceStagingBuffer;
			bufferTransferInfos.destination = &perInstanceBuffer;
			bufferTransferInfos.srcOffset = 0;
			bufferTransferInfos.dstOffset = 0;
			bufferTransferInfos.size = perInstanceDatas.size() * sizeof(se::Float);
			bufferTransferor.transfer(bufferTransferInfos);

			bufferTransferInfos.source = &uniformStagingBuffer;
			bufferTransferInfos.destination = &uniformBuffer;
			bufferTransferInfos.srcOffset = 0;
			bufferTransferInfos.dstOffset = 0;
			bufferTransferInfos.size = uniformBufferView.getTotalSize();
			bufferTransferor.transfer(bufferTransferInfos);


			/** @brief Depth buffer */
			se::renderer::ImageBufferInfos imageBufferInfos {};
			imageBufferInfos.context = &context;
			imageBufferInfos.allocator = &gpuAllocator;
			imageBufferInfos.format = se::renderer::Format::eD32;
			imageBufferInfos.size = {context.getSwapchain()->getExtent().width, context.getSwapchain()->getExtent().height};
			imageBufferInfos.usage = se::renderer::ImageBufferUsage::eDepthAttachment;
			se::renderer::vulkan::ImageBuffer depthBuffer {imageBufferInfos};

			/** @brief VB view */
			se::renderer::VertexBufferViewInfos vertexBufferViewInfos {};
			vertexBufferViewInfos.context = &context;
			vertexBufferViewInfos.attributes = {
				{se::renderer::VertexType::eFloat32, 0, 3, 0},
				{se::renderer::VertexType::eFloat32, 1, 4, 0},
				//{se::renderer::VertexType::eFloat32, 2, 1, 1, se::renderer::VertexRate::eInstance},
				//{se::renderer::VertexType::eFloat32, 3, 1, 1, se::renderer::VertexRate::eInstance},
				//{se::renderer::VertexType::eFloat32, 4, 1, 1, se::renderer::VertexRate::eInstance}
			};
			se::renderer::vulkan::VertexBufferView vertexBufferView {vertexBufferViewInfos};


			/** @brief Shaders */
			se::renderer::ShaderInfos shaderInfos {};
			shaderInfos.context = &context;
			shaderInfos.entryPoint = "main";

			shaderInfos.type = se::renderer::ShaderType::eVertex;
			shaderInfos.file = "shaders/test.vert";
			se::renderer::vulkan::Shader vertexShader {shaderInfos};

			shaderInfos.type = se::renderer::ShaderType::eGeometry;
			shaderInfos.file = "shaders/test.geom";
			se::renderer::vulkan::Shader geometryShader {shaderInfos};

			shaderInfos.type = se::renderer::ShaderType::eFragment;
			shaderInfos.file = "shaders/test.frag";
			se::renderer::vulkan::Shader fragmentShader {shaderInfos};


			/** @brief Pipeline */
			se::renderer::PipelineInfos pipelineInfos {};
			pipelineInfos.context = &context;
			pipelineInfos.vertexBufferView = &vertexBufferView;
			pipelineInfos.shaders = {&vertexShader, &geometryShader, &fragmentShader};
			pipelineInfos.colorAttachmentFormats = {se::renderer::vulkan::formatVkToSe(context.getSwapchain()->getFormat().format)};
			pipelineInfos.depthAttachmentFormat = depthBuffer.getInfos().format;
			pipelineInfos.stencilAttachmentFormat = se::renderer::Format::eNone;
			pipelineInfos.blendMode = se::renderer::BlendMode::eSrcAlpha;
			pipelineInfos.attributeBufferView = {&uniformBufferView};
			se::renderer::vulkan::Pipeline pipeline {pipelineInfos};


			/********************************************/
			/******** playing a bit with vulkan *********/
			/********************************************/


			se::renderer::vulkan::CommandBufferInfos commandBufferInfos {};
			commandBufferInfos.context = &context;
			commandBufferInfos.queue = se::renderer::vulkan::QueueType::eGraphics;
			se::renderer::vulkan::CommandBuffer graphicsCommandBuffer {commandBufferInfos};


			uniformBufferView.setBuffer(&uniformBuffer);
			pipeline.updateBuffer();


			se::Uint32 imageIndex {};

			se::renderer::vulkan::SemaphoreInfos semaphoreInfos {};
			semaphoreInfos.context = &context;
			se::renderer::vulkan::Semaphore imageReadySemaphore {semaphoreInfos};
			se::renderer::vulkan::Semaphore imageDrawnSemaphore {semaphoreInfos};

			se::renderer::vulkan::FenceInfos fenceInfos {};
			fenceInfos.context = &context;
			fenceInfos.signaled = false;
			se::renderer::vulkan::Fence previousFrameReadyFence {fenceInfos};


			VkCommandBufferBeginInfo beginInfos {};
			beginInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfos.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			beginInfos.pInheritanceInfo = nullptr;
			(void)vkBeginCommandBuffer(graphicsCommandBuffer.getCommandBuffer(), &beginInfos);

			for (se::Count i {0}; i < context.getSwapchain()->getImages().size(); ++i) {
				se::renderer::vulkan::ImageLayoutTransferInfos transferInfos {};
				transferInfos.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
				transferInfos.commandBuffer = graphicsCommandBuffer.getCommandBuffer();
				transferInfos.srcStage = VK_PIPELINE_STAGE_HOST_BIT;
				transferInfos.dstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				transferInfos.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transferInfos.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				transferInfos.image = context.getSwapchain()->getImages()[i];
				transferInfos.dstAccessMask = 0;
				transferInfos.srcAccessMask = 0;
				se::renderer::vulkan::transferImageLayout(transferInfos);
			}

			{
				se::renderer::vulkan::ImageLayoutTransferInfos transferInfos {};
				transferInfos.aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
				transferInfos.commandBuffer = graphicsCommandBuffer.getCommandBuffer();
				transferInfos.srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				transferInfos.dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				transferInfos.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				transferInfos.newLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
				transferInfos.image = depthBuffer.getImage();
				transferInfos.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				transferInfos.srcAccessMask = 0;
				se::renderer::vulkan::transferImageLayout(transferInfos);
			}

			(void)vkEndCommandBuffer(graphicsCommandBuffer.getCommandBuffer());

			VkSubmitInfo submitInfos {};
			VkPipelineStageFlags dstStageMask {VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT};
			submitInfos.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfos.commandBufferCount = 1;
			submitInfos.pCommandBuffers = &graphicsCommandBuffer.getCommandBuffer();
			submitInfos.waitSemaphoreCount = 0;
			submitInfos.pWaitSemaphores = nullptr;
			submitInfos.pWaitDstStageMask = &dstStageMask;
			submitInfos.signalSemaphoreCount = 0;
			submitInfos.pSignalSemaphores = nullptr;
			(void)vkQueueSubmit(context.getDevice()->getQueue(se::renderer::vulkan::QueueType::eGraphics),
				1, &submitInfos, previousFrameReadyFence.getFence()
			);

			/********************************************/
			/******** playing a bit with vulkan *********/
			/********************************************/


			bufferTransferor.sync();
			se::Float32 sensibility {0.025f};

			SDL_SetRelativeMouseMode(SDL_TRUE);

			while (true) {
				if (se::input::InputManager::wasKeyPressed(se::input::Key::eEscape) && se::input::InputManager::getFocusedWindowUUID() != 0) {
					se::window::WindowManager::destroyWindow(se::input::InputManager::getFocusedWindowUUID());
				}
				se::input::InputManager::update();
				if (!se::Engine::isRunning())
					break;

				if (se::input::InputManager::getFocusedWindowUUID() == window.getUUID()) {
					if (se::input::InputManager::hasMouseMoved()) {
						cameraOrientation.x += se::input::InputManager::getMouseMotion().x * sensibility;
						cameraOrientation.y += se::input::InputManager::getMouseMotion().y * sensibility;
						if (cameraOrientation.y >= se::pi<se::Float32> / 2.f)
							cameraOrientation.y = se::pi<se::Float32> / 2.f - 0.000001f;
						if (cameraOrientation.y <= -se::pi<se::Float32> / 2.f)
							cameraOrientation.y = -se::pi<se::Float32> / 2.f + 0.000001f;
					}

					if (se::input::InputManager::isKeyDown(se::input::Key::eQ))
						sensibility += 0.001f;
					if (se::input::InputManager::isKeyDown(se::input::Key::eE))
						sensibility -= 0.001f;

					se::Vec3f direction {
						cosf(cameraOrientation.y) * sinf(cameraOrientation.x),
						-sinf(cameraOrientation.y),
						cosf(cameraOrientation.y) * cosf(cameraOrientation.x),
					};
					se::Vec3f normal {se::cross(direction, se::Vec3f(0.f, 1.f, 0.f))};
					normal *= 1.f / se::length(normal);

					if (se::input::InputManager::isKeyDown(se::input::Key::eW))
						cameraPosition += direction * 0.04f;
					if (se::input::InputManager::isKeyDown(se::input::Key::eS))
						cameraPosition -= direction * 0.04f;
					if (se::input::InputManager::isKeyDown(se::input::Key::eD))
						cameraPosition -= normal * 0.04f;
					if (se::input::InputManager::isKeyDown(se::input::Key::eA))
						cameraPosition += normal * 0.04f;
				}

				position = {
					1.f, 0.f, 0.f, -cameraPosition.x,
					0.f, 1.f, 0.f, -cameraPosition.y,
					0.f, 0.f, 1.f, -cameraPosition.z,
					0.f, 0.f, 0.f, 1.f
				};
				rotation1 = {
					cosf(cameraOrientation.x), 0.f, -sinf(cameraOrientation.x), 0.f,
					0.f, 1.f, 0.f, 0.f,
					sinf(cameraOrientation.x), 0.f, cosf(cameraOrientation.x), 0.f,
					0.f, 0.f, 0.f, 1.f
				};
				rotation2 = {
					1.f, 0.f, 0.f, 0.f,
					0.f, cosf(cameraOrientation.y), sinf(cameraOrientation.y), 0.f,
					0.f, -sinf(cameraOrientation.y), cosf(cameraOrientation.y), 0.f,
					0.f, 0.f, 0.f, 1.f
				};
				camera = {
					near/right, 0.f,         0.f,            0.f,
					0.f,        near/bottom, 0.f,            0.f,
					0.f,        0.f,         far/(far-near), -far*near/(far-near),
					0.f,        0.f,         1.f,            0.f
				};
				camera = camera * rotation2 * rotation1 * position;
				se::renderer::BufferWriteAttributeInfos attributeBufferWriteInfos {};
				attributeBufferWriteInfos.offset = 0;
				attributeBufferWriteInfos.attributeBufferView = &uniformBufferView;
				attributeBufferWriteInfos.attributes = {
					{"colorBias",  se::utils::vectorize(colorBias)},
					{"camera",     se::utils::vectorize(camera)}
				};
				uniformStagingBuffer.write(attributeBufferWriteInfos);

				bufferTransferInfos.source = &uniformStagingBuffer;
				bufferTransferInfos.destination = &uniformBuffer;
				bufferTransferInfos.srcOffset = 0;
				bufferTransferInfos.dstOffset = 0;
				bufferTransferInfos.size = uniformBufferView.getTotalSize();
				bufferTransferor.transfer(bufferTransferInfos);
				bufferTransferor.sync();



				previousFrameReadyFence.sync(SE_NEVER_TIMEOUT);
				previousFrameReadyFence.reset();

				(void)vkAcquireNextImageKHR(
					context.getDevice()->getDevice(),
					context.getSwapchain()->getSwapChain(),
					UINT64_MAX,
					imageReadySemaphore.getSemaphore(),
					VK_NULL_HANDLE,
					&imageIndex
				);

				renderer.render(
					previousFrameReadyFence,
					imageReadySemaphore,
					imageDrawnSemaphore,
					context.getSwapchain()->getImages()[imageIndex]
				);

				VkSwapchainKHR swapchain {context.getSwapchain()->getSwapChain()};
				VkPresentInfoKHR presentInfos {};
				presentInfos.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				presentInfos.swapchainCount = 1;
				presentInfos.pSwapchains = &swapchain;
				presentInfos.pImageIndices = &imageIndex;
				presentInfos.pResults = nullptr;
				presentInfos.waitSemaphoreCount = 1;
				presentInfos.pWaitSemaphores = &imageDrawnSemaphore.getSemaphore();
				(void)vkQueuePresentKHR(context.getDevice()->getQueue(se::renderer::vulkan::QueueType::ePresent), &presentInfos);

				/*(void)vkResetCommandBuffer(graphicsCommandBuffer.getCommandBuffer(), 0);


				VkRenderingAttachmentInfo colorAttachmentInfos {};
				colorAttachmentInfos.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
				colorAttachmentInfos.clearValue.color = {{0.1f, 0.1f, 0.1f, 1.f}};
				colorAttachmentInfos.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				colorAttachmentInfos.imageView = context.getSwapchain()->getImageViews()[imageIndex];
				colorAttachmentInfos.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				colorAttachmentInfos.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachmentInfos.resolveMode = VK_RESOLVE_MODE_NONE;
				colorAttachmentInfos.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachmentInfos.resolveImageView = VK_NULL_HANDLE;

				VkRenderingAttachmentInfo depthAttachmentInfos {};
				depthAttachmentInfos.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
				depthAttachmentInfos.clearValue.depthStencil = {1.f, 0};
				depthAttachmentInfos.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
				depthAttachmentInfos.imageView = depthBuffer.getImageView();
				depthAttachmentInfos.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				depthAttachmentInfos.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				depthAttachmentInfos.resolveMode = VK_RESOLVE_MODE_NONE;
				depthAttachmentInfos.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				depthAttachmentInfos.resolveImageView = VK_NULL_HANDLE;

				VkRenderingInfo renderingInfos {};
				renderingInfos.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
				renderingInfos.colorAttachmentCount = 1;
				renderingInfos.pColorAttachments = &colorAttachmentInfos;
				renderingInfos.renderArea.offset = {0, 0};
				renderingInfos.renderArea.extent = context.getSwapchain()->getExtent();
				renderingInfos.layerCount = 1;
				renderingInfos.viewMask = 0;
				renderingInfos.pDepthAttachment = &depthAttachmentInfos;

				VkCommandBufferBeginInfo beginInfos {};
				beginInfos.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfos.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				beginInfos.pInheritanceInfo = nullptr;
				(void)vkBeginCommandBuffer(graphicsCommandBuffer.getCommandBuffer(), &beginInfos);

				se::renderer::vulkan::ImageLayoutTransferInfos transferInfos {};
				transferInfos.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
				transferInfos.commandBuffer = graphicsCommandBuffer.getCommandBuffer();
				transferInfos.srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				transferInfos.dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				transferInfos.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				transferInfos.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transferInfos.image = context.getSwapchain()->getImages()[imageIndex];
				transferInfos.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				transferInfos.srcAccessMask = 0;
				se::renderer::vulkan::transferImageLayout(transferInfos);

				vkCmdBeginRendering(graphicsCommandBuffer.getCommandBuffer(), &renderingInfos);

				vkCmdBindPipeline(graphicsCommandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipeline());

				VkViewport viewport {};
				viewport.x = 0.f;
				viewport.y = context.getSwapchain()->getExtent().height;
				viewport.width = context.getSwapchain()->getExtent().width;
				viewport.height = -(se::Float32)context.getSwapchain()->getExtent().height;
				viewport.minDepth = 0.f;
				viewport.maxDepth = 1.f;
				vkCmdSetViewport(graphicsCommandBuffer.getCommandBuffer(), 0, 1, &viewport);

				VkRect2D scissor {};
				scissor.offset = {0, 0};
				scissor.extent = context.getSwapchain()->getExtent();
				vkCmdSetScissor(graphicsCommandBuffer.getCommandBuffer(), 0, 1, &scissor);

				VkBuffer buffers[2] {vertexBuffer.getBuffer(), perInstanceBuffer.getBuffer()};
				VkDeviceSize offsets[2] {vertexBufferView.getInfos().offset, vertexBufferView.getInfos().offset};
				vkCmdBindVertexBuffers(graphicsCommandBuffer.getCommandBuffer(), 0, 2, buffers, offsets);

				vkCmdBindDescriptorSets(
					graphicsCommandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
					pipeline.getPipelineLayout(), 0, 1, &pipeline.getDescriptorSet(), 0, nullptr
				);

				vkCmdDraw(graphicsCommandBuffer.getCommandBuffer(), vertices.size() / 7, /*perInstanceDatas.size() / 3 * / 1, 0, 0);

				vkCmdEndRendering(graphicsCommandBuffer.getCommandBuffer());

				transferInfos.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
				transferInfos.commandBuffer = graphicsCommandBuffer.getCommandBuffer();
				transferInfos.srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				transferInfos.dstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				transferInfos.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				transferInfos.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				transferInfos.image = context.getSwapchain()->getImages()[imageIndex];
				transferInfos.dstAccessMask = 0;
				transferInfos.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				se::renderer::vulkan::transferImageLayout(transferInfos);

				(void)vkEndCommandBuffer(graphicsCommandBuffer.getCommandBuffer());


				VkSubmitInfo submitInfos {};
				VkPipelineStageFlags dstStageMask {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
				submitInfos.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfos.commandBufferCount = 1;
				submitInfos.pCommandBuffers = &graphicsCommandBuffer.getCommandBuffer();
				submitInfos.waitSemaphoreCount = 1;
				submitInfos.pWaitSemaphores = &imageReadySemaphore.getSemaphore();
				submitInfos.pWaitDstStageMask = &dstStageMask;
				submitInfos.signalSemaphoreCount = 1;
				submitInfos.pSignalSemaphores = &imageDrawnSemaphore.getSemaphore();
				(void)vkQueueSubmit(context.getDevice()->getQueue(se::renderer::vulkan::QueueType::eGraphics),
					1, &submitInfos, previousFrameReadyFence.getFence()
				);

				VkSwapchainKHR swapchain {context.getSwapchain()->getSwapChain()};
				VkPresentInfoKHR presentInfos {};
				presentInfos.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				presentInfos.swapchainCount = 1;
				presentInfos.pSwapchains = &swapchain;
				presentInfos.pImageIndices = &imageIndex;
				presentInfos.pResults = nullptr;
				presentInfos.waitSemaphoreCount = 1;
				presentInfos.pWaitSemaphores = &imageDrawnSemaphore.getSemaphore();
				(void)vkQueuePresentKHR(context.getDevice()->getQueue(se::renderer::vulkan::QueueType::ePresent), &presentInfos);*/
			}

			vkDeviceWaitIdle(context.getDevice()->getDevice());
		}
};


se::Application *createApplication(const std::vector<std::string> &/*args*/) {
	return new SandboxApp();
}