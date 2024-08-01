#pragma once

#include "se/core.hpp"
#include "se/types.hpp"



namespace se {
	enum class StringEncoding {
		eUtf8,
		eUtf16
	};


	template <se::StringEncoding encoding>
	struct EncodingChar {};

	template <> struct EncodingChar<se::StringEncoding::eUtf8>  {using Char = se::Char8;};
	template <> struct EncodingChar<se::StringEncoding::eUtf16> {using Char = se::Char16;};


	template <se::StringEncoding encoding>
	class SE_CORE _String final {
		using _Char = typename se::EncodingChar<encoding>::Char;

		public:
			_String() noexcept;
			_String(const _Char *const str) noexcept;
			_String(const _Char *const str, const se::Count size) noexcept;
			~_String();

			_String(const se::_String<encoding> &string) noexcept;
			const se::_String<encoding> &operator=(const se::_String<encoding> &string) noexcept;

			_String(se::_String<encoding> &&string) noexcept;
			const se::_String<encoding> &operator=(se::_String<encoding> &&string) noexcept;


			template <se::StringEncoding encoding2>
			se::_String<encoding2> useEncoding() const noexcept;

			se::_String<encoding> operator+(const se::_String<encoding> &string);
			const se::_String<encoding> &operator+=(const se::_String<encoding> &string);

			_Char &operator[] (se::Count index) const noexcept;

			inline const _Char *const &getData() const noexcept {return m_data;}
			inline const se::Count &getSize() const noexcept {return m_size;}


		private:
			_Char *m_data;
			se::Count m_size;
	};


	using UTF8String  = se::_String<se::StringEncoding::eUtf8>;
	using UTF16String = se::_String<se::StringEncoding::eUtf16>;

	#ifndef SE_USE_UTF16_STRING
		using String = se::UTF8String;
	#else
		using String = se::UTF16String;
	#endif

} // namespace se



#include "se/utils/string.inl"