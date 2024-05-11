#pragma once

#include "private_fixedarray.hpp"

namespace pycpp
{

#pragma region Declaration : Fixed Array

	template<u32 Capacity>
	class FixedArray final : public List, public __FixedArray<Capacity>
	{
	public:
		TYPE_ID(FixedArray<Capacity>);

	#pragma region Constructors & Destructor

		FixedArray() = default;
		FixedArray(std::initializer_list<Object*> list) :
			__FixedArray<Capacity>(list) {}
		FixedArray(Iterable* iterable) :
			__FixedArray<Capacity>(iterable) {}

		FixedArray(FixedArray& source) : __FixedArray(&source) {}

		~FixedArray() = default;

	#pragma endregion

	#pragma region Override Object

		i64 type() override
		{
			return type_id;
		}

		std::string class_name() override
		{
			return typeid(FixedArray<Capacity>).name();
		}

		void output() override
		{
			__FixedArray<Capacity>::output();
		}

		void output(std::string& out) override
		{
			__FixedArray<Capacity>::output(out);
		}

		u32 bytes() override
		{
			u32 size_of_this = sizeof(FixedArray<Capacity>);

			u32 size_of_elements = 0u;
			for (u32 i = 0u; i < __FixedArray<Capacity>::len; ++i)
				size_of_elements += __FixedArray<Capacity>::data[i]->bytes();

			return size_of_this + size_of_elements;
		}

		u64 hash() override
		{
			return pycpp::hash_array(__FixedArray<Capacity>::data, __FixedArray<Capacity>::len);
		}


		bool equal(Object* other) override
		{
			auto arguments = dynamic_cast<Arguments*>(other);
			if (arguments == nullptr)
				return false;

			auto __array = dynamic_cast<__Array*>(other);
			if (__array)
				return __FixedArray<Capacity>::equal(__array);
			return Arguments::equal(arguments);
		}

		i32 rich_compare(Object* other) override
		{
			auto arguments = dynamic_cast<Arguments*>(other);
			if (arguments == nullptr)
			{
				char content_error[128];
				sprintf_s(content_error,
					"Error in FixedArray<%d>::rich_compare : "
					"the compare operator cannot be used"
					"to compare FixedArray with %s !",
					Capacity, other->class_name().c_str());

				std::cout << content_error << std::endl;
				throw std::exception("Error: comparing FixedArray with a different data type other than Arguments");

				return false;
			}

			auto __array = dynamic_cast<__Array*>(other);
			if (__array)
				return __FixedArray<Capacity>::rich_compare(__array);
			return Arguments::rich_compare(arguments);
		}


		Object* add(Object* other) override
		{
			auto iterable = dynamic_cast<Iterable*>(other);
			if (iterable == nullptr)
			{
				char content_error[128];
				std::sprintf(content_error,
					"Error in FixedArray<%d>::add : the add operator cannot be used"
					"to add FixedArray with %s !",
					Capacity, other->class_name().c_str());

				std::cout << content_error << std::endl;
				throw std::exception("Error : cannot concatenate a FixedArray with an Object that is not Iterable!");

				return nullptr;
			}

			__FixedArray<Capacity>::extend(iterable);
			// Don't need to SAFE "iterable"
			return this;
		}

		Object* mul(Object* other) override
		{
			i64 other_type = other->type();
			i32 num;

			switch (other_type)
			{
			case I32::type_id:
				num = dynamic_cast<I32*>(other)->value;
				break;

			case I64::type_id:
				num = (i32) dynamic_cast<I64*>(other)->value;
				break;

			default:
			{
				char content_error[128];
				std::sprintf(content_error,
					"Error in FixedArray<%d>::mul : the mul operator cannot be used"
					"to multiply ArrayList with %s !",
					Capacity, other->class_name().c_str());

				std::cout << content_error << std::endl;
				throw std::exception("Error : operator with invalid data type");
			}
			}

			// Error : multiplying FixedList with by a negative number
			if (num < 0)
			{
				char content_error[128];
				std::sprintf(content_error,
					"Error in FixedArray<%d>::mul : the mul operator cannot be used"
					"to multiply FixedArray with negative number of %s !",
					Capacity, other->class_name().c_str());

				std::cout << content_error << std::endl;
				throw std::exception("Error : multiplying a list by a negative number");
			}

			__FixedArray<Capacity>::multiply_with_int(u32(num));
			SAFE(other);
			return this;
		}

	#pragma endregion

	#pragma region Override Iterable

		Iterator* first() override
		{
			return __FixedArray<Capacity>::first();
		}

		Iterator* last() override
		{
			return __FixedArray<Capacity>::last();
		}

	#pragma endregion

	#pragma region Override Arguments

		bool is_empty() override
		{
			return __FixedArray<Capacity>::len == 0u;
		}

		u32 size() override
		{
			return __FixedArray<Capacity>::len;
		}

		Object* get(i32 index) override
		{
			return __FixedArray<Capacity>::get(index);
		}

	#pragma endregion

	#pragma region Override Container

		void clear() override
		{
			__FixedArray<Capacity>::clear();
		}

		bool contains(Object* elem) override
		{
			return __FixedArray<Capacity>::contains(elem);
		}

		bool push(Object* item) override
		{
			__FixedArray<Capacity>::push(item);
			return true;
		}

		Object* pop() override
		{
			return __FixedArray<Capacity>::pop(-1);
		}

		void extend(Iterable* iterable) override
		{
			__FixedArray<Capacity>::extend(iterable);
			// Don't need to SAFE "iterable"
		}

	#pragma endregion

#pragma region Override List

		u32 capacity() override
		{
			return __FixedArray<Capacity>::capacity;
		}

		void insert(i32 index, Object* item) override
		{
			__FixedArray<Capacity>::insert(index, item);
		}

		Object* pop(i32 index) override
		{
			return __FixedArray<Capacity>::pop(index);
		}

		void remove(i32 index) override
		{
			__FixedArray<Capacity>::remove(index);
		}

		void sort() override
		{
			__FixedArray<Capacity>::sort();
		}

#pragma endregion

	};

#pragma endregion

}