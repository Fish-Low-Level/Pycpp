#include "arraylist.hpp"

// #define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

namespace pycpp
{

#pragma region Definition: Array List

	#pragma region Constructors & Destructors

	ArrayList::ArrayList(std::initializer_list<Object*> list) :
		__Array(list) {}

	ArrayList::ArrayList(ArrayList& source) : __Array(source) {}

	ArrayList::ArrayList(Iterable* iterable) : __Array(iterable) {}

	#pragma endregion

	#pragma region Standard Methods

	

	#pragma endregion

	#pragma region Override Object

	i64 ArrayList::type()
	{
		return ArrayList::type_id;
	}

	std::string ArrayList::class_name()
	{
		return "ArrayList";
	}

	u32 ArrayList::bytes()
	{
		u32 size_of_data = sizeof(Object*) * __Array::capacity;
		u32 size_of_this = sizeof(ArrayList);

		u32 size_of_elements = 0u;
		for (u32 i = 0u; i < __Array::len; ++i)
			size_of_elements += data[i]->bytes();

		return size_of_data + size_of_this + size_of_elements;
	}

	u64 ArrayList::hash()
	{
		return pycpp::hash_array(data, len);
	}

	void ArrayList::output()
	{
		std::string content;
		output(content);
		std::cout << content;
	}

	void ArrayList::output(std::string& out)
	{
		std::string output_array;
		__Array::output(output_array);

		out = "[" + output_array + "]";
	}


	bool ArrayList::equal(Object* other)
	{
		auto arguments = dynamic_cast<Arguments*>(other);
		if (arguments == nullptr)
			return false;

		auto __array = dynamic_cast<__Array*>(other);
		if (__array)
			return __Array::equal(__array);
		return Arguments::equal(arguments);
	}

	i32 ArrayList::rich_compare(Object* other)
	{
		auto arguments = dynamic_cast<Arguments*>(other);
		if (arguments == nullptr)
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in ArrayList::rich_compare : "
				"the compare operator cannot be used"
				"to compare ArrayList with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error: comparing ArrayList with a different data type other than Arguments");

			return 0;
		}

		auto __array = dynamic_cast<__Array*>(other);
		if (__array)
			return __Array::rich_compare(__array);
		return Arguments::rich_compare(arguments);
	}


	Object* ArrayList::add(Object* other)
	{
		auto iterable = dynamic_cast<Iterable*>(other);
		if (iterable == nullptr)
		{
			char content_error[128];
			std::sprintf(content_error,
				"Error in ArrayList::add : the add operator cannot be used"
				"to add ArrayList with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : cannot concatenate a ArrayList with an Object that is not Iterable!");

			return nullptr;
		}

		__Array::extend(iterable);
		// Don't need to SAFE "iterable"
		return this;
	}

	Object* ArrayList::mul(Object* other)
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
				"Error in ArrayList::mul : the mul operator cannot be used"
				"to multiply ArrayList with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}
		}

		// Error : multiplying ArrayList with by a negative number
		if (num < 0)
		{
			char content_error[128];
			std::sprintf(content_error,
				"Error in ArrayList::mul : the mul operator cannot be used"
				"to multiply ArrayList with negative number of %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : multiplying a list by a negative number");
		}

		__Array::multiply_with_int(u32(num));

		SAFE(other);
		return this;
	}

	#pragma endregion

	#pragma region Override Iterable

	Iterator* ArrayList::first()
	{
		return __Array::first();
	}

	Iterator* ArrayList::last()
	{
		return __Array::last();
	}

	#pragma endregion

	#pragma region Override Arguments

		bool ArrayList::is_empty()
		{
			return len == 0u;
		}

		u32 ArrayList::size()
		{
			return len;
		}

		Object* ArrayList::get(i32 index)
		{
			return __Array::get(index);
		}

	#pragma endregion

	#pragma region Override Container

	void ArrayList::clear()
	{
		__Array::clear();
	}

	bool ArrayList::contains(Object* elem)
	{
		return __Array::contains(elem);
	}

	bool ArrayList::push(Object* item)
	{
		__Array::push(item);
		return true;
	}

	Object* ArrayList::pop()
	{
		return __Array::pop(-1);
	}

	void ArrayList::extend(Iterable* iterable)
	{
		__Array::extend(iterable);
		// Don't need to SAFE "iterable"
	}

	#pragma endregion

	#pragma region Override List

	u32 ArrayList::capacity()
	{
		return __Array::capacity;
	}

	void ArrayList::set(i32 index, Object* item)
	{
		if (index < -i32(len) || index >= i32(len))
		{
			__pycpp_error_out_of_bound("ArrayList::set(i32 index, Object item)");
			return;
		}

		if (index < 0)
			index += len;
		auto deleting = data[index];
		DEREF(deleting);
		SAFE(deleting);

		INREF(item);
		data[index] = item;
	}

	void ArrayList::insert(i32 index, Object* item)
	{
		__Array::insert(index, item);
	}

	Object* ArrayList::pop(i32 index)
	{
		return __Array::pop(index);
	}

	void ArrayList::remove(i32 index)
	{
		__Array::remove(index);
	}

	void ArrayList::sort() 
	{
		__Array::sort();
	}

	#pragma endregion

#pragma endregion

}