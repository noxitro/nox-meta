//	Copyright (C) 2026 NOX ENGINE All rights reserved.

///	@file	utility.h
///	@brief	utility
#pragma once
#include	<string_view>
#include	<algorithm>
#include	<ranges>
#include	<array>
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
		template<class CharT, std::size_t N>
		struct LiteralBasicString
		{
			std::array<CharT, N + 1> value{};

			inline constexpr LiteralBasicString(const CharT(&literal)[N + 1])noexcept
			{
				std::ranges::copy(literal, value.data());
			}

			[[nodiscard]] inline constexpr std::basic_string_view<CharT> view()const noexcept { return std::basic_string_view<CharT>(value.data(), N); }
			[[nodiscard]] inline constexpr operator std::basic_string_view<CharT>()const noexcept { return view(); }

			[[nodiscard]] inline constexpr bool operator==(const LiteralBasicString&)const noexcept = default;
		};

		template<class CharT, std::size_t N>
		LiteralBasicString(const CharT(&)[N]) -> LiteralBasicString<CharT, N - 1>;

		template<std::size_t N>
		using LiteralString = LiteralBasicString<char, N>;

		template<nox::meta::detail::LiteralString signature>
		[[nodiscard]] inline constexpr auto GetTypeNameImpl()noexcept
		{
			constexpr const std::string_view signature_view = signature.view();

#if defined(__clang__)
			constexpr std::string_view key_begin = " = ";
			constexpr std::size_t begin_mark = signature_view.find(key_begin);
			constexpr std::size_t begin = begin_mark + key_begin.size();
			constexpr std::size_t end = signature_view.rfind(']');

			static_assert(begin_mark != std::string_view::npos, "GetTypeName parse failed (begin mark)");
#else
			constexpr std::string_view key_begin = "GetTypeName<";
			constexpr std::string_view key_end = ">(void)";
			constexpr std::size_t begin_mark = signature_view.find(key_begin);
			constexpr std::size_t begin = begin_mark + key_begin.size();
			constexpr std::size_t end = signature_view.rfind(key_end);

			static_assert(begin_mark != std::string_view::npos, "GetTypeName parse failed (begin mark)");
#endif

			static_assert(begin != std::string_view::npos, "GetTypeName parse failed (begin)");
			static_assert(end != std::string_view::npos, "GetTypeName parse failed (end)");
			static_assert(begin <= end, "GetTypeName parse failed (range)");

			return signature_view.substr(begin, end - begin);
		}

		template<class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<class...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<class...> class> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<template<class...> class> class> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<auto...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<auto, class, auto, class> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<auto, class, class...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<class, auto, auto...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<class, auto, class, class...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<auto, class, auto, auto...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<class, auto, class, auto, auto...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<class...> class, class, class...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<class...> class, auto, auto...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<template<class...> class> class, class, class...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<template<class...> class> class, auto, auto...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<auto...> class> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<auto...> class, class, class...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
		}

		template<template<template<auto...> class, auto, auto...> class T>
		[[nodiscard]] inline constexpr auto GetTypeName(void)noexcept
		{
#if defined(__clang__)
			return ::nox::meta::detail::GetTypeNameImpl<__PRETTY_FUNCTION__>();
#else
			return ::nox::meta::detail::GetTypeNameImpl<__FUNCSIG__>();
#endif
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
		inline constexpr std::size_t GetPointerDepth()noexcept
		{ 
			if constexpr (std::is_pointer_v<T>)
			{
				return 1 + nox::meta::detail::GetPointerDepth<std::remove_pointer_t<T>>();
			}
			else
			{
				return 0;
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
		struct TypeKindHolder<signed char> : ITypeKindHolder<nox::meta::TypeKind::Int8> {};

		template<>
		struct TypeKindHolder<unsigned char> : ITypeKindHolder<nox::meta::TypeKind::UInt8> {};

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

#if defined(__SIZEOF_INT128__)
		template<>
		struct TypeKindHolder<__int128> : ITypeKindHolder < nox::meta::TypeKind::Int128> {};

		template<>
		struct TypeKindHolder<unsigned __int128> : ITypeKindHolder<nox::meta::TypeKind::UInt128> {};		
#endif // defined(__SIZEOF_INT128__)

		template<>
		struct TypeKindHolder<std::float_t> : ITypeKindHolder<nox::meta::TypeKind::Float> {};

		template<>
		struct TypeKindHolder<std::double_t> : ITypeKindHolder<nox::meta::TypeKind::Double> {};

#if defined(__STDCPP_FLOAT16_T__)
		template<>
		struct TypeKindHolder<std::float16_t> : ITypeKindHolder<nox::meta::TypeKind::Float16> {};
#endif // defined(__STDCPP_FLOAT16_T__)

#if defined(__STDCPP_BFLOAT16_T__)
		template<>
		struct TypeKindHolder<std::bfloat16_t> : ITypeKindHolder<nox::meta::TypeKind::BFloat16> {};
#endif // defined(__STDCPP_BFLOAT16_T__)

#if defined(__STDCPP_FLOAT128_T__)
		template<>
		struct TypeKindHolder<std::float128_t> : ITypeKindHolder<nox::meta::TypeKind::Float128> {};
#endif // defined(__STDCPP_FLOAT128_T__)

		template<>
		struct TypeKindHolder<long double> : ITypeKindHolder<nox::meta::TypeKind::LongDouble> {};

		template<>
		struct TypeKindHolder<long> : ITypeKindHolder<nox::meta::TypeKind::Long> {};

		template<>
		struct TypeKindHolder<unsigned long> : ITypeKindHolder<nox::meta::TypeKind::UnsignedLong> {};

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

		template<class T> requires(std::is_member_function_pointer_v<T>)
			struct TypeKindHolder<T> : ITypeKindHolder<nox::meta::TypeKind::MemberFunctionPointer> {};

		template<class T> requires(std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>)
			struct TypeKindHolder<T> : ITypeKindHolder<nox::meta::TypeKind::FunctionPointer> {};

		template<class T> requires(std::is_member_object_pointer_v<T>)
			struct TypeKindHolder<T> : ITypeKindHolder<nox::meta::TypeKind::MemberObjectPointer> {};

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

	template<class T>
	[[nodiscard]] inline constexpr nox::meta::TypeKind GetTypeKind()noexcept
	{
		using TT = std::remove_cv_t<T>;
		return nox::meta::detail::TypeKindHolder<TT>::value;
	}

	template<class T>
	[[nodiscard]] inline constexpr nox::meta::TypeAttributeFlag GetTypeAttributeFlags()noexcept
	{
		nox::meta::TypeAttributeFlag flags = nox::meta::TypeAttributeFlag::None;

		//		qualifiers
		flags = nox::meta::detail::BitOrConditional<std::is_const_v<T>, nox::meta::TypeAttributeFlag::Const>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_volatile_v<T>, nox::meta::TypeAttributeFlag::Volatile>(flags);

		//		class attributes
		flags = nox::meta::detail::BitOrConditional<std::is_final_v<T>, nox::meta::TypeAttributeFlag::Final>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_abstract_v<T>, nox::meta::TypeAttributeFlag::Abstract>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_polymorphic_v<T>, nox::meta::TypeAttributeFlag::Polymorphic>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_aggregate_v<T>, nox::meta::TypeAttributeFlag::Aggregate>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_trivial_v<T>, nox::meta::TypeAttributeFlag::Trivial>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_standard_layout_v<T>, nox::meta::TypeAttributeFlag::StandardLayout>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_empty_v<T>, nox::meta::TypeAttributeFlag::Empty>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_trivially_copyable_v<T>, nox::meta::TypeAttributeFlag::TriviallyCopyable>(flags);

		//		signed / unsigned
		flags = nox::meta::detail::BitOrConditional<std::is_signed_v<T>, nox::meta::TypeAttributeFlag::Signed>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_unsigned_v<T>, nox::meta::TypeAttributeFlag::Unsigned>(flags);

		//		constructibility
		flags = nox::meta::detail::BitOrConditional<std::is_default_constructible_v<T>, nox::meta::TypeAttributeFlag::DefaultConstructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_trivially_default_constructible_v<T>, nox::meta::TypeAttributeFlag::TriviallyDefaultConstructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_nothrow_default_constructible_v<T>, nox::meta::TypeAttributeFlag::NothrowDefaultConstructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_copy_constructible_v<T>, nox::meta::TypeAttributeFlag::CopyConstructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_trivially_copy_constructible_v<T>, nox::meta::TypeAttributeFlag::TriviallyCopyConstructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_nothrow_copy_constructible_v<T>, nox::meta::TypeAttributeFlag::NothrowCopyConstructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_move_constructible_v<T>, nox::meta::TypeAttributeFlag::MoveConstructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_trivially_move_constructible_v<T>, nox::meta::TypeAttributeFlag::TriviallyMoveConstructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_nothrow_move_constructible_v<T>, nox::meta::TypeAttributeFlag::NothrowMoveConstructible>(flags);
		
		//		destructibility
		flags = nox::meta::detail::BitOrConditional<std::is_destructible_v<T>, nox::meta::TypeAttributeFlag::Destructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_trivially_destructible_v<T>, nox::meta::TypeAttributeFlag::TriviallyDestructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_nothrow_destructible_v<T>, nox::meta::TypeAttributeFlag::NothrowDestructible>(flags);
		flags = nox::meta::detail::BitOrConditional<std::has_virtual_destructor_v<T>, nox::meta::TypeAttributeFlag::HasVirtualDestructor>(flags);

		//		assignability
		flags = nox::meta::detail::BitOrConditional<std::is_copy_assignable_v<T>, nox::meta::TypeAttributeFlag::CopyAssignable>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_trivially_copy_assignable_v<T>, nox::meta::TypeAttributeFlag::TriviallyCopyAssignable>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_nothrow_copy_assignable_v<T>, nox::meta::TypeAttributeFlag::NothrowCopyAssignable>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_move_assignable_v<T>, nox::meta::TypeAttributeFlag::MoveAssignable>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_trivially_move_assignable_v<T>, nox::meta::TypeAttributeFlag::TriviallyMoveAssignable>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_nothrow_move_assignable_v<T>, nox::meta::TypeAttributeFlag::NothrowMoveAssignable>(flags);

		//		swappability
		flags = nox::meta::detail::BitOrConditional<std::is_swappable_v<T>, nox::meta::TypeAttributeFlag::Swappable>(flags);
		flags = nox::meta::detail::BitOrConditional<std::is_nothrow_swappable_v<T>, nox::meta::TypeAttributeFlag::NothrowSwappable>(flags);

		return flags;
	}
}