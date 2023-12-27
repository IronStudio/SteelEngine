#include "graphicsApi.hpp"

#include <map>



namespace se
{
	std::string stringFromGraphicsApi(se::GraphicsApi api)
	{
		static const std::map<se::GraphicsApi, std::string> convertMap {
			{se::GraphicsApi::eOpenGL, "OpenGL"},
			{se::GraphicsApi::eVulkan, "Vulkan"},
			{se::GraphicsApi::eDirectX11, "DirectX11"},
			{se::GraphicsApi::eDirectX12, "DirectX12"}
		};

		auto it {convertMap.find(api)};
		if (it == convertMap.end())
			return "Unknown";

		return it->second;
	}



} // namespace se
