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
	class SE_CORE String final {
		using _Char = typename se::EncodingChar<encoding>::Char;

		public:
			String() noexcept;
			String(const _Char *const str) noexcept;
			String(const _Char *const str, const se::Count size) noexcept;
			~String();

			String(const se::String<encoding> &string) noexcept;
			const se::String<encoding> &operator=(const se::String<encoding> &string) noexcept;

			String(se::String<encoding> &&string) noexcept;
			const se::String<encoding> &operator=(se::String<encoding> &&string) noexcept;


			template <se::StringEncoding encoding2>
			se::String<encoding2> useEncoding() const noexcept;


			inline const _Char *const &getData() const noexcept {return m_data;}
			inline const se::Count &getSize() const noexcept {return m_size;}


		private:
			_Char *m_data;
			se::Count m_size;
	};

} // namespace se



#include "se/utils/string.inl"