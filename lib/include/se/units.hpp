#pragma once

#include <string>

#include "se/math.hpp"
#include "se/types.hpp"



namespace se {
	template <se::Uint NUM, se::Uint DEN>
	struct Ratio {
		static constexpr se::Uint num {NUM};
		static constexpr se::Uint den {DEN};
		static constexpr se::Float ratio {static_cast<se::Float> (NUM) / static_cast<se::Float> (DEN)};
		static const std::string prefix;
	};

	template <> constexpr std::string Ratio<1, 1000>::prefix {"m"};
	template <> constexpr std::string Ratio<1, 1'000'000>::prefix {"μ"};
	template <> constexpr std::string Ratio<1, 1'000'000'000>::prefix {"n"};
	template <> constexpr std::string Ratio<1000, 1>::prefix {"k"};
	template <> constexpr std::string Ratio<1'000'000, 1>::prefix {"M"};
	template <> constexpr std::string Ratio<1'000'000'000, 1>::prefix {"G"};
	template <> constexpr std::string Ratio<1024, 1>::prefix {"ki"};
	template <> constexpr std::string Ratio<1024*1024, 1>::prefix {"Mi"};
	template <> constexpr std::string Ratio<1024*1024*1024, 1>::prefix {"Gi"};

	template <se::Uint NUM, se::Uint DEN>
	constexpr std::string Ratio<NUM, DEN>::prefix {""};

	using Milli = se::Ratio<1, 1000>;
	using Micro = se::Ratio<1, 1'000'000>;
	using Nano  = se::Ratio<1, 1'000'000'000>;
	using Kilo  = se::Ratio<1000, 1>;
	using Mega  = se::Ratio<1'000'000, 1>;
	using Giga  = se::Ratio<1'000'000'000, 1>;

	using Kibi  = se::Ratio<1024, 1>;
	using Mebi  = se::Ratio<1024*1024, 1>;
	using Gibi  = se::Ratio<1024*1024*1024, 1>;

	template <typename T>
	T pi;

	template <> constexpr se::Float32 pi<se::Float32> {3.14159265358979323846264338327950288419716939937510582097494459230781640628620899f};
	template <> constexpr se::Float64 pi<se::Float64> {3.14159265358979323846264338327950288419716939937510582097494459230781640628620899};
	template <> constexpr long double pi<long double> {3.14159265358979323846264338327950288419716939937510582097494459230781640628620899L};


	namespace literals {
		inline long double operator ""_deg(long double val) {return val * se::pi<long double> / 180.0L;}
		inline long double operator ""_deg(unsigned long long val) {return val * se::pi<long double> / 180.0L;}
	} // namespace literals

} // namespace se
