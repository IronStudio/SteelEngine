#pragma once

#include <functional>

#include "core.hpp"
#include "event.hpp"



namespace se
{
	struct EventListenerInfos
	{
		se::UUID uuid;
		se::UUID eventType;
		se::UUID linkedObject;
	};



	/**
	 * @brief Base class of all events listeners 
	*/
	class SE_CORE EventListener
	{
		public:
			EventListener(se::UUID eventType, se::UUID linkedObject);
			virtual ~EventListener() = default;

			virtual bool process(se::EventType type, se::Event event) SE_THREAD_SAFE = 0;

			inline const se::EventListenerInfos &getInfos() const noexcept;


		protected:
			se::EventListenerInfos m_infos;
	};



	class SE_CORE LambdaListener final : public se::EventListener
	{
		public:
			using Callback = std::function<bool(se::EventType, se::Event)>;

			LambdaListener(se::UUID eventType, se::UUID linkedObject, const se::LambdaListener::Callback &callback);
			~LambdaListener() override = default;

			bool process(se::EventType type, se::Event event) SE_THREAD_SAFE override;


		private:
			se::LambdaListener::Callback m_callback;
	};



} // namespace se



#include "eventListener.inl"