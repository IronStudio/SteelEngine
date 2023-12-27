#pragma once

#include <string>

#include "core.hpp"



namespace se
{
	enum class GraphicsApi
	{
		eOpenGL,
		eVulkan,
		eDirectX11,
		eDirectX12
	};



	SE_CORE std::string stringFromGraphicsApi(se::GraphicsApi api);


	
} // namespace se
