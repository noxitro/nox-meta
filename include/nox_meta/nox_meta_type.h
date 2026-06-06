//	Copyright (C) 2026 NOX ENGINE All rights reserved.

///	@file	type.h
///	@brief	type
#pragma once
#include	<string_view>
#include	<span>
#include	<memory>
#include	"nox_meta_utility.h"
#include	"nox_meta_function_signature.h"
#include	"nox_meta_type_traits.h"

namespace nox::meta
{
	//	forward declaration
	struct IType;
	class Type;
	inline constexpr const nox::meta::Type& GetInvalidType()noexcept;

	template<class T>
	[[nodiscard]]
	inline constexpr const nox::meta::Type& Typeof()noexcept;

	[[nodiscard]]
	inline constexpr  bool operator==(const nox::meta::IType& a, const nox::meta::IType& b)noexcept;

	constexpr std::size_t k_max_pointer_depth = 1;

	namespace detail
	{
		struct TypeDesc
		{
			nox::meta::TypeKind kind;
			nox::meta::TypeAttributeFlag attribute_flags;
			std::size_t size;
			std::size_t alignment;
			std::uint16_t array_rank;
			std::uint32_t array_extent;
			std::string_view name;
			void* (* const create_object)();
			void* (* const create_object_placement)(void*);
			void (* const destroy_at)(void*);

			bool (*is_convertible_functor)(const nox::meta::Type&, const nox::meta::Type&)noexcept;

			std::uint8_t argument_length;
			std::span<const std::reference_wrapper<const nox::meta::Type>>(* const get_argument_type_list)(const Type& self)noexcept;
			const nox::meta::Type& remove_pointer_type;
			const nox::meta::Type& result_type;
			const nox::meta::Type& remove_element_type;
			const nox::meta::Type& remove_all_element_type;
			const nox::meta::Type& underlying_type;
			const nox::meta::Type& add_const_type;
			const nox::meta::Type& remove_const_type;
			const nox::meta::Type& add_volatile_type;
			const nox::meta::Type& remove_volatile_type;
			const nox::meta::Type& remove_reference_type;
			const nox::meta::Type& add_lvalue_reference_type;
			const nox::meta::Type& add_rvalue_reference_type;
			const nox::meta::Type& remove_all_modifiers_type;
			const nox::meta::Type& owner_type;
			const nox::meta::Type& add_pointer_type;
		};

		enum class DerivedOption : std::uint8_t
		{
			AddConst,
			RemoveConst,

			RemovePointer,
			RemoveExtentType,
		};

		template<class T>
		inline constexpr const nox::meta::Type& GetRemovePointerType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetResultType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetRemoveExtentType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetRemoveAllExtentType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetUnderlyingType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetAddConstType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetRemoveConstType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetAddVolatileType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetRemoveVolatileType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetRemoveReferenceType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetAddLValueReferenceType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetAddRValueReferenceType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetRemoveAllModifiersType()noexcept;

		template<nox::meta::function_signature::concepts::FunctionSignatureType T>
		inline constexpr std::array<std::reference_wrapper<const nox::meta::Type>, nox::meta::function_signature::FunctionArgsLength<T>> GetArgumentTypeList()noexcept;

		template<class T>
		inline constexpr const nox::meta::Type& GetOwnerType()noexcept;
		template<class T>
		inline constexpr const nox::meta::Type& GetAddPointerType()noexcept;

		template<class T>
		inline constexpr const nox::meta::Type& GetDecayType()noexcept
		{
			if constexpr (std::is_same_v<T, std::decay_t<T>> == false)
			{
				return nox::meta::Typeof<std::decay_t<T>>();
			}
			else 
			{
				return nox::meta::GetInvalidType();
			}
		}

		/// @brief 無駄に増やさないためのダミー
		inline constexpr std::span<const std::reference_wrapper<const nox::meta::Type>> GetArgumentTypeListInvalid([[maybe_unused]] const nox::meta::Type&)noexcept
		{
			return {};
		}

		template <class _Ty>
			requires requires(_Ty* _Location) {
			::new (static_cast<void*>(_Location)) _Ty(); // per LWG-3888
		}
		constexpr void* CreateObject(_Ty* const _Location)
			noexcept(noexcept(::new (static_cast<void*>(_Location)) _Ty())) /* strengthened */ {
			return ::new (static_cast<void*>(_Location)) _Ty();
		}

	/*	template<class T> requires(std::is_default_constructible_v<T> && requires{ []()constexpr noexcept->void {T* _ = new T(); }(); })
		inline constexpr void* CreateObject() noexcept(noexcept(new T()))
		{
			return static_cast<void*>(const_cast<std::remove_extent_t<std::remove_cv_t<T>>*>(new T()));
		}*/

		template<class T>
		inline constexpr void* CreateObject() noexcept
		{
			return nullptr;
		}

		template<class T> requires(std::is_default_constructible_v<T>&& requires{ []()constexpr noexcept->void {T* _ = new T(); }(); })
			inline constexpr void* CreateObjectAlignment(std::align_val_t alignment) noexcept(noexcept(new(alignment) T()))
		{
			return static_cast<void*>(const_cast<std::remove_extent_t<std::remove_cv_t<T>>*>(new(alignment) T()));
		}

		template<class T>
		inline constexpr void* CreateObjectAlignment(std::align_val_t) noexcept
		{
			return nullptr;
		}

		template<class T>
		concept IsConstructAt = requires(T* p)
		{
			[]()noexcept->void {T* _ = new(nullptr) T(); }();
		};

		template<class T> requires(
			std::is_default_constructible_v<T> && 
			std::is_invocable_v<decltype(static_cast<T*(*)(T*)>(std::construct_at)), std::add_pointer_t<T>>&&
			requires { []()constexpr noexcept -> void { T* _ = std::construct_at(static_cast<std::add_pointer_t<T>>(nullptr)); }(); })
		inline constexpr void* ConstructAt(T* const storage) noexcept(noexcept(new((void*)(storage)) T()))
		{
//			T* p = new(static_cast<void*>(const_cast<std::remove_extent_t<std::remove_cv_t<T>>*>(storage))) T();
//			return static_cast<void*>(const_cast<std::remove_extent_t<std::remove_cv_t<T>>*>(p));

			return std::construct_at(storage);
		}

		template<class T>
		inline constexpr void* ConstructAt(T*const)noexcept
		{
			return nullptr;
		}

		template<class T> requires(std::is_destructible_v<T> && requires(T* p) { std::destroy_at(p); })
			inline constexpr void DestroyAt(T*const storage) noexcept(noexcept(std::destroy_at(storage)))
		{
			std::destroy_at(storage);
		}

		template<class T>
		inline constexpr void DestroyAt(T* const)noexcept
		{
			return;
		}
	}

	struct IType
	{
		//inline constexpr IType()noexcept = default;
		//inline constexpr ~IType()noexcept = default;
		//inline constexpr IType(const IType&)noexcept = delete;
		//inline constexpr IType(IType&&)noexcept = delete;

		//inline constexpr IType& operator=(const IType&)noexcept = delete;
	};

	class Type : nox::meta::IType
	{
	public:
		[[nodiscard]]
		inline constexpr explicit Type(const nox::meta::detail::TypeDesc& desc)noexcept :
			kind_(desc.kind),
			attribute_flags_(desc.attribute_flags),
			size_(desc.size),
			alignment_(desc.alignment),
			array_rank_(desc.array_rank),
			array_extent_(desc.array_extent),
			name_(desc.name),
			create_object_(desc.create_object),
			create_object_placement_(desc.create_object_placement),
			destroy_at_(desc.destroy_at),
			is_convertible_functor_(desc.is_convertible_functor),
			argument_length_(desc.argument_length),
			get_argument_type_list_(desc.get_argument_type_list),
			remove_pointer_type_(desc.remove_pointer_type),
			result_type_(desc.result_type),
			remove_element_type_(desc.remove_element_type),
			remove_all_element_type_(desc.remove_all_element_type),
			underlying_type_(desc.underlying_type),
			add_const_type_(desc.add_const_type),
			remove_const_type_(desc.remove_const_type),
			add_volatile_type_(desc.add_volatile_type),
			remove_volatile_type_(desc.remove_volatile_type),
			remove_reference_type_(desc.remove_reference_type),
			add_lvalue_reference_type_(desc.add_lvalue_reference_type),
			add_rvalue_reference_type_(desc.add_rvalue_reference_type),
			remove_all_modifiers_type_(desc.remove_all_modifiers_type),
			owner_type_(desc.owner_type),
			add_pointer_type_(desc.add_pointer_type)
		{
		}

		inline constexpr Type()noexcept = delete;
		inline constexpr Type(const Type&)noexcept = delete;
		inline constexpr Type(Type&&)noexcept = delete;

		inline constexpr Type& operator=(const Type&)noexcept = delete;

		[[nodiscard]] inline constexpr nox::meta::TypeKind GetKind()const noexcept { return kind_; }
		[[nodiscard]] inline constexpr nox::meta::TypeAttributeFlag GetAttributeFlags()const noexcept { return attribute_flags_; }
		[[nodiscard]] inline constexpr std::size_t GetSize()const noexcept { return size_; }
		[[nodiscard]] inline constexpr std::size_t GetAlignment()const noexcept { return alignment_; }
		[[nodiscard]] inline constexpr std::uint16_t GetArrayRank()const noexcept { return array_rank_; }
		[[nodiscard]] inline constexpr std::uint32_t GetArrayExtent()const noexcept { return array_extent_; }
		[[nodiscard]] inline constexpr std::string_view GetName()const noexcept { return name_; }
		[[nodiscard]] inline constexpr void* CreateObject()const noexcept { return create_object_(); }
		[[nodiscard]] inline constexpr void* CreateObject(std::align_val_t alignment)const noexcept { return create_object_(); }
		[[nodiscard]] inline constexpr void* CreateObject(std::align_val_t alignment, const std::nothrow_t&)const noexcept { return create_object_(); }
		[[nodiscard]] inline constexpr void* CreateObject(const std::nothrow_t&)const noexcept { return create_object_(); }
		[[nodiscard]] inline constexpr void* CreateObjectAt(void* const storage)const noexcept { return create_object_placement_(storage); }
		inline constexpr void DestroyAt(void* const object)const noexcept { destroy_at_(object); }
		[[nodiscard]] inline constexpr bool IsConvertibleTo(const nox::meta::Type& to)const noexcept { return is_convertible_functor_(*this, to); }
		[[nodiscard]] inline constexpr std::uint8_t GetArgumentLength()const noexcept { return argument_length_; }
		[[nodiscard]] inline constexpr std::span<const std::reference_wrapper<const nox::meta::Type>> GetArgumentTypeList()const noexcept { return get_argument_type_list_(*this); }
		[[nodiscard]] inline constexpr const nox::meta::Type& RemovePointerType()const noexcept { return remove_pointer_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& ResultType()const noexcept { return result_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& RemoveElementType()const noexcept { return remove_element_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& RemoveAllElementType()const noexcept { return remove_all_element_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& UnderlyingType()const noexcept { return underlying_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& AddConstType()const noexcept { return add_const_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& RemoveConstType()const noexcept { return remove_const_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& AddVolatileType()const noexcept { return add_volatile_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& RemoveVolatileType()const noexcept { return remove_volatile_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& RemoveReferenceType()const noexcept { return remove_reference_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& AddLValueReferenceType()const noexcept { return add_lvalue_reference_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& AddRValueReferenceType()const noexcept { return add_rvalue_reference_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& RemoveAllModifiersType()const noexcept { return remove_all_modifiers_type_; }
		[[nodiscard]] inline constexpr const nox::meta::Type& GetAddPointerType()const noexcept { return add_pointer_type_; }
//		[[nodiscard]] inline constexpr bool IsTypeAttributeFlag(nox::meta::TypeAttributeFlag flag)const noexcept { return  }

#pragma region attributes
		[[nodiscard]] inline constexpr bool HasAttributeFlag(const nox::meta::TypeAttributeFlag flag)const noexcept
		{
			return (nox::meta::detail::ToUnderlying(attribute_flags_) & nox::meta::detail::ToUnderlying(flag)) != 0;
		}

		//	qualifiers
		[[nodiscard]] inline constexpr bool IsConst()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Const); }
		[[nodiscard]] inline constexpr bool IsVolatile()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Volatile); }

		//	class attributes
		[[nodiscard]] inline constexpr bool IsFinal()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Final); }
		[[nodiscard]] inline constexpr bool IsAbstract()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Abstract); }
		[[nodiscard]] inline constexpr bool IsPolymorphic()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Polymorphic); }
		[[nodiscard]] inline constexpr bool IsAggregate()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Aggregate); }
		[[nodiscard]] inline constexpr bool IsTrivial()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Trivial); }
		[[nodiscard]] inline constexpr bool IsStandardLayout()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::StandardLayout); }
		[[nodiscard]] inline constexpr bool IsEmpty()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Empty); }
		[[nodiscard]] inline constexpr bool IsTriviallyCopyable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::TriviallyCopyable); }

		//	signed / unsigned
		[[nodiscard]] inline constexpr bool IsSigned()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Signed); }
		[[nodiscard]] inline constexpr bool IsUnsigned()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Unsigned); }

		//	constructibility
		[[nodiscard]] inline constexpr bool IsDefaultConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::DefaultConstructible); }
		[[nodiscard]] inline constexpr bool IsTriviallyDefaultConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::TriviallyDefaultConstructible); }
		[[nodiscard]] inline constexpr bool IsNothrowDefaultConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::NothrowDefaultConstructible); }
		[[nodiscard]] inline constexpr bool IsCopyConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::CopyConstructible); }
		[[nodiscard]] inline constexpr bool IsTriviallyCopyConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::TriviallyCopyConstructible); }
		[[nodiscard]] inline constexpr bool IsNothrowCopyConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::NothrowCopyConstructible); }
		[[nodiscard]] inline constexpr bool IsMoveConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::MoveConstructible); }
		[[nodiscard]] inline constexpr bool IsTriviallyMoveConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::TriviallyMoveConstructible); }
		[[nodiscard]] inline constexpr bool IsNothrowMoveConstructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::NothrowMoveConstructible); }

		//	destructibility
		[[nodiscard]] inline constexpr bool IsDestructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Destructible); }
		[[nodiscard]] inline constexpr bool IsTriviallyDestructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::TriviallyDestructible); }
		[[nodiscard]] inline constexpr bool IsNothrowDestructible()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::NothrowDestructible); }
		[[nodiscard]] inline constexpr bool HasVirtualDestructor()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::HasVirtualDestructor); }

		//	assignability
		[[nodiscard]] inline constexpr bool IsCopyAssignable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::CopyAssignable); }
		[[nodiscard]] inline constexpr bool IsTriviallyCopyAssignable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::TriviallyCopyAssignable); }
		[[nodiscard]] inline constexpr bool IsNothrowCopyAssignable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::NothrowCopyAssignable); }
		[[nodiscard]] inline constexpr bool IsMoveAssignable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::MoveAssignable); }
		[[nodiscard]] inline constexpr bool IsTriviallyMoveAssignable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::TriviallyMoveAssignable); }
		[[nodiscard]] inline constexpr bool IsNothrowMoveAssignable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::NothrowMoveAssignable); }

		//	swappability
		[[nodiscard]] inline constexpr bool IsSwappable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::Swappable); }
		[[nodiscard]] inline constexpr bool IsNothrowSwappable()const noexcept { return HasAttributeFlag(nox::meta::TypeAttributeFlag::NothrowSwappable); }

		//	basic type
		[[nodiscard]] inline constexpr bool IsIntegral()const noexcept
		{
			switch (kind_)
			{
			case nox::meta::TypeKind::Bool:
			case nox::meta::TypeKind::Char:
			case nox::meta::TypeKind::Char8:
			case nox::meta::TypeKind::Char16:
			case nox::meta::TypeKind::Char32:
			case nox::meta::TypeKind::WideChar:
			case nox::meta::TypeKind::Int8:
			case nox::meta::TypeKind::UInt8:
			case nox::meta::TypeKind::Int16:
			case nox::meta::TypeKind::UInt16:
			case nox::meta::TypeKind::Int32:
			case nox::meta::TypeKind::UInt32:
			case nox::meta::TypeKind::Int64:
			case nox::meta::TypeKind::UInt64:
			case nox::meta::TypeKind::Int128:
			case nox::meta::TypeKind::UInt128:
				return true;
			default:
				return false;
			}
		}

		[[nodiscard]] inline constexpr bool IsFloatingPoint()const noexcept
		{
			switch (kind_)
			{
			case nox::meta::TypeKind::Float:
			case nox::meta::TypeKind::Double:
			case nox::meta::TypeKind::LongDouble:
				return true;
			default:
				return false;
			}
		}

		[[nodiscard]] inline constexpr bool IsArithmetic()const noexcept
		{
			return IsIntegral() || IsFloatingPoint();
		}

		[[nodiscard]] inline constexpr bool IsScalar()const noexcept
		{
			if (IsArithmetic())
			{
				return true;
			}

			switch (kind_)
			{
			case nox::meta::TypeKind::Enum:
			case nox::meta::TypeKind::ScopedEnum:
			case nox::meta::TypeKind::Pointer:
			case nox::meta::TypeKind::MemberFunctionPointer:
			case nox::meta::TypeKind::MemberObjectPointer:
				return true;
			default:
				return false;
			}
		}

		[[nodiscard]] inline constexpr bool IsObject()const noexcept
		{
			if (IsScalar())
			{
				return true;
			}
			switch (kind_)
			{
			case nox::meta::TypeKind::Class:
			case nox::meta::TypeKind::Union:
				return true;
			default:
				return false;
			}
		}

		[[nodiscard]] inline constexpr bool IsMemberPointer()const noexcept
		{
			return kind_ == nox::meta::TypeKind::MemberFunctionPointer || kind_ == nox::meta::TypeKind::MemberObjectPointer;
		}

		[[nodiscard]] inline constexpr bool IsReference()const noexcept
		{
			return kind_ == nox::meta::TypeKind::LValueReference || kind_ == nox::meta::TypeKind::RValueReference;
		}


#pragma endregion

	private:
		[[nodiscard]] inline constexpr const nox::meta::Type& GetDerivedType(const nox::meta::detail::DerivedOption option)const noexcept
		{
			return result_type_;
		}

	private:
	
		std::size_t size_;
		std::size_t alignment_;
		nox::meta::TypeKind kind_;
		nox::meta::TypeAttributeFlag attribute_flags_;
		std::uint16_t array_rank_;
		std::uint32_t array_extent_;
		std::string_view name_;

		void* (* const create_object_)();
		void* (* const create_object_placement_)(void*);
		void (* const destroy_at_)(void*);
		bool (*is_convertible_functor_)(const nox::meta::Type&, const nox::meta::Type&)noexcept;
		std::uint8_t argument_length_;
		std::span<const std::reference_wrapper<const nox::meta::Type>>(* const get_argument_type_list_)(const Type& self)noexcept;
		const nox::meta::Type& add_pointer_type_;
		const nox::meta::Type& remove_pointer_type_;
		const nox::meta::Type& result_type_;
		const nox::meta::Type& remove_element_type_;
		const nox::meta::Type& remove_all_element_type_;
		const nox::meta::Type& underlying_type_;
		const nox::meta::Type& add_const_type_;
		const nox::meta::Type& remove_const_type_;
		const nox::meta::Type& add_volatile_type_;
		const nox::meta::Type& remove_volatile_type_;
		const nox::meta::Type& remove_reference_type_;
		const nox::meta::Type& add_lvalue_reference_type_;
		const nox::meta::Type& add_rvalue_reference_type_;
		const nox::meta::Type& remove_all_modifiers_type_;
		const nox::meta::Type& owner_type_;
	};

	[[nodiscard]]
	inline constexpr bool operator==(const nox::meta::IType& a, const nox::meta::IType& b)noexcept 
	{
		return &a == &b; 
	}

	namespace detail
	{
		class InvalidType final : public nox::meta::Type
		{
		public:
#pragma warning(push)
#pragma warning(disable:4355) // self-reference in base initializer is intentional for invalid sentinel
			[[nodiscard]]
			inline constexpr InvalidType()noexcept :
				nox::meta::Type(
					nox::meta::detail::TypeDesc{
						.kind = nox::meta::TypeKind::Unknown,
						.attribute_flags = nox::meta::TypeAttributeFlag::None,
						.size = 0,
						.alignment = 0,
						.array_rank = 0,
						.array_extent = 0,
						.name = "",
						.create_object = +[]()->void* { return nullptr; },
						.create_object_placement = +[](void*)->void* {return nullptr; },
						.destroy_at = +[](void*) {},
						.is_convertible_functor = +[](const nox::meta::Type&, const nox::meta::Type&)constexpr noexcept -> bool { return false; },
						.argument_length = 0,
						.get_argument_type_list = &nox::meta::detail::GetArgumentTypeListInvalid,
						.remove_pointer_type = *this,
						.result_type = *this,
						.remove_element_type = *this,
						.remove_all_element_type = *this,
						.underlying_type = *this,
						.add_const_type = *this,
						.remove_const_type = *this,
						.add_volatile_type = *this,
						.remove_volatile_type = *this,
						.remove_reference_type = *this,
						.add_lvalue_reference_type = *this,
						.add_rvalue_reference_type = *this,
						.remove_all_modifiers_type = *this,
						.owner_type = *this,
						.add_pointer_type = *this
					}
				)
			{
			}
#pragma warning(pop)
			inline constexpr InvalidType(const InvalidType&)noexcept = delete;
			inline constexpr InvalidType(InvalidType&&)noexcept = delete;

			inline constexpr InvalidType& operator=(const InvalidType&)noexcept = delete;

		};

		template<class T>
		class TypeImpl : public nox::meta::Type
		{
		public:
			inline constexpr TypeImpl()noexcept :
				TypeImpl(0, &nox::meta::detail::GetArgumentTypeListInvalid)
			{
			}

			inline constexpr TypeImpl(const TypeImpl&)noexcept = delete;
			inline constexpr TypeImpl(TypeImpl&&)noexcept = delete;

			inline constexpr TypeImpl& operator=(const TypeImpl&)noexcept = delete;

			[[nodiscard]]
			inline constexpr explicit TypeImpl(
				const std::uint8_t argument_length,
				std::span<const std::reference_wrapper<const nox::meta::Type>>(* const get_argument_type_list)(const nox::meta::Type& self)noexcept = &nox::meta::detail::GetArgumentTypeListInvalid
			)noexcept:
				nox::meta::Type(
					nox::meta::detail::TypeDesc{
						.kind = nox::meta::GetTypeKind<T>(),
						.attribute_flags = nox::meta::GetTypeAttributeFlags<T>(),
						.size = nox::meta::util::SafeSizeof<T>(),
						.alignment = nox::meta::util::SafeAlignof<T>(),
						.array_rank = std::rank_v<T>,
						.array_extent = std::extent_v<T>,
						.name = nox::meta::detail::GetTypeName<T>(),
						.create_object = &TypeImpl::CreateObject,
						.create_object_placement = &TypeImpl::ConstructAt,
						.destroy_at = &TypeImpl::DestroyAt,
						.is_convertible_functor = &TypeImpl::IsConvertibleImpl,
						.argument_length = argument_length,
						.get_argument_type_list = get_argument_type_list,
						.remove_pointer_type = nox::meta::detail::GetRemovePointerType<T>(),
						.result_type = nox::meta::detail::GetResultType<T>(),
						.remove_element_type = nox::meta::detail::GetRemoveExtentType<T>(),
						.remove_all_element_type = nox::meta::detail::GetRemoveAllExtentType<T>(),
						.underlying_type = nox::meta::detail::GetUnderlyingType<T>(),
						.add_const_type = nox::meta::detail::GetAddConstType<T>(),
						.remove_const_type = nox::meta::detail::GetRemoveConstType<T>(),
						.add_volatile_type = nox::meta::detail::GetAddVolatileType<T>(),
						.remove_volatile_type = nox::meta::detail::GetRemoveVolatileType<T>(),
						.remove_reference_type = nox::meta::detail::GetRemoveReferenceType<T>(),
						.add_lvalue_reference_type = nox::meta::detail::GetAddLValueReferenceType<T>(),
						.add_rvalue_reference_type = nox::meta::detail::GetAddRValueReferenceType<T>(),
						.remove_all_modifiers_type = nox::meta::detail::GetRemoveAllModifiersType<T>(),
						.owner_type = nox::meta::detail::GetOwnerType<T>(),
						.add_pointer_type = nox::meta::detail::GetAddPointerType<T>()
					}
				)
			{
			}

		private:
			static inline constexpr void* CreateObject()noexcept(noexcept(nox::meta::detail::CreateObject<T>()))
			{
				return nox::meta::detail::CreateObject<T>();
			}

			static inline constexpr void* ConstructAt(void*const storage)//noexcept(noexcept(nox::meta::detail::ConstructAt(static_cast<std::conditional_t<std::is_array_v<T>, std::decay_t<T>, std::add_pointer_t<T>>>(storage))))
			{
				using TT = std::add_pointer_t<T>;
				
				//return nox::meta::detail::ConstructAt(static_cast<std::conditional_t<std::is_array_v<T>, std::decay_t<T>, std::add_pointer_t<T>>>(storage));
				return nullptr;
			/*	if constexpr (std::is_invocable_v<decltype(static_cast<void* (*)(std::add_pointer_t<T>)>(&nox::meta::detail::ConstructAt)), std::add_pointer_t<T>> == true)
				{
					return nox::meta::detail::ConstructAt(static_cast<TT>(storage));
				}
				else
				{
					return nullptr;
				}*/
			}

			static inline constexpr void DestroyAt(void*const object)
				noexcept(noexcept(nox::meta::detail::DestroyAt(static_cast<std::conditional_t<std::is_array_v<T>, std::decay_t<T>, std::add_pointer_t<T>>>(object))))
			{
				nox::meta::detail::DestroyAt(static_cast<std::conditional_t<std::is_array_v<T>, std::decay_t<T>, std::add_pointer_t<T>>>(object));
			}

			static inline constexpr bool IsConvertibleImpl(const nox::meta::Type& , const nox::meta::Type& )noexcept
			{
				return false;
			}

			static inline constexpr const nox::meta::Type& GetDerivedOption(nox::meta::detail::DerivedOption option)noexcept
			{
				switch (option)
				{
				case nox::meta::detail::DerivedOption::AddConst:
					return nox::meta::detail::GetAddConstType<T>();
				}
			}
		};

		template<class T>
		class TypeFunction : public nox::meta::detail::TypeImpl<T>
		{

		};

		template<template<class...> class T>
		struct TemplateTypeImpl_0 : nox::meta::IType
		{
		public:
			
		};

		template<template<template<class...> class, class...> class T>
		struct TemplateTypeImpl_1 : nox::meta::IType
		{
		public:

		};

		template<template<template<template<class...> class> class, class...> class T>
		struct TemplateTypeImpl_2 : nox::meta::IType
		{
		public:

		};

		template<class, template<template<class...> class, class...> class T>
		struct TemplateTypeImpl_0_1 : nox::meta::IType
		{
		public:

		};

		template<class, template<template<template<class...> class> class, class...> class T>
		struct TemplateTypeImpl_0_2 : nox::meta::IType
		{
		public:

		};

		template<template<class...> class T>
		struct TemplateTypeHolder_0
		{
			static constexpr nox::meta::detail::TemplateTypeImpl_0<T> value{};
		};

		template<template<template<class...> class, class...> class T>
		struct TemplateTypeHolder_1
		{
			static constexpr nox::meta::detail::TemplateTypeImpl_1<T> value{};
		};
	}

	namespace detail
	{
		
	}
	
	namespace detail
	{
		static constexpr nox::meta::detail::InvalidType k_invalid_type{};

		template<class T>
		struct TypeHolder
		{
			static constexpr nox::meta::detail::TypeImpl<T> value{};

			constexpr TypeHolder() noexcept = delete;
			constexpr ~TypeHolder() noexcept = delete;
		};
	}

	[[nodiscard]]
	inline constexpr const nox::meta::Type& nox::meta::GetInvalidType()noexcept
	{
		return nox::meta::detail::k_invalid_type;
	}

	//	template types...
	template<template<class...> class T>
	[[nodiscard]]
	inline constexpr const nox::meta::IType& Typeof()noexcept
	{
		return nox::meta::detail::TemplateTypeHolder_0<T>::value;
	}

	template<template<template<class...> class, class...> class T>
	[[nodiscard]]
	inline constexpr const nox::meta::IType& Typeof()noexcept
	{
		return nox::meta::detail::TemplateTypeImpl_1<T>::value;
	}
}

template<class T>
[[nodiscard]]
inline constexpr const nox::meta::Type& nox::meta::Typeof()noexcept
{
	return nox::meta::detail::TypeHolder<T>::value;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetRemovePointerType()noexcept
{
	if constexpr (std::is_pointer_v<T>)
	{
		return nox::meta::detail::TypeHolder<std::remove_pointer_t<T>>::value;
	}
	else
	{
		return nox::meta::detail::k_invalid_type;
	}
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetResultType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetRemoveExtentType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetRemoveAllExtentType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetUnderlyingType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetAddConstType()noexcept
{
	if constexpr (std::is_const_v<T>)
	{
		return nox::meta::detail::k_invalid_type;
	}
	else
	{
		return nox::meta::Typeof<std::add_const_t<T>>();
	}
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetRemoveConstType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetAddVolatileType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetRemoveVolatileType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetRemoveReferenceType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetAddLValueReferenceType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetAddRValueReferenceType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetRemoveAllModifiersType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<nox::meta::function_signature::concepts::FunctionSignatureType T>
inline constexpr std::array<std::reference_wrapper<const nox::meta::Type>, nox::meta::function_signature::FunctionArgsLength<T>> nox::meta::detail::GetArgumentTypeList()noexcept
{
	return {};
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetOwnerType()noexcept
{
	return nox::meta::detail::k_invalid_type;
}

template<class T>
inline constexpr const nox::meta::Type& nox::meta::detail::GetAddPointerType()noexcept
{
	if constexpr (nox::meta::detail::GetPointerDepth<T>() <= nox::meta::k_max_pointer_depth)
	{
		return nox::meta::Typeof<std::add_pointer_t<T>>();
	}
	else
	{
		return nox::meta::GetInvalidType();
	}
}
