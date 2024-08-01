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
	struct _String;

	template <se::StringEncoding encoding>
	class SE_CORE StringIterator final {
		friend class se::_String<encoding>;
		using _Char = typename se::EncodingChar<encoding>::Char;

		public:
			StringIterator() noexcept;
			~StringIterator() = default;

			StringIterator(const se::StringIterator<encoding> &iterator);
			const se::StringIterator<encoding> &operator=(const se::StringIterator<encoding> &iterator);

			se::StringIterator<encoding> next() const noexcept;
			const se::StringIterator<encoding> &operator++() noexcept;
			se::StringIterator<encoding> operator++(int) noexcept;

			se::StringIterator<encoding> previous() const noexcept;
			const se::StringIterator<encoding> &operator--() noexcept;
			se::StringIterator<encoding> operator--(int) noexcept;

			bool operator==(const se::StringIterator<encoding> &iterator) const noexcept;

			_Char &operator*() const noexcept;
			_Char *operator->() const noexcept;

			const se::StringIterator<encoding> &operator+=(se::Count offset) noexcept;
			const se::StringIterator<encoding> &operator-=(se::Count offset) noexcept;
			se::StringIterator<encoding> operator+(se::Count offset) const noexcept;
			se::StringIterator<encoding> operator-(se::Count offset) const noexcept;

			se::Count operator-(const se::StringIterator<encoding> &iterator) const noexcept;

			inline const se::_String<encoding> *getLinkedContainer() const noexcept {return m_linkedContainer;}


		protected:
			StringIterator(_Char *pointer, const se::_String<encoding> *linkedContainer) noexcept;

		private:
			const se::_String<encoding> *m_linkedContainer;
			_Char *m_pointer;
	};

	template <se::StringEncoding encoding>
	inline se::StringIterator<encoding> operator+(const se::Count &offset, const se::StringIterator<encoding> &iterator) {
		return iterator.operator+(offset);
	}


	template <se::StringEncoding encoding>
	class SE_CORE _String final {
		friend class se::StringIterator<encoding>;
		using _Char = typename se::EncodingChar<encoding>::Char;

		public:
			using Iterator = se::StringIterator<encoding>;

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

			Iterator begin() const noexcept;
			Iterator end() const noexcept;
			Iterator rbegin() const noexcept;
			Iterator rend() const noexcept;

			inline const _Char *const &getData() const noexcept {return m_data;}
			inline const se::Count &getSize() const noexcept {return m_size;}


		protected:
			inline _Char *const &getIterableData() const noexcept {return m_data;}

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