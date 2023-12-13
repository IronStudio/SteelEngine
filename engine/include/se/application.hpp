#pragma once



namespace se
{
	class Application
	{
		public:
			inline Application() = default;
			virtual ~Application() = default;

			inline virtual void onLoad();
			virtual void onUpdate() = 0;
			inline virtual void onUnload() noexcept;
			virtual void onError() noexcept = 0;
	};


	se::Application *createApplication();

} // namespace se



#include "application.inl"