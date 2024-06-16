#pragma once

#include <string>

#include "se/core.hpp"
#include "se/utils/version.hpp"



namespace se {
	struct ApplicationInfos {
		std::string name;
		se::utils::Version version;
	};

	class SE_CORE Application {
		public:
			Application();
			virtual ~Application();

			virtual void run() = 0;

			inline const se::ApplicationInfos &getInfos() const noexcept {return m_infos;}

		protected:
			se::ApplicationInfos m_infos;
	};

} // namespace se
