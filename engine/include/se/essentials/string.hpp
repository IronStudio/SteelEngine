#pragma once

#include <ostream>
#include <string>

#include "../core.hpp"
#include "intTypes.hpp"



namespace se
{
	/**
	 * @brief Enum representing different string charset, encoding
	*/
	enum class Charset
	{
		UTF8,
		UTF16,
		UTF32
	};


	template <se::Charset charset>
	struct Char
	{
		struct Type;
		static se::Size size;
	};



	/**
	 * @brief Allow for easy, safe and fast manipulation of string. Class of all strings in the engine
	 * @tparam charset The charset, encoding of the string (UTF8 | UTF16 | UTF32)
	 * @warning For the moment, only UTF8 charset is supported
	*/
	template <se::Charset charset>
	class SE_CORE String final
	{
		public:
			String();
			String(se::Char<charset>::Type character);
			String(const se::Char<charset>::Type *str);
			String(const se::String<charset> &str);
			String(se::String<charset> &&str) noexcept;
			String(const std::string &str);

			const se::String<charset> &operator=(se::Char<charset>::Type character);
			const se::String<charset> &operator=(const se::Char<charset>::Type *str);
			const se::String<charset> &operator=(const se::String<charset> &str);
			const se::String<charset> &operator=(se::String<charset> &&str) noexcept;
			const se::String<charset> &operator=(const std::string &str);

			~String();

			/**
			 * @brief Append str to the end of the current string
			 * @warning Reallocate memory
			 * @deprecated Please use operator+=(const se::String<charset> &) where possible
			*/
			inline const se::String<charset> &operator+=(const se::Char<charset>::Type *str);
			/**
			 * @brief Append str to the end of the current string
			 * @warning Reallocate memory
			*/
			const se::String<charset> &operator+=(const se::String<charset> &str);
			/**
			 * @brief Append str to the end of the current string
			 * @warning Reallocate memory
			 * @deprecated Please use operator+=(const se::String<charset> &) where possible
			*/
			inline const se::String<charset> &operator+=(const std::string &str);

			/**
			 * @brief Check if str is equal to the current string
			 * @warning Allocate memory
			 * @deprecated Please use operator==(const se::String<charset> &) where possible
			*/
			inline bool operator==(const se::Char<charset>::Type *str) const noexcept;
			/**
			 * @brief Check if str is equal to the current string
			*/
			bool operator==(const se::String<charset> &str) const noexcept;
			/**
			 * @brief Check if str is equal to the current string
			 * @warning Allocate memory
			 * @deprecated Please use operator==(const se::String<charset> &) where possible
			*/
			inline bool operator==(const std::string &str) const noexcept;

			/**
			 * @brief Access a character of the string
			 * @warning In UTF8 and UTF16 charset, the returned character can be only one part of a multi-bytes character, like é
			*/
			se::Char<charset>::Type &operator[](se::Length index);
			/**
			 * @brief Access a character of the string
			 * @warning In UTF8 and UTF16 charset, the returned character can be only one part of a multi-bytes character, like é
			*/
			se::Char<charset>::Type operator[](se::Length index) const;

			/**
			 * @brief Free the memory of the string and send the string in the state `se::String<charset> ()`
			*/
			void clear();
			/**
			 * @brief Return `true` if the string is empty (getLength() = 0 or getCString() = 0) and `false` otherwise
			*/
			inline bool isEmpty() const noexcept;
			/**
			 * @brief Resize the memory buffer of the string to `size` to accomodate more characters
			 * @warning `size` **must** be greater or equal to `this->getSizeInBytes()`
			*/
			void reserve(se::Size size);

			/**
			 * @brief Return a C-style raw array reprensenting the null-terminated string
			*/
			inline const se::Char<charset>::Type *getCString() const noexcept;
			/**
			 * @brief Return the string in the form of an std::string
			*/
			inline std::string getCppString() const noexcept;
			/**
			 * @brief Return the length of the string, not including '\0'
			 * @warning The length is in **visible** character, not in bytes (eg 'é' count as 1, not 2 in UTF8)
			*/
			inline se::Length getLength() const noexcept;
			/**
			 * @brief Return the size in bytes of the string, including '\0'
			 * @warning Multi-bytes character count as the number of bytes they occupe in memory, not as 1 (eg 'é' count as 2 in UTF8) 
			*/
			inline se::Size getSizeInBytes() const noexcept;


		private:
			void p_allocateMemoryForArray(const se::Char<charset>::Type *str);
			void p_copyRawArray(const se::Char<charset>::Type *str);

			se::Char<charset>::Type *m_data;
			se::Length m_length;
			se::Size m_sizeInBytes;
			se::Size m_capacity;
	};


	template <se::Charset charset>
	SE_CORE se::String<charset> intToString(se::Int64 number, se::Uint base = 10);
	template <se::Charset charset>
	SE_CORE se::String<charset> uintToString(se::Uint64 number, se::Uint base = 10);



	/**
	 * @deprecated Please use operator+(const se::String<charset> &, const se::String<charset> &) where possible
	*/
	template <se::Charset charset>
	inline const se::String<charset> &operator+(se::String<charset> a, const typename se::Char<charset>::Type *b);
	template <se::Charset charset>
	inline const se::String<charset> &operator+(se::String<charset> a, const se::String<charset> &b);
	/**
	 * @deprecated Please use operator+(const se::String<charset> &, const se::String<charset> &) where possible
	*/
	template <se::Charset charset>
	inline const se::String<charset> &operator+(se::String<charset> a, const std::string &b);


	/**
	 * @brief Ouput the str to the stream, if the str is not empty
	 * @deprecated Use it only to output to std::cout
	*/
	template <se::Charset charset>
	inline std::ostream &operator<<(std::ostream &stream, const se::String<charset> &str) noexcept;


	using UTF8String  = se::String<se::Charset::UTF8>;
	//using UTF16String = se::String<se::Charset::UTF16>;
	//using UTF32String = se::String<se::Charset::UTF32>;


	template <>
	struct Char<se::Charset::UTF8>
	{
		using Type = char;
		static se::Size size;
	};

	template <>
	struct Char<se::Charset::UTF16>
	{
		using Type = char16_t;
		static se::Size size;
	};

	template <>
	struct Char<se::Charset::UTF32>
	{
		using Type = char32_t;
		static se::Size size;
	};

} // namespace se


#include "string.inl"