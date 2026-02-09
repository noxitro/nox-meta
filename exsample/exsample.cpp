//	Copyright (c) 2026 NOX ENGINE All rights reserved.

///	@file	exsample.cpp
///	@brief	exsample
#include	"exsample.h"
#include	<vector>
#include	"../include/nox_meta/nox_meta_type.h"

namespace nox
{
	template<class T>
	class IntrusivePtr
	{
	public:
		constexpr IntrusivePtr() noexcept :
			ptr_(nullptr)
		{

		}

		inline constexpr IntrusivePtr(T* ptr) noexcept :
			ptr_(ptr)
		{
			IntrusivePtrAddRef(ptr_);
		}

		inline constexpr IntrusivePtr(std::nullptr_t) noexcept :
			ptr_(nullptr)
		{
		}
		
		constexpr T* get() noexcept
		{
			return ptr_;
		}


	private:
		T* ptr_;
	};
}

namespace my
{
	struct Object
	{
		int ref_count = 0;
	};

	void IntrusivePtrAddRef(Object* obj) noexcept
	{
		if (obj)
		{
			++obj->ref_count;
		}
	}
}

int main()
{
	nox::IntrusivePtr<my::Object> objPtr = new my::Object();

	//constexpr decltype(auto) v = nox::meta::GetInvalidType();

	//constexpr decltype(auto) t = nox::meta::Typeof<int>();
	//constexpr decltype(auto) t2 = nox::meta::Typeof<std::reference_wrapper>();

	//auto& v2 = t;
	return 0;
}