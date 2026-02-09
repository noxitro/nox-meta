//	Copyright (C) 2026 NOX ENGINE All rights reserved.

///	@file	definition.h
///	@brief	definition
#pragma once
#include	<cstdint>
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
		SignedChar,
		UnsignedChar,
		Char8,
		Char16,
		Char32,
		WideChar,
		Int8 = SignedChar,
		UInt8 = UnsignedChar,
		Int16,
		UInt16,
		Int32,
		UInt32,
		Int64,
		UInt64,
		Float,
		Double,
		LongDouble,
		Enum,
		ScopedEnum,
		Class,
		Union,
		Function,
		FunctionPointer,
		MemberFunctionPointer,
		Pointer,
		LValueReference,
		RValueReference,
		Array,
		UnboundedArray,
		Nullptr,
	};

	enum class TypeAttributeFlag : std::uint32_t 
	{
		None = 0,
		Const = 1 << 0,
		Volatile = 1 << 1,
		Final = 1 << 2,
		Abstract = 1 << 3,
		Polymorphic = 1 << 4,
		Aggregate = 1 << 5,
		Trivial = 1 << 6,
		StandardLayout = 1 << 7,
		Empty = 1 << 8,
		Swappable = 1 << 9,
		Constructible = 1 << 10,
		DefaultConstructible = 1 << 11,
		TriviallyConstructible = 1 << 12,
		CopyConstructible = 1 << 13,
		TriviallyCopyConstructible = 1 << 14,
		MoveConstructible = 1 << 15,
		TriviallyMoveConstructible = 1 << 16,

	};
	
}