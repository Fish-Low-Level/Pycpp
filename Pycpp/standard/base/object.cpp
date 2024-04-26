#include "object.hpp"
#include "error.hpp"
#include "var.hpp"
#include "primitive.hpp"
#include "pair.hpp"

namespace pycpp
{

#pragma region Object

	Object::Object() : ref_count(0u) {}

	i64 Object::type()
	{
		return -1;
	}

	std::string Object::class_name()
	{
		return "Unknown";
	}

	void Object::output()
	{
		char content[64];
		sprintf_s(content, "<%s at %p>", class_name(), this);
		std::cout << content;
	}

	void Object::output(std::string& out)
	{
		char content[64];
		sprintf_s(content, "<%s at %p>", class_name().c_str(), this);
		out = content;
	}

	void Object::info()
	{
		output();
	}

	void Object::info(std::string& out)
	{
		output(out);
	}

	u32 Object::bytes()
	{
		return sizeof(Object);
	}

	u64 Object::hash()
	{
		return reinterpret_cast<u64>(this);
	}

	Object* Object::copy()
	{
		char block_location[64];
		sprintf_s(block_location, "%s::copy", class_name());
		__pycpp_error_dont_support_method(block_location);
		return nullptr;
	}


	bool Object::equal(Object* other)
	{
		if (this == other)
			return true;

		bool result = false;
		if (other->type() == Pair::type_id)
			result = equal(static_cast<Pair*>(other)->key());

		SAFE(other);
		return result;
	}

	i32 Object::rich_compare(Object*)
	{
		char block_location[32];
		sprintf_s(block_location, "%s::lt", class_name().c_str());

		__pycpp_error_dont_support_method(block_location);
		return 0;
	}

	Object* Object::add(Object*)
	{
		char block_location[64];
		sprintf_s(block_location, "%s::add", class_name().c_str());

		__pycpp_error_dont_support_method(block_location);
		return nullptr;
	}

	Object* Object::sub(Object*)
	{
		char block_location[64];
		sprintf_s(block_location, "%s::sub", class_name().c_str());

		__pycpp_error_dont_support_method(block_location);
		return nullptr;
	}

	Object* Object::mul(Object*)
	{
		char block_location[64];
		sprintf_s(block_location, "%s::mul", class_name().c_str());

		__pycpp_error_dont_support_method(block_location);
		return nullptr;
	}

	Object* Object::div(Object*)
	{
		char block_location[64];
		sprintf_s(block_location, "%s::div", class_name().c_str());

		__pycpp_error_dont_support_method(block_location);
		return nullptr;
	}

#pragma endregion

#pragma region Boolean

	Boolean Boolean::True;
	Boolean Boolean::False;


	Boolean::Boolean()
	{
		ref_count = 1u;
	}

	void Boolean::output()
	{
		if (this == &Boolean::True)
			std::cout << "True";
		else
			std::cout << "False";
	}

	void Boolean::output(std::string& out)
	{
		if (this == &Boolean::True)
			out = "True";
		else
			out = "False";
	}

	std::string Boolean::class_name()
	{
		return "Boolean";
	}

	i64 Boolean::type()
	{
		return Boolean::type_id;
	}

	u32 Boolean::bytes()
	{
		return sizeof(Boolean);
	}

	u64 Boolean::hash()
	{
		if(this == &Boolean::True)
			return 1ul;

		return 0ul;
	}


	bool Boolean::equal(Object* other)
	{
		bool result = false;
		i32 value_of_this = (IS(this, TRUE) ? 1 : 0);

		switch (other->type())
		{
		case Boolean::type_id:
			result = this == other;
			break;

		case I32::type_id:
			result = value_of_this == static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value_of_this == static_cast<const I64*>(other)->value;
			break;
		
		case F32::type_id:
			result = value_of_this == static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value_of_this == static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = equal(static_cast<Pair*>(other)->key());
			break;
		}

		SAFE(other);
		return result;
	}

	i32 Boolean::rich_compare(Object* other)
	{
		i32 result;
		i32 value_of_this = (IS(this, TRUE) ? 1 : 0);

		switch (other->type())
		{
		case Boolean::type_id:
			result = value_of_this - (IS(other, TRUE) ? 1 : 0);
			break;

		case I32::type_id:
			result = value_of_this - static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value_of_this - static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value_of_this - static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value_of_this - static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = rich_compare(static_cast<Pair*>(other)->key());
			break;

		default:
		{
			auto __class_name = class_name();
			char content_error[128];
			sprintf_s(content_error,
				"Error in i32 %::rich_compare(Object) : "
				"cannot compare %s with %s !",
				__class_name.c_str(),
				other->class_name().c_str(),
				__class_name.c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : compare with invalid data type");
		}
		}

		SAFE(other);
		return result;
	}

	Object* Boolean::add(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();
		i32 value_of_this = (IS(this, TRUE) ? 1 : 0);

		if (other_type == Boolean::type_id)
			result = new I32(
				value_of_this + (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I32(
				value_of_this + static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value_of_this + static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value_of_this + static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value_of_this + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in Boolean::add : the add operator cannot be used"
				"to add Boolean with %s !",
				other->class_name());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* Boolean::sub(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();
		i32 value_of_this = (IS(this, TRUE) ? 1 : 0);

		if (other_type == Boolean::type_id)
			result = new I32(
				value_of_this - (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I32(
				value_of_this - static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value_of_this - static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value_of_this - static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value_of_this - static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in Boolean::sub : the sub operator cannot be used"
				"to subtract Boolean from %s !",
				other->class_name());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* Boolean::mul(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();
		i32 value_of_this = (IS(this, TRUE) ? 1 : 0);

		if (other_type == Boolean::type_id)
			result = new I32(
				value_of_this * (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I32(
				value_of_this * static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value_of_this * static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value_of_this * static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value_of_this * static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in Boolean::mul : the mul operator cannot be used"
				"to multiply Boolean with %s !",
				other->class_name());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* Boolean::div(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();
		i32 value_of_this = (IS(this, TRUE) ? 1 : 0);

		if (other_type == Boolean::type_id)
			result = new I32(
				value_of_this / (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new F32(
				f32(value_of_this) / static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F32(
				f32(value_of_this) / static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value_of_this / static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value_of_this / static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in Boolean::div : the div operator cannot be used"
				"to divide Boolean with %s !",
				other->class_name());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

#pragma endregion

#pragma region None

	None None::instance;

	None::None()
	{
		ref_count = 1u;
	}

	void None::output()
	{
		std::cout << "None";
	}

	void None::output(std::string& out)
	{
		out = "None";
	}

	std::string None::class_name()
	{
		return "None";
	}

	i64 None::type()
	{
		return None::type_id;
	}

	u32 None::bytes()
	{
		return sizeof(None);
	}

	u64 None::hash()
	{
		constexpr const u64 _hash = 0xabcd9876 ^ 0x6789dcba;
		return _hash;
	}


	bool None::equal(Object* other)
	{
		bool result = false;

		switch (other->type())
		{
		case None::type_id:
			result = true;
			break;

		case Pair::type_id:
			result = equal(static_cast<Pair*>(other)->key());
			break;
		}

		SAFE(other);
		return result;
	}

	i32 None::rich_compare(Object* other)
	{
		if (this == other)
			return;

		i32 result;

		switch (other->type())
		{
		case None::type_id:
			result = true;
			break;

		case Pair::type_id:
			result = equal(static_cast<Pair*>(other)->key());
			break;

		default:
			result = -1;
		}

		SAFE(other);
		return result;
	}

#pragma endregion

}