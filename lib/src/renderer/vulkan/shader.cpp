#include "se/renderer/vulkan/shader.hpp"

#include "se/exceptions.hpp"
#include "se/renderer/vulkan/context.hpp"
#include "se/utils/files.hpp"



namespace se::renderer::vulkan {
	VkShaderStageFlags shaderTypeMaskSeToVk(se::renderer::ShaderTypeMask type) {
		VkShaderStageFlags output {};

		for (se::Count i {0}; i < SE_SHADER_TYPE_COUNT; ++i) {
			auto flag {static_cast<se::renderer::ShaderType> (1 << i)};
			if (type & flag)
				output |= shaderTypeSeToVk(flag);
		}

		return output;
	}


	se::renderer::ShaderTypeMask shaderTypeMaskVkToSe(VkShaderStageFlags type) {
		static std::vector<VkShaderStageFlagBits> flagsList {
			VK_SHADER_STAGE_FRAGMENT_BIT,
			VK_SHADER_STAGE_GEOMETRY_BIT,
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_COMPUTE_BIT
		};

		se::renderer::ShaderTypeMask output {};
		for (auto flag : flagsList) {
			if (type & flag)
				output |= shaderTypeVkToSe(flag);
		}

		return output;
	}


	VkShaderStageFlagBits shaderTypeSeToVk(se::renderer::ShaderType type) {
		static std::map<se::renderer::ShaderType, VkShaderStageFlagBits> shaderTypesMap {
			{se::renderer::ShaderType::eFragment, VK_SHADER_STAGE_FRAGMENT_BIT},
			{se::renderer::ShaderType::eGeometry, VK_SHADER_STAGE_GEOMETRY_BIT},
			{se::renderer::ShaderType::eVertex,   VK_SHADER_STAGE_VERTEX_BIT},
			{se::renderer::ShaderType::eCompute,  VK_SHADER_STAGE_COMPUTE_BIT},
		};

		return shaderTypesMap[type];
	}


	se::renderer::ShaderType shaderTypeVkToSe(VkShaderStageFlagBits type) {
		static std::map<VkShaderStageFlagBits, se::renderer::ShaderType> shaderTypesMap {
			{VK_SHADER_STAGE_FRAGMENT_BIT, se::renderer::ShaderType::eFragment},
			{VK_SHADER_STAGE_GEOMETRY_BIT, se::renderer::ShaderType::eGeometry},
			{VK_SHADER_STAGE_VERTEX_BIT,   se::renderer::ShaderType::eVertex},
			{VK_SHADER_STAGE_COMPUTE_BIT,  se::renderer::ShaderType::eCompute}
		};

		return shaderTypesMap[type];
	}



	Shader::Shader(const se::renderer::ShaderInfos &infos) :
		se::renderer::Shader(infos),
		m_shaderModule {VK_NULL_HANDLE},
		m_shaderStageCreateInfos {}
	{
		

		m_shaderModule = s_loadShaderModule(m_infos.context, m_infos.file);

		m_shaderStageCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_shaderStageCreateInfos.module = m_shaderModule;
		m_shaderStageCreateInfos.pSpecializationInfo = nullptr;
		m_shaderStageCreateInfos.stage = shaderTypeSeToVk(m_infos.type);
		m_shaderStageCreateInfos.pName = m_infos.entryPoint.c_str();
	}


	Shader::~Shader() {
		if (m_shaderModule != VK_NULL_HANDLE)
			s_unloadShaderModule(m_infos.context, m_shaderModule);
	}


	VkShaderModule Shader::s_loadShaderModule(se::renderer::Context *context, const std::string &file) {
		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (context)->getDevice()->getDevice()};

		auto it {s_moduleMap.find(file)};
		if (it != s_moduleMap.end()) {
			++s_instanceCountMap[it->second];
			return it->second;
		}

		std::ifstream binaryFile {file + ".spv", std::ios::binary};
		auto code {se::utils::readBinaryFile(binaryFile)};

		VkShaderModuleCreateInfo moduleCreateInfos {};
		moduleCreateInfos.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfos.codeSize = code.size();
		moduleCreateInfos.pCode = reinterpret_cast<se::Uint32*> (code.data());

		VkShaderModule shaderModule {};
		if (vkCreateShaderModule(device, &moduleCreateInfos, nullptr, &shaderModule) != VK_SUCCESS)
			throw se::exceptions::RuntimeError("Can't create a shader module");

		s_moduleMap[file] = shaderModule;
		s_instanceCountMap[shaderModule] = 1;
		return shaderModule;
	}


	void Shader::s_unloadShaderModule(se::renderer::Context *context, VkShaderModule shaderModule) {
		if (--s_instanceCountMap[shaderModule] > 0)
			return;

		for (auto it {s_moduleMap.begin()}; it != s_moduleMap.end(); ++it) {
			if (it->second != shaderModule)
				continue;

			s_moduleMap.erase(it);
			break;
		}

		s_instanceCountMap.erase(shaderModule);

		VkDevice device {reinterpret_cast<se::renderer::vulkan::Context*> (context)->getDevice()->getDevice()};
		vkDestroyShaderModule(device, shaderModule, nullptr);
	}


	std::map<std::string, VkShaderModule> Shader::s_moduleMap {};
	std::map<VkShaderModule, se::Count> Shader::s_instanceCountMap {};

} // namespace se::renderer::vulkan
