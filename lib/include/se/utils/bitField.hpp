#pragma once

#include <bitset>
#include <ostream>

#include "se/types.hpp"



#define SE_CREATE_BIT_FIELD(name, mask, ...) enum class name {\
	__VA_ARGS__\
};\
\
struct mask {\
	inline mask() : content {0} {}\
	inline mask(name flag) : content {static_cast<se::Uint64> (flag)} {}\
	inline const mask &operator=(name flag) {content = static_cast<se::Uint64> (flag); return *this;}\
	inline mask(const mask &flag) : content {flag.content} {}\
	inline const mask &operator=(const mask &flag) {content = flag.content; return *this;}\
	inline explicit operator bool() const noexcept {return !!content;}\
	inline bool operator==(const mask &flag) {return content == flag.content;}\
\
	inline const mask &operator|=(const mask &flag) {content |= flag.content; return *this;}\
	inline const mask &operator|=(name flag) {content |= static_cast<se::Uint64> (flag); return *this;}\
	inline const mask &operator&=(const mask &flag) {content &= flag.content; return *this;}\
	inline const mask &operator&=(name flag) {content &= static_cast<se::Uint64> (flag); return *this;}\
\
	se::Uint64 content;\
};\
\
inline mask operator|(mask lhs, mask rhs) {\
	mask out {};\
	out.content = lhs.content | rhs.content;\
	return out;\
}\
inline mask operator|(name lhs, mask rhs) {\
	mask out {};\
	out.content = static_cast<se::Uint64> (lhs) | rhs.content;\
	return out;\
}\
inline mask operator|(mask lhs, name rhs) {\
	mask out {};\
	out.content = lhs.content | static_cast<se::Uint64> (rhs);\
	return out;\
}\
inline mask operator|(name lhs, name rhs) {\
	mask out {};\
	out.content = static_cast<se::Uint64> (lhs) | static_cast<se::Uint64> (rhs);\
	return out;\
}\
\
inline mask operator&(mask lhs, mask rhs) {\
	mask out {};\
	out.content = lhs.content & rhs.content;\
	return out;\
}\
inline mask operator&(name lhs, mask rhs) {\
	mask out {};\
	out.content = static_cast<se::Uint64> (lhs) & rhs.content;\
	return out;\
}\
inline mask operator&(mask lhs, name rhs) {\
	mask out {};\
	out.content = lhs.content & static_cast<se::Uint64> (rhs);\
	return out;\
}\
inline mask operator&(name lhs, name rhs) {\
	mask out {};\
	out.content = static_cast<se::Uint64> (lhs) & static_cast<se::Uint64> (rhs);\
	return out;\
}\
inline std::ostream &operator<<(std::ostream &stream, const mask &val) {\
	stream << std::bitset<64> (val.content);\
	return stream;\
}


