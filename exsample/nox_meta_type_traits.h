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
	}
}