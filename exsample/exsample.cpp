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

int main()
{
	constexpr const auto& v0000 = nox::meta::Typeof<nox::meta::detail::TemplateOf<std::vector<int>>::template type>();
	

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