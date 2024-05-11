#include "pair.hpp"
#include "var.hpp"

namespace pycpp
{

#pragma region Definition : Pair

	#pragma region Constructors & Destructors

	Pair::Pair() : __key(NONE), __value(NONE)
	{
		INREF(__key);
		INREF(__value);
	}

	Pair::Pair(Object* key) : __key(key), __value(NONE)
	{
		INREF(__key);
		INREF(__value);
	}

	Pair::Pair(Object* key, Object* value) : __key(key), __value(value)
	{
		INREF(__key);
		INREF(__value);
	}

	Pair::~Pair()
	{
		DEREF(__key);
		DEREF(__value);
		SAFE(__key);
		SAFE(__value);
	}

	#pragma endregion

	#pragma region Override : Object

	i64 Pair::type()
	{
		return Pair::type_id;
	}

	std::string Pair::class_name()
	{
		return "Pair";
	}

	void Pair::output()
	{
		std::string content;
		output(content);
		std::cout << content;
	}

	void Pair::output(std::string& out)
	{
		std::string key_content;
		std::string value_content;

		__key->output(key_content);
		if (__value == this)
			value_content = "this";
		else
			__value->output(value_content);

		char content[64];
		sprintf_s(content, "<%s, %s>",
			key_content.c_str(), value_content.c_str());
		out = content;
	}

	u32 Pair::bytes()
	{
		u32 size_of_this = sizeof(Pair);
		u32 size_of_key = __key->bytes();
		u32 size_of_value = (__value == this) ? 0u : __value->bytes();
		return size_of_this + size_of_key + size_of_value;
	}

	u64 Pair::hash()
	{
		return __key->hash();
	}

	Object* Pair::copy()
	{
		return new Pair(__key, __value);
	}


	bool Pair::equal(Object* other)
	{
		INREF(other);
		bool result = __key->equal(other);
		DEREF(other);

		SAFE(other);
		return result;
	}

	i32 Pair::rich_compare(Object* other)
	{
		INREF(other);
		i32 result = __key->rich_compare(other);
		DEREF(other);

		SAFE(other);
		return result;
	}

	#pragma endregion

	#pragma region Standard Methods

	void Pair::set(Object* value)
	{
		// Cần INREF tham số 'value' trước
		// tránh trường hợp 'value' chính là 'this->value'
		INREF(value);

		DEREF(this->__value);
		SAFE(this->__value);
		this->__value = value;
	}

	Object* Pair::key()
	{
		return __key;
	}

	Object* Pair::value()
	{
		return __value;
	}

	#pragma endregion

#pragma endregion

}