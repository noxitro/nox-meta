// Copyright (C) 2026 NOX ENGINE All rights reserved.

///	@file	nox_meta_typetraits.h
///	@brief	nox_meta_typetraits
#pragma once
#include	<type_traits>

namespace nox::meta
{
	namespace detail
	{
		template<class T>
		struct TemplateOf;

		template<template<class...> class T, class... Args>
		struct TemplateOf<T<Args...>>
		{
			template<class... NewArgs>
			using type = T<NewArgs...>;
		};

		template<template<auto...> class T, auto... Values>
		struct TemplateOf<T<Values...>>
		{
			template<auto... NewValues>
			using type = T<NewValues...>;
		};

		template<template<auto, class, class...> class T, auto Value0, class Type0, class... Types>
		struct TemplateOf<T<Value0, Type0, Types...>>
		{
			template<auto NewValue0, class NewType0, class... NewTypes>
			using type = T<NewValue0, NewType0, NewTypes...>;
		};

		template<template<class, auto, auto...> class T, class Type0, auto Value0, auto... Values>
		struct TemplateOf<T<Type0, Value0, Values...>>
		{
			template<class NewType0, auto NewValue0, auto... NewValues>
			using type = T<NewType0, NewValue0, NewValues...>;
		};

		template<template<class, auto, class, class...> class T, class Type0, auto Value0, class Type1, class... Types>
		struct TemplateOf<T<Type0, Value0, Type1, Types...>>
		{
			template<class NewType0, auto NewValue0, class NewType1, class... NewTypes>
			using type = T<NewType0, NewValue0, NewType1, NewTypes...>;
		};

		template<template<auto, class, auto, auto...> class T, auto Value0, class Type0, auto Value1, auto... Values>
		struct TemplateOf<T<Value0, Type0, Value1, Values...>>
		{
			template<auto NewValue0, class NewType0, auto NewValue1, auto... NewValues>
			using type = T<NewValue0, NewType0, NewValue1, NewValues...>;
		};

		template<template<class, auto, class, auto, auto...> class T, class Type0, auto Value0, class Type1, auto Value1, auto... Values>
		struct TemplateOf<T<Type0, Value0, Type1, Value1, Values...>>
		{
			template<class NewType0, auto NewValue0, class NewType1, auto NewValue1, auto... NewValues>
			using type = T<NewType0, NewValue0, NewType1, NewValue1, NewValues...>;
		};

		template<template<auto, class, auto, class> class T, auto Value0, class Type0, auto Value1, class Type1>
		struct TemplateOf<T<Value0, Type0, Value1, Type1>>
		{
			template<auto NewValue0, class NewType0, auto NewValue1, class NewType1>
			using type = T<NewValue0, NewType0, NewValue1, NewType1>;
		};

		template<template<template<class...> class> class T, template<class...> class U>
		struct TemplateOf<T<U>>
		{
			template<template<class...> class NewU>
			using type = T<NewU>;
		};

		template<template<template<class> class> class T, template<class> class U>
		struct TemplateOf<T<U>>
		{
			template<template<class> class NewU>
			using type = T<NewU>;
		};

		template<template<template<template<class...> class> class> class T, template<template<class...> class> class U>
		struct TemplateOf<T<U>>
		{
			template<template<template<class...> class> class NewU>
			using type = T<NewU>;
		};

		template<template<template<template<class> class> class> class T, template<template<class> class> class U>
		struct TemplateOf<T<U>>
		{
			template<template<template<class> class> class NewU>
			using type = T<NewU>;
		};

		template<template<template<class...> class, class, class...> class T, template<class...> class U, class Arg0, class... Args>
		struct TemplateOf<T<U, Arg0, Args...>>
		{
			template<template<class...> class NewU, class NewArg0, class... NewArgs>
				requires(sizeof...(NewArgs) == sizeof...(Args))
			using type = T<NewU, NewArg0, NewArgs...>;
		};

		template<template<template<class...> class, auto, auto...> class T, template<class...> class U, auto Value0, auto... Values>
		struct TemplateOf<T<U, Value0, Values...>>
		{
			template<template<class...> class NewU, auto NewValue0, auto... NewValues>
				requires(sizeof...(NewValues) == sizeof...(Values))
			using type = T<NewU, NewValue0, NewValues...>;
		};

		template<template<template<template<class...> class> class, class, class...> class T, template<template<class...> class> class U, class Arg0, class... Args>
		struct TemplateOf<T<U, Arg0, Args...>>
		{
			template<template<template<class...> class> class NewU, class NewArg0, class... NewArgs>
				requires(sizeof...(NewArgs) == sizeof...(Args))
			using type = T<NewU, NewArg0, NewArgs...>;
		};

		template<template<template<template<class...> class> class, auto, auto...> class T, template<template<class...> class> class U, auto Value0, auto... Values>
		struct TemplateOf<T<U, Value0, Values...>>
		{
			template<template<template<class...> class> class NewU, auto NewValue0, auto... NewValues>
				requires(sizeof...(NewValues) == sizeof...(Values))
			using type = T<NewU, NewValue0, NewValues...>;
		};

		template<template<template<auto...> class> class T, template<auto...> class U>
		struct TemplateOf<T<U>>
		{
			template<template<auto...> class NewU>
			using type = T<NewU>;
		};

		template<template<template<auto...> class, class, class...> class T, template<auto...> class U, class Arg0, class... Args>
		struct TemplateOf<T<U, Arg0, Args...>>
		{
			template<template<auto...> class NewU, class NewArg0, class... NewArgs>
				requires(sizeof...(NewArgs) == sizeof...(Args))
			using type = T<NewU, NewArg0, NewArgs...>;
		};

		template<template<template<auto...> class, auto, auto...> class T, template<auto...> class U, auto Value0, auto... Values>
		struct TemplateOf<T<U, Value0, Values...>>
		{
			template<template<auto...> class NewU, auto NewValue0, auto... NewValues>
				requires(sizeof...(NewValues) == sizeof...(Values))
			using type = T<NewU, NewValue0, NewValues...>;
		};

		template<class T>
		inline constexpr bool IsTemplateInstantiation = requires {
			sizeof(nox::meta::detail::TemplateOf<T>);
		};
	}
}