#include "hashtable.hpp"

namespace pycpp
{

#pragma region Definition : HashTable

	#pragma region Constructors & Destructors

	HashTable::HashTable(std::initializer_list<Object*> list) :
		__HashTable(list) {}

	HashTable::HashTable(HashTable& source) :
		__HashTable(source) {}

	HashTable::HashTable(Iterable* iterable) :
		__HashTable(iterable) {}

	#pragma endregion

	#pragma region Override Object

	i64 HashTable::type()
	{
		return HashTable::type_id;
	}

	const char* HashTable::class_name()
	{
		return "HashTable";
	}

	u32 HashTable::bytes()
	{
		return sizeof(HashTable);
	}

	u64 HashTable::hash()
	{
		return 0;
	}

	void HashTable::output()
	{
		std::cout << '{';
		__HashTable::output();
		std::cout << '}';
	}

	Object* HashTable::copy()
	{
		return new HashTable(*this);
	}

	bool HashTable::eq(Object*)
	{
		// TODO
		return true;
	}

	Object* HashTable::add(Object* other)
	{
		auto iterable = dynamic_cast<Iterable*>(other);
		if (iterable == nullptr)
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in HashTable::add : the add operator cannot be used"
				"to add HashTable with %s !",
				other->class_name());

			std::cout << content_error << std::endl;
			throw std::exception("Error : cannot union a HashTable with an Object that is not Iterable!");

			return nullptr;
		}

		__HashTable::extend(iterable);
		// Don't need to SAFE "iterable"
		return this;
	}

	#pragma endregion

	#pragma region Override Iterable

	Iterator* HashTable::first()
	{
		return __HashTable::first();
	}

	Iterator* HashTable::last()
	{
		return __HashTable::last();
	}

	#pragma endregion

	#pragma region Override Container

	void HashTable::clear()
	{
		__HashTable::clear();
	}

	bool HashTable::contains(Object* elem)
	{
		return __HashTable::contains(elem, elem->hash());
	}

	bool HashTable::push(Object* item)
	{
		return __HashTable::push(item);
	}

	Object* HashTable::pop()
	{
		return __HashTable::pop();
	}

	void HashTable::extend(Iterable* iterable)
	{
		__HashTable::extend(iterable);
	}

	void HashTable::extend(std::initializer_list<Object*> list)
	{
		__HashTable::extend(list);
	}

	#pragma endregion

	#pragma region Override Set

	Object* HashTable::get(Object* elem)
	{
		return __HashTable::get(elem);
	}

	#pragma endregion

#pragma endregion

}