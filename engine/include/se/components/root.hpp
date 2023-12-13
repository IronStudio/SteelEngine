#pragma once

#include "../component.hpp"



namespace se::components
{
	class Root final : public se::Component
	{
		public:
			inline Root();
			~Root() override = default;

			inline bool canBeChild() const noexcept override;
			inline bool canBeParent() const noexcept override;
	};

} // namespace se::components



#include "root.inl"