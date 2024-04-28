#pragma once

#include <string>

#include "se/types.hpp"



namespace se {
	template <se::Uint NUM, se::Uint DEN>
	struct Ratio {
		static constexpr se::Uint num {NUM};
		static constexpr se::Uint den {DEN};
		static constexpr se::Float ratio {static_cast<se::Float> (NUM) / static_cast<se::Float> (DEN)};
		static const std::string prefix;
	};

	template <> const std::string Ratio<1, 1000>::prefix {"m"};
	template <> const std::string Ratio<1, 1'000'000>::prefix {"μ"};
	template <> const std::string Ratio<1, 1'000'000'000>::prefix {"n"};
	template <> const std::string Ratio<1000, 1>::prefix {"k"};
	template <> const std::string Ratio<1'000'000, 1>::prefix {"M"};
	template <> const std::string Ratio<1'000'000'000, 1>::prefix {"G"};

	template <se::Uint NUM, se::Uint DEN>
	const std::string Ratio<NUM, DEN>::prefix {""};

	using Milli = se::Ratio<1, 1000>;
	using Micro = se::Ratio<1, 1'000'000>;
	using Nano  = se::Ratio<1, 1'000'000'000>;
	using Kilo  = se::Ratio<1000, 1>;
	using Mega  = se::Ratio<1'000'000, 1>;
	using Giga  = se::Ratio<1'000'000'000, 1>;
} // namespace se
