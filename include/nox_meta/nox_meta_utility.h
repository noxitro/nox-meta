//	Copyright (C) 2026 NOX ENGINE All rights reserved.

///	@file	utility.h
///	@brief	utility
#pragma once
#include	<string_view>
#include	"nox_meta_definition.h"

namespace nox::meta
{
	namespace util
	{
		template<class T> requires(sizeof(T) >= 0)
		[[nodiscard]]
		inline constexpr std::size_t SafeSizeof()noexcept
		{
			return sizeof(T);
		}

		template<class T>
		[[nodiscard]]
		inline constexpr std::size_t SafeSizeof()noexcept
		{
			return 0;
		}

		template<class T> requires(alignof(T) >= 0)
		[[nodiscard]]
		inline constexpr std::size_t SafeAlignof()noexcept
		{
			return alignof(T);
		}

		template<class T>
		[[nodiscard]]
		inline constexpr std::size_t SafeAlignof()noexcept
		{
			return 0;
		}
	}

	namespace detail
	{
		template<class T>
		[[nodiscard]] constexpr auto __cdecl GetTypeName(void)noexcept
		{
#if defined(__clang__)
			constexpr std::string_view signature = __PRETTY_FUNCTION__;
			constexpr std::string_view preSignature = "auto nox::meta::detail::GetTypeName()[T = ";
			constexpr std::string_view lastSignature = "]";
#else
			constexpr std::string_view signature = __FUNCSIG__;
			constexpr std::string_view preSignature = "auto __cdecl nox::meta::detail::GetTypeName<";
			constexpr std::string_view lastSignature = ">(void) noexcept";
#endif

			constexpr std::string_view contentsName(
				signature.data() + preSignature.size(),
				signature.size() - (preSignature.size() + lastSignature.size())
			);
			return contentsName;
		}

		template<class T>
		constexpr bool is_scoped_enum_v = std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

		template<class T> requires(std::is_enum_v<T>)
			[[nodiscard]] inline constexpr auto ToUnderlying(const T value)noexcept { return static_cast<std::underlying_type_t<T>>(value); }

		template<class T> requires(std::is_enum_v<T>)
			[[nodiscard]] inline constexpr T BitOr(const T a, const T b)noexcept { return static_cast<T>(nox::meta::detail::ToUnderlying(a) | nox::meta::detail::ToUnderlying(b)); }

		template<bool conditional, auto flag, class FlagType = decltype(flag)>
		[[nodiscard]] inline constexpr FlagType BitOrConditional(FlagType value)noexcept
		{
			if constexpr (conditional)
			{
				return nox::meta::detail::BitOr(value, flag);
			}
			else
			{
				return value;
			}
		}

		template<class T>
		struct TypeKindHolder;

		template<nox::meta::TypeKind _value>
		struct ITypeKindHolder
		{
			static constexpr nox::meta::TypeKind value = _value;
		};

		template<class T>
		struct TypeKindHolder : ITypeKindHolder<nox::meta::TypeKind::Unknown> {};

		template<>
		struct TypeKindHolder<void> : ITypeKindHolder<nox::meta::TypeKind::Void> {};

		template<>
		struct TypeKindHolder<bool> : ITypeKindHolder<nox::meta::TypeKind::Bool> {};

		template<>
		struct TypeKindHolder<char> : ITypeKindHolder<nox::meta::TypeKind::Char> {};

		template<>
		struct TypeKindHolder<signed char> : ITypeKindHolder<nox::meta::TypeKind::SignedChar> {};

		template<>
		struct TypeKindHolder<unsigned char> : ITypeKindHolder<nox::meta::TypeKind::UnsignedChar> {};

		template<>
		struct TypeKindHolder<char8_t> : ITypeKindHolder<nox::meta::TypeKind::Char8> {};

		template<>
		struct TypeKindHolder<char16_t> : ITypeKindHolder<nox::meta::TypeKind::Char16> {};

		template<>
		struct TypeKindHolder<char32_t> : ITypeKindHolder<nox::meta::TypeKind::Char32> {};

		template<>
		struct TypeKindHolder<wchar_t> : ITypeKindHolder<nox::meta::TypeKind::WideChar> {};


		template<>
		struct TypeKindHolder<std::int16_t> : ITypeKindHolder<nox::meta::TypeKind::Int16> {};

		template<>
		struct TypeKindHolder<std::uint16_t> : ITypeKindHolder<nox::meta::TypeKind::UInt16> {};

		template<>
		struct TypeKindHolder<std::int32_t> : ITypeKindHolder<nox::meta::TypeKind::Int32> {};

		template<>
		struct TypeKindHolder<std::uint32_t> : ITypeKindHolder<nox::meta::TypeKind::UInt32> {};

		template<>
		struct TypeKindHolder<std::int64_t> : ITypeKindHolder<nox::meta::TypeKind::Int64> {};

		template<>
		struct TypeKindHolder<std::uint64_t> : ITypeKindHolder<nox::meta::TypeKind::UInt64> {};

		template<>
		struct TypeKindHolder<std::float_t> : ITypeKindHolder<nox::meta::TypeKind::Float> {};

		template<>
		struct TypeKindHolder<std::double_t> : ITypeKindHolder<nox::meta::TypeKind::Double> {};

		template<>
		struct TypeKindHolder<long double> : ITypeKindHolder<nox::meta::TypeKind::LongDouble> {};

		//	--- nullptr ---
		template<>
		struct TypeKindHolder<std::nullptr_t> : ITypeKindHolder<nox::meta::TypeKind::Nullptr> {};

		//	--- user defined categories ---
		template<class T> requires(std::is_union_v<T>)
			struct TypeKindHolder<T> : ITypeKindHolder<nox::meta::TypeKind::Union> {};

		template<class T> requires(std::is_class_v<T>)
			struct TypeKindHolder<T> : ITypeKindHolder<nox::meta::TypeKind::Class> {};

		template<class T> requires(std::is_enum_v<T>&& nox::meta::detail::is_scoped_enum_v<T>)
			struct TypeKindHolder<T> : ITypeKindHolder<nox::meta::TypeKind::ScopedEnum> {};

		template<class T> requires(std::is_enum_v<T> && (!nox::meta::detail::is_scoped_enum_v<T>))
			struct TypeKindHolder<T> : ITypeKindHolder<nox::meta::TypeKind::Enum> {};

		//	--- function ---
		template<class T> requires(std::is_function_v<T>)
			struct TypeKindHolder<T> : ITypeKindHolder<nox::meta::TypeKind::Function> {};

		//	--- pointer / reference ---
		template<class T>
		struct TypeKindHolder<T*> : ITypeKindHolder<nox::meta::TypeKind::Pointer> {};

		template<class T>
		struct TypeKindHolder<T&> : ITypeKindHolder<nox::meta::TypeKind::LValueReference> {};

		template<class T>
		struct TypeKindHolder<T&&> : ITypeKindHolder<nox::meta::TypeKind::RValueReference> {};

		//	--- array ---
		template<class T, std::size_t N>
		struct TypeKindHolder<T[N]> : ITypeKindHolder<nox::meta::TypeKind::Array> {};

		template<class T>
		struct TypeKindHolder<T[]> : ITypeKindHolder<nox::meta::TypeKind::UnboundedArray> {};
	}

	template<class _T>
	[[nodiscard]] inline constexpr nox::meta::TypeKind GetTypeKind()noexcept
	{
		using T = std::remove_cv_t<_T>;
		return nox::meta::detail::TypeKindHolder<T>::value;
	}

	template<class T>
	[[nodiscard]] inline constexpr nox::meta::TypeAttributeFlag GetTypeAttributeFlags()noexcept
	{
		nox::meta::TypeAttributeFlag flags = nox::meta::TypeAttributeFlag::None;
		flags = nox::meta::detail::BitOrConditional<std::is_const_v<T>, nox::meta::TypeAttributeFlag::Const>(flags);

		return flags;
	}
}