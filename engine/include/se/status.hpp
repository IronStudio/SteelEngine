#pragma once



namespace se
{
	enum class Status
	{
		eSuccess        = 0b00000001,
		eFailure        = 0b00000010,
		eRemoveListener = 0b00000100,
		eDuplicate      = 0b00001000
	};



} // namespace se
