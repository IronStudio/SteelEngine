#include "se/window/window.hpp"



namespace se::window
{
	template <typename Implementation>
	SE_REQUIRES(se::concepts::WindowImplementation<Implementation>)
	WindowBase<Implementation>::WindowBase() :
		m_implementation {}
	{

	}

	template <typename Implementation>
	SE_REQUIRES(se::concepts::WindowImplementation<Implementation>)
	WindowBase<Implementation>::~WindowBase() {
		m_implementation.destroy();
	}

	template <typename Implementation>
	SE_REQUIRES(se::concepts::WindowImplementation<Implementation>)
	WindowBase<Implementation>::WindowBase(const se::window::WindowInfos &infos) :
		m_implementation {}
	{
		m_implementation.create(infos);
	}

	template <typename Implementation>
	SE_REQUIRES(se::concepts::WindowImplementation<Implementation>)
	WindowBase<Implementation>::WindowBase(se::window::WindowBase<Implementation> &&window) noexcept :
		m_implementation {}
	{
		m_implementation.move(window.m_implementation);
	}

	template <typename Implementation>
	SE_REQUIRES(se::concepts::WindowImplementation<Implementation>)
	const se::window::WindowBase<Implementation> &WindowBase<Implementation>::operator=(
		se::window::WindowBase<Implementation> &&window
	) noexcept {
		m_implementation.move(window.m_implementation);
		return *this;
	}

} // namespace se::window
