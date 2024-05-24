#include "se/utils/byteSize.hpp"



namespace se::literals {
	se::ByteCount operator ""_kiB(long double val) {return val * (se::Kibi::num / se::Kibi::den);}
	se::ByteCount operator "" _MiB(long double val) {return val * (se::Mebi::num / se::Mebi::den);}
	se::ByteCount  operator ""_GiB(long double val) {return val * (se::Gibi::num / se::Gibi::den);}
	se::ByteCount operator ""_kB(long double val) {return val * (se::Kilo::num / se::Kilo::den);}
	se::ByteCount operator "" _MB(long double val) {return val * (se::Mega::num / se::Mega::den);}
	se::ByteCount  operator ""_GB(long double val) {return val * (se::Giga::num / se::Giga::den);}
	se::ByteCount operator ""_B(unsigned long long int val) {return val;}
	se::ByteCount operator ""_kiB(unsigned long long int val) {return val * (se::Kibi::num / se::Kibi::den);}
	se::ByteCount operator "" _MiB(unsigned long long int val) {return val * (se::Mebi::num / se::Mebi::den);}
	se::ByteCount  operator ""_GiB(unsigned long long int val) {return val * (se::Gibi::num / se::Gibi::den);}
	se::ByteCount operator ""_kB(unsigned long long int val) {return val * (se::Kibi::num / se::Kibi::den);}
	se::ByteCount operator "" _MB(unsigned long long int val) {return val * (se::Mega::num / se::Mega::den);}
	se::ByteCount  operator ""_GB(unsigned long long int val) {return val * (se::Giga::num / se::Giga::den);}
} // namespace se::literals