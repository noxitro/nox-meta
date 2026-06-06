//	Copyright (C) 2026 NOX ENGINE All rights reserved.

///	@file	definition.h
///	@brief	definition
#pragma once
#include	<cstdint>
#include	<stdfloat>
#include	<cmath>
#include	<type_traits>

namespace nox::meta
{
	enum class TypeKind : std::uint8_t
	{
		Unknown,
		Void,
		Bool,
		Char,
		//	SignedChar = Int8,
		//	UnsignedChar = Uint8,
		Char8,
		Char16,
		Char32,
		WideChar,
		Int8,
		UInt8,
		Int16,
		UInt16,
		Int32,
		UInt32,
		Int64,
		UInt64,
		//	compiler extension
		Int128,
		UInt128,
		//	end compiler extension
		Float,
		Double,
		LongDouble,
		//	c++23
		Float16,
		BFloat16,
		Float128,
		//	end c++23
		Long,
		UnsignedLong,
		Enum,
		ScopedEnum,
		Class,
		Union,
		Function,
		FunctionPointer,
		MemberFunctionPointer,
		MemberObjectPointer,
		Pointer,
		LValueReference,
		RValueReference,
		Array,
		UnboundedArray,
		Nullptr,
	};

	enum class TypeAttributeFlag : std::uint64_t 
	{
		None = 0,

		//	qualifier
		Const = 1ULL << 0,
		Volatile = 1ULL << 1,

		//	class attributes
		Final = 1ULL << 2,
		Abstract = 1ULL << 3,
		Polymorphic = 1ULL << 4,
		Aggregate = 1ULL << 5,
		Trivial = 1ULL << 6,
		StandardLayout = 1ULL << 7,
		Empty = 1ULL << 8,
		TriviallyCopyable = 1ULL << 9, 

		Signed = 1ULL << 10, 
		Unsigned = 1ULL << 11,

		//	constructibility
		DefaultConstructible = 1ULL << 16,
		TriviallyDefaultConstructible = 1ULL << 17, 
		NothrowDefaultConstructible = 1ULL << 18, 
		CopyConstructible = 1ULL << 19,
		TriviallyCopyConstructible = 1ULL << 20,
		NothrowCopyConstructible = 1ULL << 21, 
		MoveConstructible = 1ULL << 22,
		TriviallyMoveConstructible = 1ULL << 23,
		NothrowMoveConstructible = 1ULL << 24, 

		//	destructibility
		Destructible = 1ULL << 28, 
		TriviallyDestructible = 1ULL << 29, 
		NothrowDestructible = 1ULL << 30, 
		HasVirtualDestructor = 1ULL << 31, 

		//	assignability
		CopyAssignable = 1ULL << 32, 
		TriviallyCopyAssignable = 1ULL << 33, 
		NothrowCopyAssignable = 1ULL << 34, 
		MoveAssignable = 1ULL << 35, 
		TriviallyMoveAssignable = 1ULL << 36, 
		NothrowMoveAssignable = 1ULL << 37, 

		//	swappability
		Swappable = 1ULL << 40,
		NothrowSwappable = 1ULL << 41, 
	};
	
}