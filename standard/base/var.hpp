#pragma once

#include "error.hpp"
#include "object.hpp"

#include <type_traits>

namespace pycpp
{

#pragma region Macro

#define SAME(U, T) std::is_same_v<U, T>
#define BASE_OF(base, derived) std::is_base_of_v<base, derived>
#define ENABLE_IF(expression) typename std::enable_if_t<expression, bool> = true


	inline void SAFE(const Object* object)
	{
		if (object->ref_count == 0u)
			delete object;
	}

	template <typename T, ENABLE_IF(!BASE_OF(Object, T))>
	inline void SAFE(const T* interface__)
	{
		// Note : mọi class cần phải kế thừa Object
		auto object = dynamic_cast<const Object*>(interface__);
		if (object->ref_count == 0u)
			delete object;
	}

	// Không kiểm tra nullptr.
	inline void INREF(const Object* object)
	{
		++object->ref_count;
	}

	// Không kiểm tra nullptr.
	template <typename T, ENABLE_IF(!BASE_OF(Object, T))>
	inline void INREF(const T* object)
	{
		// Note : mọi class cần phải kế thừa Object
		++ dynamic_cast<const Object*>(object)->ref_count;
	}


	// Không kiểm tra nullptr.
	inline void DEREF(const Object* object)
	{
		--object->ref_count;
	}

	// Không kiểm tra nullptr.
	template <typename T, ENABLE_IF(!BASE_OF(Object, T))>
	inline void DEREF(const T* object)
	{
		// Note : mọi class cần phải kế thừa Object
		-- dynamic_cast<const Object*>(object)->ref_count;
	}


	// T* REFERENCE<T, U>(U*) with 7 case : with U is not T.
	// Acronyms :
	//		`<-` : is base of.
	//		`<X` : is not base of.
	// - U <- T && T <- Object => U <- Object. (1)
	// - U <X T && T <- Object && U <- Object. (2)
	// - U <- T && T <X Object && U <- Object. (3)
	// - U <- T && T <- Object && U <X Object. ( Delete case )
	// - U <X T && T <X Object && U <- Object. (5)
	// - U <X T && T <- Object && U <X Object. (6)
	// - U <- T && T <X Object && U <X Object. (7)
	// - U <X T && T <X Object && U <X Object. (8)
	// Group by same case : [1, 3], [2, 5], [6], [7], [8]

	// Case [1, 3]
	template<typename T, typename U,
		ENABLE_IF(!SAME(T, U) && BASE_OF(T, U) && BASE_OF(Object, U))>
	inline T* REFERENCE(U* object)
	{
		/*
		U kế thừa Object.
		U kế thừa T.
		
		(1) Nếu 'object' != nullptr :
			(1.1) Tăng tham chiếu 'object'
			(1.2) Trả về 'object'
		(2) Trả về nullptr
		*/

		if (object != nullptr)	// (1)
		{
			INREF(object);		// (1.1)
			return object;		// (1.2)
		}

		return nullptr;			// (2)
	}

	// Case [2, 5]
	template<typename T, typename U,
		ENABLE_IF(!SAME(T, U) && !BASE_OF(T, U) && BASE_OF(Object, U))>
	inline T* REFERENCE(U* object)
	{
		/*
		U 'object' kế thừa từ Object.
		T là Interface.
		
		(1) Nếu 'object' != nullptr :
		{
			(1.1) result = cast<T>(object)
			(1.2) Nếu 'result' != nullptr :
				(1.2.1) Tăng tham chiếu 'object'
			(1.3) Else
			{
				(1.3.1) Giải phóng 'object'
				(1.3.2) Warning : lost object reference
			}

			(1.5) Trả về 'result'
		}

		(2) Trả về nullptr
		*/

		if (object != nullptr)	// (1)
		{
			auto result = dynamic_cast<T*>(object);	// (1.1)

			if (result != nullptr)	// (1.2)
				INREF(object);
			else 	// (1.3)
			{
				SAFE(object);	// (1.3.1)
				__pycpp_warning_lost_object_reference<U, T>();	// (1.3.2)
			}
			return result;		// (1.5)
		}

		return nullptr;	// (2)
	}

	// Case [6]
	template<typename T, typename U,
		ENABLE_IF(!SAME(T, U) && !BASE_OF(T, U) && BASE_OF(Object, T) && !BASE_OF(Object, U))>
	inline T* REFERENCE(U* interface__)
	{
		/*
		T kế thừa từ Object.
		U là Interface.

		(1) Nếu 'interface__' != nullptr :
		{
			(1.1) result = cast<T>(interface__)
			(1.2) If 'result' != nullptr :
				(1.2.1) Tăng tham chiếu 'result'
			(1.3) Else :
			{
				(1.3.1) Giải phóng 'interface__'
				(1.3.2) Warning : lost object reference
			}
			(1.3) Trả về 'result'
		}

		(2) Trả về nullptr
		*/

		if (interface__ != nullptr)	// (1.)
		{
			auto result = dynamic_cast<T*>(interface__);	// (1.1)

			if (result != nullptr)	// (1.2)
				INREF(result);	// (1.2.1)
			else // (1.3)
			{
				// Note : mọi class buộc phải kế thừa Object
				SAFE(interface__);	// (1.3.1)
				__pycpp_warning_lost_object_reference<U, T>();	// (1.3.2)
			}

			return result;
		}

		return nullptr;
	}

	// Case [7]
	template<typename T, typename U,
		ENABLE_IF(!SAME(T, U) && BASE_OF(T, U) && !BASE_OF(Object, U))>
	inline T* REFERENCE(U* interface__)
	{
		/*
		U là Interface.
		U kế thừa T.

		(1) Nếu 'interface__' != nullptr :
		{
			(1.1) Tăng tham chiếu 'interface__'
			(1.2) Trả về 'interface__'
		}

		(2) Trả về nullptr
		*/

		if (interface__ != nullptr)	// (1)
		{
			// Note : mọi class buộc phải kế thừa Object
			INREF(interface__);	// (1.1)
			return interface__;	// (1.2)
		}

		return nullptr;	// (2)
	}

	// Case [8]
	template<typename T, typename U,
		ENABLE_IF(!SAME(T, U) && !BASE_OF(T, U) &&
		!BASE_OF(Object, T) && !BASE_OF(Object, U))>
	inline T* REFERENCE(U* interface__)
	{
		/*
		T, U là Interface.

		(1) Nếu 'interface__' != nullptr :
		{
			(1.1) object = cast<Object>(interface__)
			(1.2) Tăng tham chiếu 'object'

			(1.3) result = cast<T>(interface__)
			(1.4) Nếu 'result' == nullptr :
			{
				(1.4.1) Giải phóng 'object'
				(1.4.2) Warning : lost object reference
			}

			(1.5) Trả về 'result';
		}

		(2) Trả về nullptr
		*/

		if (interface__ != nullptr)	// (1)
		{
			// Note : mọi class buộc phải kế thừa Object
			auto object = dynamic_cast<Object*>(interface__);	// (1.1)
			INREF(object);	// (1.2)

			auto result = dynamic_cast<T*>(interface__);	// (1.3)
			if (result == nullptr)	// (1.4)
			{
				SAFE(object);	// (1.4.1)
				__pycpp_warning_lost_object_reference<U, T>();	// (1.4.2)
			}

			return result;	// (1.5)
		}

		return nullptr;	// (2)
	}


	template<typename T, ENABLE_IF(BASE_OF(Object, T))>
	inline void DEREFERENCE(T*& reference)
	{
		if (reference == nullptr)
			return;

		DEREF(reference);
		SAFE(reference);
		reference = nullptr;
	}

	template<typename T, ENABLE_IF(!BASE_OF(Object, T))>
	inline void DEREFERENCE(T*& reference)
	{
		if (reference == nullptr)
			return;

		// Note : mọi class buộc phải kế thừa Object
		auto object = dynamic_cast<Object*>(reference);
		reference = nullptr;
		DEREF(object);
		SAFE(object);
	}

#pragma endregion

#pragma region Var Definition

	template <class T>
	class Var final
	{
		T* ref;

	public:

#pragma region Constructors & Destructors

		inline Var() : ref(nullptr) {}

		~Var()
		{
			safe();
		}

		inline Var(T* object) : ref(object)
		{
			INREF(object);
		}


		inline Var(Var& source) : ref(source.ref)
		{
			INREF(ref);
		}

#pragma region Var<T>(U*)

		// Case [1, 3]
		template<class U, ENABLE_IF(!SAME(T, U) && BASE_OF(T, U))>// && BASE_OF(Object, U))>
		inline Var(U* object) : ref(object)
		{
			if (object == nullptr)
				return;

			INREF(object);
		}

		// Case [2, 5]
		template<class U, ENABLE_IF(!SAME(T, U) && !BASE_OF(T, U) && BASE_OF(Object, U))>
		inline Var(U* object) : ref(nullptr)
		{
			if (object == nullptr)
				return;

			ref = dynamic_cast<T*>(object);

			// Lost reference to object
			if (ref == nullptr)
			{
				SAFE(object);
				__pycpp_warning_lost_object_reference<U, T>();
			}
			else
				INREF(object);
		}

		// Case [6]
		template<class U, ENABLE_IF(!SAME(T, U) && !BASE_OF(T, U) && BASE_OF(Object, T) && !BASE_OF(Object, U))>
		inline Var(U* object) : ref(nullptr)
		{
			if (object == nullptr)
				return;

			ref = dynamic_cast<T*>(object);

			// Lost reference to object
			if (ref != nullptr)
				INREF(ref);
			else
			{
				SAFE(object);
				__pycpp_warning_lost_object_reference<U, T>();
			}
		}

		// Case [7]
		/*template<class U, ENABLE_IF(!SAME(T, U) && BASE_OF(T, U) && !BASE_OF(Object, U))>
		inline Var(U* object) : ref(object)
		{
			if (object == nullptr)
				return;

			INREF(object);
		}*/

		// Case [8]
		template<typename T, typename U,
			ENABLE_IF(!SAME(T, U) && !BASE_OF(T, U) &&
				!BASE_OF(Object, T) && !BASE_OF(Object, U))>
		inline Var(U* object) : ref(nullptr)
		{
			if (object == nullptr)
				return;

			ref = dynamic_cast<T*>(object);

			// Lost reference to object
			if (ref == nullptr)
				INREF(object);
			else
			{
				SAFE(object);
				__pycpp_warning_lost_object_reference<U, T>();
			}
		}

#pragma endregion

		template <class U, ENABLE_IF(!SAME(T, U))>
		inline Var(Var<U> source) : ref(REFERENCE(source.ref)) {}


#pragma endregion

#pragma region Methods Definition

	private:

		inline void safe()
		{
			DEREFERENCE(ref);
		}

	public:

		template <class U, ENABLE_IF(!SAME(U, T) && BASE_OF(U, T))>
		inline U* bind()
		{
			if (ref == nullptr)
				return nullptr;
			return ref;
		}

		template <class U, ENABLE_IF(!SAME(U, T) && !BASE_OF(U, T))>
		inline U* bind()
		{
			U* binding = dynamic_cast<U*>(ref);
			return binding;
		}

#pragma endregion

#pragma region Overloading Operator Definition

		inline T* operator->()
		{
			return ref;
		}

		inline operator bool()
		{
			return ref != nullptr;
		}

		inline T* operator&()
		{
			return ref;
		}


		inline bool operator==(Var& source)
		{
			return ref == source.ref;
		}


		inline Var& operator=(T* object)
		{
			safe();

			ref = REFERENCE(object);
			return *this;
		}

		inline Var& operator=(Var& source)
		{
			safe();

			ref = REFERENCE(source.ref);
			return *this;
		}

		template <class U, ENABLE_IF(!SAME(T, U))>
		inline Var& operator=(U* object)
		{
			safe();

			ref = REFERENCE(object);
			return *this;
		}

		template <class U, ENABLE_IF(!SAME(T, U))>
		inline Var& operator=(Var<U>& source)
		{
			safe();

			ref = REFERENCE(source.ref);
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& os, Var& source)
		{
			if (source)
				source.bind<Object>()->output();
			else
				std::cout << "None";
			return os;
		}

#pragma endregion

	};

#pragma endregion

}