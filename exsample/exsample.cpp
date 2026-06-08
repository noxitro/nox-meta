//	Copyright (c) 2026 NOX ENGINE All rights reserved.

///	@file	exsample.cpp
///	@brief	exsample
#include	"exsample.h"
#include	<vector>
#include	"../include/nox_meta/nox_meta_type.h"

class Test
{
public:
	constexpr Test()noexcept {}

	int value;
};

class AbstractTest
{
public:
	constexpr AbstractTest()noexcept {}

	virtual void F() = 0;
};


struct INewDeleteDisabled
{
	static void* operator new(std::size_t) = delete;
	static void* operator new(std::size_t, const std::nothrow_t&) = delete;
	static void* operator new(std::size_t, std::align_val_t) = delete;
	static void* operator new(std::size_t, std::align_val_t, const std::nothrow_t&) = delete;

	static void* operator new[](std::size_t) = delete;
	static void* operator new[](std::size_t, const std::nothrow_t&) = delete;
	static void* operator new[](std::size_t, std::align_val_t) = delete;
	static void* operator new[](std::size_t, std::align_val_t, const std::nothrow_t&) = delete;

	static void  operator delete(void*) = delete;
	static void  operator delete[](void*) = delete;
};


// 実行時にデフォルト new 可能か（consteval コンストラクタのみの型は false）
template<class T>
concept RuntimeDefaultNewable = std::is_constructible_v<T> && requires
{
	[]()constexpr noexcept->void {T* _ = new T(); }();
};


template <class _Ty>
	requires requires(_Ty* _Location) {
	::new (static_cast<void*>(_Location)) _Ty(); // per LWG-3888
}
constexpr _Ty* construct_at(_Ty* const _Location)
noexcept(noexcept(::new (static_cast<void*>(_Location)) _Ty())) /* strengthened */ {
	return ::new (static_cast<void*>(_Location)) _Ty();
}

template<template<class...> class T>
class TemplateClassA {};

template<template<template<class...>class> class T>
class TemplateClassB {};

template<auto>
class TemplateClass0 {};

template<auto Value0, class Type0, auto Value1, class Type1>
class TemplateClassEx {};

template<auto Value0, class Type0, class... Types>
class TemplateClassAutoTypePack {};

template<class Type0, auto Value0, auto... Values>
class TemplateClassTypeAutoPack {};

template<class Type0, auto Value0, class Type1, class... Types>
class TemplateClassTypeAutoTypePack {};

template<auto Value0, class Type0, auto Value1, auto... Values>
class TemplateClassAutoTypeAutoPack {};

template<class Type0, auto Value0, class Type1, auto Value1, auto... Values>
class TemplateClassTypeAutoTypeAutoPack {};

template<template<class...> class TemplateParam, class Type0, class... Types>
class TemplateClassTemplateTypePack {};

template<template<class...> class TemplateParam, auto Value0, auto... Values>
class TemplateClassTemplateAutoPack {};

template<template<template<class...> class> class TemplateParam, class Type0, class... Types>
class TemplateClassTemplateTemplateTypePack {};

template<template<template<class...> class> class TemplateParam, auto Value0, auto... Values>
class TemplateClassTemplateTemplateAutoPack {};

template<template<auto...> class TemplateParam>
class TemplateClassTemplateAutoOnly {};

template<template<auto...> class TemplateParam, class Type0, class... Types>
class TemplateClassTemplateAutoTypePack {};

template<template<auto...> class TemplateParam, auto Value0, auto... Values>
class TemplateClassTemplateAutoValuePack {};

using TemplateClassExRebind = nox::meta::detail::TemplateOf<TemplateClassEx<1, int, 2, float>>::type<3, double, 4, char>;
using TemplateClassAutoTypePackRebind = nox::meta::detail::TemplateOf<TemplateClassAutoTypePack<1, int, float>>::type<2, double, char>;
using TemplateClassTypeAutoPackRebind = nox::meta::detail::TemplateOf<TemplateClassTypeAutoPack<int, 1, 2>>::type<double, 3, 4>;
using TemplateClassTypeAutoTypePackRebind = nox::meta::detail::TemplateOf<TemplateClassTypeAutoTypePack<int, 1, float, double>>::type<char, 2, short, long>;
using TemplateClassAutoTypeAutoPackRebind = nox::meta::detail::TemplateOf<TemplateClassAutoTypeAutoPack<1, int, 2, 3>>::type<4, double, 5, 6>;
using TemplateClassTypeAutoTypeAutoPackRebind = nox::meta::detail::TemplateOf<TemplateClassTypeAutoTypeAutoPack<int, 1, float, 2, 3>>::type<char, 4, double, 5, 6>;
using TemplateClassTemplateTypePackRebind = nox::meta::detail::TemplateOf<TemplateClassTemplateTypePack<std::vector, int, float>>::type<std::tuple, char, double>;
using TemplateClassTemplateAutoPackRebind = nox::meta::detail::TemplateOf<TemplateClassTemplateAutoPack<std::vector, 1, 2>>::type<std::tuple, 3, 4>;
using TemplateClassTemplateTemplateTypePackRebind = nox::meta::detail::TemplateOf<TemplateClassTemplateTemplateTypePack<TemplateClassA, int, float>>::type<TemplateClassA, char, double>;
using TemplateClassTemplateTemplateAutoPackRebind = nox::meta::detail::TemplateOf<TemplateClassTemplateTemplateAutoPack<TemplateClassA, 1, 2>>::type<TemplateClassA, 3, 4>;
using TemplateClassTemplateAutoOnlyRebind = nox::meta::detail::TemplateOf<TemplateClassTemplateAutoOnly<TemplateClass0>>::type<TemplateClass0>;
using TemplateClassTemplateAutoTypePackRebind = nox::meta::detail::TemplateOf<TemplateClassTemplateAutoTypePack<TemplateClass0, int, float>>::type<TemplateClass0, char, double>;
using TemplateClassTemplateAutoValuePackRebind = nox::meta::detail::TemplateOf<TemplateClassTemplateAutoValuePack<TemplateClass0, 1, 2>>::type<TemplateClass0, 3, 4>;

static_assert(std::is_same_v<TemplateClassExRebind, TemplateClassEx<3, double, 4, char>>);
static_assert(std::is_same_v<TemplateClassAutoTypePackRebind, TemplateClassAutoTypePack<2, double, char>>);
static_assert(std::is_same_v<TemplateClassTypeAutoPackRebind, TemplateClassTypeAutoPack<double, 3, 4>>);
static_assert(std::is_same_v<TemplateClassTypeAutoTypePackRebind, TemplateClassTypeAutoTypePack<char, 2, short, long>>);
static_assert(std::is_same_v<TemplateClassAutoTypeAutoPackRebind, TemplateClassAutoTypeAutoPack<4, double, 5, 6>>);
static_assert(std::is_same_v<TemplateClassTypeAutoTypeAutoPackRebind, TemplateClassTypeAutoTypeAutoPack<char, 4, double, 5, 6>>);
static_assert(std::is_same_v<TemplateClassTemplateTypePackRebind, TemplateClassTemplateTypePack<std::tuple, char, double>>);
static_assert(std::is_same_v<TemplateClassTemplateAutoPackRebind, TemplateClassTemplateAutoPack<std::tuple, 3, 4>>);
static_assert(std::is_same_v<TemplateClassTemplateTemplateTypePackRebind, TemplateClassTemplateTemplateTypePack<TemplateClassA, char, double>>);
static_assert(std::is_same_v<TemplateClassTemplateTemplateAutoPackRebind, TemplateClassTemplateTemplateAutoPack<TemplateClassA, 3, 4>>);
static_assert(std::is_same_v<TemplateClassTemplateAutoOnlyRebind, TemplateClassTemplateAutoOnly<TemplateClass0>>);
static_assert(std::is_same_v<TemplateClassTemplateAutoTypePackRebind, TemplateClassTemplateAutoTypePack<TemplateClass0, char, double>>);
static_assert(std::is_same_v<TemplateClassTemplateAutoValuePackRebind, TemplateClassTemplateAutoValuePack<TemplateClass0, 3, 4>>);
static_assert(nox::meta::Typeof<std::vector>() == nox::meta::Typeof<std::vector<int>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClass0>() == nox::meta::Typeof<TemplateClass0<0>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassB>() == nox::meta::Typeof<TemplateClassB<TemplateClassA>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassEx>() == nox::meta::Typeof<TemplateClassEx<1, int, 2, float>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassAutoTypePack>() == nox::meta::Typeof<TemplateClassAutoTypePack<1, int, float>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTypeAutoPack>() == nox::meta::Typeof<TemplateClassTypeAutoPack<int, 1, 2>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTypeAutoTypePack>() == nox::meta::Typeof<TemplateClassTypeAutoTypePack<int, 1, float, double>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassAutoTypeAutoPack>() == nox::meta::Typeof<TemplateClassAutoTypeAutoPack<1, int, 2, 3>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTypeAutoTypeAutoPack>() == nox::meta::Typeof<TemplateClassTypeAutoTypeAutoPack<int, 1, float, 2, 3>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTemplateTypePack>() == nox::meta::Typeof<TemplateClassTemplateTypePack<std::vector, int, float>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTemplateAutoPack>() == nox::meta::Typeof<TemplateClassTemplateAutoPack<std::vector, 1, 2>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTemplateTemplateTypePack>() == nox::meta::Typeof<TemplateClassTemplateTemplateTypePack<TemplateClassA, int, float>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTemplateTemplateAutoPack>() == nox::meta::Typeof<TemplateClassTemplateTemplateAutoPack<TemplateClassA, 1, 2>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTemplateAutoOnly>() == nox::meta::Typeof<TemplateClassTemplateAutoOnly<TemplateClass0>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTemplateAutoTypePack>() == nox::meta::Typeof<TemplateClassTemplateAutoTypePack<TemplateClass0, int, float>>().GetTemplateOfType());
static_assert(nox::meta::Typeof<TemplateClassTemplateAutoValuePack>() == nox::meta::Typeof<TemplateClassTemplateAutoValuePack<TemplateClass0, 1, 2>>().GetTemplateOfType());
static_assert(nox::meta::detail::GetTypeName<TemplateClassEx>() == nox::meta::Typeof<TemplateClassEx>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassAutoTypePack>() == nox::meta::Typeof<TemplateClassAutoTypePack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTypeAutoPack>() == nox::meta::Typeof<TemplateClassTypeAutoPack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTypeAutoTypePack>() == nox::meta::Typeof<TemplateClassTypeAutoTypePack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassAutoTypeAutoPack>() == nox::meta::Typeof<TemplateClassAutoTypeAutoPack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTypeAutoTypeAutoPack>() == nox::meta::Typeof<TemplateClassTypeAutoTypeAutoPack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTemplateTypePack>() == nox::meta::Typeof<TemplateClassTemplateTypePack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTemplateAutoPack>() == nox::meta::Typeof<TemplateClassTemplateAutoPack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTemplateTemplateTypePack>() == nox::meta::Typeof<TemplateClassTemplateTemplateTypePack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTemplateTemplateAutoPack>() == nox::meta::Typeof<TemplateClassTemplateTemplateAutoPack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTemplateAutoOnly>() == nox::meta::Typeof<TemplateClassTemplateAutoOnly>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTemplateAutoTypePack>() == nox::meta::Typeof<TemplateClassTemplateAutoTypePack>().GetName());
static_assert(nox::meta::detail::GetTypeName<TemplateClassTemplateAutoValuePack>() == nox::meta::Typeof<TemplateClassTemplateAutoValuePack>().GetName());

int main()
{
	constexpr const auto test_name = nox::meta::detail::GetTypeName<std::vector>();
//	constexpr const auto test_name2 = nox::meta::detail::GetTypeName<TemplateClassA>();
//	constexpr const auto test_name3 = nox::meta::detail::GetTypeName<TemplateClassA>();
//	constexpr const auto test_name4 = nox::meta::detail::GetTypeName<TemplateClassA>();

	constexpr const auto& v0000 = nox::meta::Typeof<std::vector<int>>();
	constexpr const auto& v0001 = v0000.GetTemplateOfType();
	constexpr const auto& v0002 = nox::meta::Typeof<std::vector>();
	constexpr const auto& v0003 = nox::meta::Typeof<TemplateClassA>();
	constexpr const auto& v0004 = nox::meta::Typeof<TemplateClassB>();
	constexpr const auto& v0006 = nox::meta::Typeof<TemplateClassB<TemplateClassA>>();
	constexpr const auto& v0007 = nox::meta::Typeof<TemplateClass0>();
	bool vvvvv21 = (v0001 == nox::meta::Typeof<std::vector>());
	bool vvvvv22 = (v0004 == v0006);

	std::is_same<signed int, int>::value;
	
	constexpr auto kind = nox::meta::GetTypeKind<decltype([]() {return 2; }) > ();
	constexpr const auto& type99 = nox::meta::Typeof<decltype([]() {return 2; }) > ();

	char buffer[128] = { 0 };


	constexpr decltype(auto) r = nox::meta::Typeof<INewDeleteDisabled>();
	void*p = r.CreateObjectAt(buffer);
	void*p2 = r.CreateObjectAt(buffer);
	r.DestroyAt(buffer);
	constexpr auto nsese = nox::meta::detail::GetPointerDepth<int*>();

	//nox::meta::Typeof<int[]>();
	//nox::meta::Typeof<int[23]>();
	using a = std::decay_t<const volatile int[3]>;
	using b = std::decay_t<const int[]>;


	return 0;
}