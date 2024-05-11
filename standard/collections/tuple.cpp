#include "tuple.hpp"
#include "../base/algo.hpp"

namespace pycpp
{

#pragma region Definition : Tuple

	#pragma region Tuple

		#pragma region Constructors & Destructors

	Tuple::Tuple() : hash_value(pycpp::hash_array(data, len)) {}

	Tuple::Tuple(const Tuple& source) :
		__Array(&source), hash_value(source.hash_value) {}

	Tuple::Tuple(std::initializer_list<Object*> list) :
		__Array(list), hash_value(pycpp::hash_array(data, len)) {}

	Tuple::Tuple(Iterable* iterable) :
		__Array(iterable), hash_value(pycpp::hash_array(data, len)) {}

		#pragma endregion

		#pragma region Override : Object

	i64 Tuple::type()
	{
		return Tuple::type_id;
	}

	std::string Tuple::class_name()
	{
		return "Tuple";
	}

	u64 Tuple::hash()
	{
		return hash_value;
	}

	u32 Tuple::bytes()
	{
		u32 size_of_class = sizeof(Tuple);
		u32 size_of_array = sizeof(Object*) * capacity;

		u32 size_of_elements = 0u;
		for (u32 i = 0u; i < len; ++i)
			size_of_elements += data[i]->bytes();
		return size_of_class + size_of_array + size_of_elements;
	}

	void Tuple::output()
	{
		std::string content;
		output(content);
		std::cout << content;
	}

	void Tuple::output(std::string& out)
	{
		__Array::output(out);
		out = "(" + out + ")";
	}


	bool Tuple::equal(Object* other)
	{
		auto __array = dynamic_cast<__Array*>(other);
		if (__array)
			return __Array::equal(__array);

		auto arguments = dynamic_cast<Arguments*>(other);
		if (arguments)
			return Arguments::equal(arguments);

		// Không thể so sánh => Không bằng nhau
		SAFE(other);
		return false;
	}

		#pragma endregion

		#pragma region Override : Iterable

	bool Tuple::is_empty()
	{
		return len == 0u;
	}

	u32 Tuple::size()
	{
		return len;
	}

	Iterator* Tuple::first()
	{
		return __Array::first();
	}

	Iterator* Tuple::last()
	{
		return __Array::last();
	}

		#pragma endregion

		#pragma region Override : Arguments

	Object* Tuple::get(i32 index)
	{
		return __Array::get(index);
	}

	bool Tuple::contains(Object* elem)
	{
		bool result = false;

		for (u32 i = 0u; i < len; ++i)
			if (elem->equal(data[i]))
			{
				result = true;
				break;
			}

		SAFE(elem);
		return result;
	}

		#pragma endregion

	#pragma endregion

#pragma endregion

}