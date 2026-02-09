//	Copyright (C) 2026 NOX ENGINE All rights reserved.

///	@file	type.h
///	@brief	type
#pragma once
#include	<string_view>
#include	<span>
#include	<memory>
#include	"nox_meta_utility.h"
#include	"nox_meta_function_signature.h"

namespace nox::meta
{
	//	forward declaration
	struct IType;
	class Type;
	inline constexpr const nox::meta::Type& GetInvalidType()noexcept;
	[[nodiscard]]
	inline constexpr  bool operator==(const nox::meta::IType& a, const nox::meta::IType& b)noexcept;

	namespace detail
	{
		struct TypeDesc
		{
			TypeKind kind;
			TypeAttributeFlag attribute_flags;
			std::uint32_t size;
			std::uint32_t alignment;
			std::uint16_t array_rank;
			std::uint32_t array_extent;
			std::string_view name;
			void* (& const create_object)();
			void* (& const create_object_placement)(void*);
			void (& const destroy_at)(void*);

			bool (*is_convertible_functor)(const nox::meta::Type&, const nox::meta::Type&)noexcept;

			std::uint8_t argument_length;
			std::span<const std::reference_wrapper<const nox::meta::Type>>(* const get_argument_type_list)(const Type& self)noexcept;
			const Type& remove_pointer_type;
			const Type& result_type;
			const Type& remove_element_type;
			const Type& remove_all_element_type;
			const Type& underlying_type;
			const Type& add_const_type;
			const Type& remove_const_type;
			const Type& add_volatile_type;
			const Type& remove_volatile_type;
			const Type& remove_reference_type;
			const Type& add_lvalue_reference_type;
			const Type& add_rvalue_reference_type;
			const Type& remove_all_modifiers_type;
			const Type& owner_type;
		};

		template<class T>
		inline constexpr const Type& GetRemovePointerType()noexcept;
		template<class T>
		inline constexpr const Type& GetResultType()noexcept;
		template<class T>
		inline constexpr const Type& GetRemoveExtentType()noexcept;
		template<class T>
		inline constexpr const Type& GetRemoveAllExtentType()noexcept;
		template<class T>
		inline constexpr const Type& GetUnderlyingType()noexcept;
		template<class T>
		inline constexpr const Type& GetAddConstType()noexcept;
		template<class T>
		inline constexpr const Type& GetRemoveConstType()noexcept;
		template<class T>
		inline constexpr const Type& GetAddVolatileType()noexcept;
		template<class T>
		inline constexpr const Type& GetRemoveVolatileType()noexcept;
		template<class T>
		inline constexpr const Type& GetRemoveReferenceType()noexcept;
		template<class T>
		inline constexpr const Type& GetAddLValueReferenceType()noexcept;
		template<class T>
		inline constexpr const Type& GetAddRValueReferenceType()noexcept;
		template<class T>
		inline constexpr const Type& GetRemoveAllModifiersType()noexcept;

		template<nox::meta::function_signature::concepts::FunctionSignatureType T>
		inline constexpr std::array<std::reference_wrapper<const nox::meta::Type>, nox::meta::function_signature::FunctionArgsLength<T>> GetArgumentTypeList()noexcept;

		template<class T>
		inline constexpr const Type& GetOwnerType()noexcept;

		/// @brief 無駄に増やさないためのダミー
		inline constexpr std::span<const std::reference_wrapper<const nox::meta::Type>> GetArgumentTypeListInvalid([[maybe_unused]] const nox::meta::Type&)noexcept
		{
			return {};
		}
	}

	struct IType
	{
		inline constexpr IType()noexcept = default;
		inline constexpr ~IType()noexcept = default;
		inline constexpr IType(const IType&)noexcept = delete;
		inline constexpr IType(IType&&)noexcept = delete;

		inline constexpr IType& operator=(const IType&)noexcept = delete;
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
			owner_type_(desc.owner_type)
		{
		}

		inline constexpr Type()noexcept = delete;
		inline constexpr Type(const Type&)noexcept = delete;
		inline constexpr Type(Type&&)noexcept = delete;

		inline constexpr Type& operator=(const Type&)noexcept = delete;

		[[nodiscard]] inline constexpr nox::meta::TypeKind GetKind()const noexcept { return kind_; }
		[[nodiscard]] inline constexpr nox::meta::TypeAttributeFlag GetAttributeFlags()const noexcept { return attribute_flags_; }
		[[nodiscard]] inline constexpr std::uint32_t GetSize()const noexcept { return size_; }
		[[nodiscard]] inline constexpr std::uint32_t GetAlignment()const noexcept { return alignment_; }
		[[nodiscard]] inline constexpr std::uint16_t GetArrayRank()const noexcept { return array_rank_; }
		[[nodiscard]] inline constexpr std::uint32_t GetArrayExtent()const noexcept { return array_extent_; }
		[[nodiscard]] inline constexpr std::string_view GetName()const noexcept { return name_; }
		[[nodiscard]] inline constexpr void* CreateObject()const noexcept { return create_object_(); }
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

	private:
		nox::meta::TypeKind kind_;
		nox::meta::TypeAttributeFlag attribute_flags_;
		std::uint32_t size_;
		std::uint32_t alignment_;

		std::uint16_t array_rank_;
		std::uint32_t array_extent_;
		std::string_view name_;

		void* (& const create_object_)();
		void* (& const create_object_placement_)(void*);
		void (& const destroy_at_)(void*);
		bool (*is_convertible_functor_)(const nox::meta::Type&, const nox::meta::Type&)noexcept;
		std::uint8_t argument_length_;
		std::span<const std::reference_wrapper<const nox::meta::Type>>(* const get_argument_type_list_)(const Type& self)noexcept;
		const Type& remove_pointer_type_;
		const Type& result_type_;
		const Type& remove_element_type_;
		const Type& remove_all_element_type_;
		const Type& underlying_type_;
		const Type& add_const_type_;
		const Type& remove_const_type_;
		const Type& add_volatile_type_;
		const Type& remove_volatile_type_;
		const Type& remove_reference_type_;
		const Type& add_lvalue_reference_type_;
		const Type& add_rvalue_reference_type_;
		const Type& remove_all_modifiers_type_;
		const Type& owner_type_;
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
						.create_object = *+[]()->void* { return nullptr; },
						.create_object_placement = *+[](void*)->void* {return nullptr; },
						.destroy_at = *+[](void*) {},
						.is_convertible_functor = +[](const nox::meta::Type& a, const nox::meta::Type& b)noexcept -> bool { return false; },
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
						.owner_type = *this
					}
				)
			{
			}
		};

		template<class T>
		class TypeImpl : public nox::meta::Type
		{
		public:
			[[nodiscard]]
			inline constexpr explicit TypeImpl(
				const std::uint8_t argument_length = 0,
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
						.owner_type = nox::meta::detail::GetOwnerType<T>()
					}
				)
			{

			}

			inline constexpr TypeImpl()noexcept:
				TypeImpl(0, &nox::meta::detail::GetArgumentTypeListInvalid)
			{
			}

		private:
			static inline constexpr void* CreateObject()noexcept(noexcept(new T()))
			{
				if constexpr (std::is_constructible_v<T>)
				{
					return static_cast<void*>(new T());
				}
				else
				{
					return nullptr;
				}
			}

			static inline constexpr void* ConstructAt(void*const storage)noexcept(noexcept(std::construct_at(static_cast<T*>(storage))))
			{
				if constexpr (std::is_constructible_v<T>)
				{
					return std::construct_at(static_cast<T*>(storage));
				}
				else
				{
					return nullptr;
				}
			}

			static inline constexpr void DestroyAt(void*const object)noexcept(noexcept(std::destroy_at(static_cast<T*>(object))))
			{
				if constexpr (std::is_destructible_v<T>)
				{
					std::destroy_at(static_cast<T*>(object));
				}
			}

			static inline constexpr bool IsConvertibleImpl(const nox::meta::Type& a, const nox::meta::Type& b)noexcept
			{
				return false;
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
		struct TypeHolder;

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

	template<class T>
	[[nodiscard]]
	inline constexpr const nox::meta::Type& Typeof()noexcept
	{
		return nox::meta::detail::TypeHolder<T>::value;
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
inline constexpr const nox::meta::Type& nox::meta::detail::GetRemovePointerType()noexcept
{
	return nox::meta::detail::k_invalid_type;
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
	return nox::meta::detail::k_invalid_type;
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