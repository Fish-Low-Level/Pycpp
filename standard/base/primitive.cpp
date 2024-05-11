#include "primitive.hpp"
#include "pair.hpp"
#include "var.hpp"

#pragma warning(disable: 4244)

namespace pycpp
{

#pragma region I32

#pragma region Constructor & Destructor

	I32::I32() : value(0) {}

	I32::I32(i32 value) : value(value) {}

	I32::I32(const I32& source) : value(source.value) {}

	I32::I32(Object* source) : value(init_from_object(source)) {}

	i32 I32::init_from_object(Object* source)
	{
		i32 result;

		auto source_type = source->type();

		if(source_type == Boolean::type_id)
			result = IS(source, TRUE) ? 1 : 0;
		else if(source_type == I32::type_id)
			result = static_cast<const I32*>(source)->value;
		else if (source_type == I64::type_id)
			result = static_cast<const I64*>(source)->value;
		else if (source_type == F32::type_id)
			result = static_cast<const F32*>(source)->value;
		else if (source_type == F64::type_id)
			result = static_cast<const F64*>(source)->value;
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I32::I32(Object* other) : "
				" data type %s of 'source' is invalid !",
				source->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : data type invalid");
		}

		SAFE(source);
		return result;
	}

#pragma endregion

#pragma region Standard Methods

	std::string I32::class_name()
	{
		return "I32";
	}

	i64 I32::type()
	{
		return I32::type_id;
	}

	void I32::output()
	{
		std::cout << value;
	}

	void I32::output(std::string& out)
	{
		out = std::to_string(value);
	}

	u64 I32::hash()
	{
		return value;
	}

	Object* I32::copy()
	{
		return new I32(value);
	}

	u32 I32::bytes()
	{
		return sizeof(I32);
	}


	bool I32::equal(Object* other)
	{
		bool result = false;

		switch (other->type())
		{
		case Boolean::type_id:
			result = value == (IS(other, TRUE) ? 1 : 0);
			break;

		case I32::type_id:
			result = value == static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value == static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value == static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value == static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = equal(static_cast<Pair*>(other)->key());
			break;
		}

		SAFE(other);
		return result;
	}

	i32 I32::rich_compare(Object* other)
	{
		i32 result;

		switch (other->type())
		{
		case Boolean::type_id:
			result = value < (IS(other, TRUE) ? 1 : 0);
			break;

		case I32::type_id:
			result = value < static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value < static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value < static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value < static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = rich_compare(static_cast<Pair*>(other)->key());
			break;

		default:
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I32::rich_compare : "
				"the compare operator cannot be used"
				"to compare I32 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : compare with invalid data type");
		}
		}

		SAFE(other);
		return result;
	}


	Object* I32::add(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new I32(
				value + (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I32(
				value + static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value + static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value + static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I32::add : the add operator cannot be used"
				"to add I32 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* I32::sub(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new I32(
				value - (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I32(
				value - static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value - static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value - static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I32::sub : the sub operator cannot be used"
				"to substract I32 from %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* I32::mul(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new I32(
				value * (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I32(
				value * static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value * static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value * static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value * static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I32::mul : the mul operator cannot be used"
				"to multiply I32 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* I32::div(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new I32(
				value / (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new F32(
				f32(value) / static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F32(
				f32(value) / static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				f32(value) / static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value / static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I32::div : the div operator cannot be used"
				"to divide I32 by %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

#pragma endregion

#pragma endregion

#pragma region I64

#pragma region Constructor & Destructor

	I64::I64() : value(0) {}

	I64::I64(i64 value) : value(value) {}

	I64::I64(const I64& source) : value(source.value) {}

	I64::I64(Object* source) : value(init_from_object(source)) {}

	i64 I64::init_from_object(Object* source)
	{
		i64 result;
		i64 source_type = source->type();

		if(source_type == Boolean::type_id)
			result = IS(source, TRUE) ? 1 : 0;
		else if (source_type == I32::type_id)
			result = static_cast<const I32*>(source)->value;
		else if (source_type == I64::type_id)
			result = static_cast<const I64*>(source)->value;
		else if (source_type == F32::type_id)
			result = static_cast<const F32*>(source)->value;
		else if (source_type == F64::type_id)
			result = static_cast<const F64*>(source)->value;
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I64::I64(Object* other) : "
				" data type %s of 'source' is invalid !",
				source->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : data type invalid");
		}

		SAFE(source);
		return result;
	}

#pragma endregion

#pragma region Standard Methods

	std::string I64::class_name()
	{
		return "I64";
	}

	i64 I64::type()
	{
		return I64::type_id;
	}

	void I64::output()
	{
		std::cout << value;
	}

	void I64::output(std::string& out)
	{
		out = std::to_string(value);
	}

	u64 I64::hash()
	{
		return value;
	}

	Object* I64::copy() {
		return new I64(value);
	}

	u32 I64::bytes()
	{
		return sizeof(I64);
	}


	bool I64::equal(Object* other)
	{
		bool result = false;

		switch (other->type())
		{
		case Boolean::type_id:
			result = value == (IS(other, TRUE) ? 1 : 0);
			break;

		case I32::type_id:
			result = value == static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value == static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value == static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value == static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = equal(static_cast<Pair*>(other)->key());
			break;
		}

		SAFE(other);
		return result;
	}

	i32 I64::rich_compare(Object* other)
	{
		i32 result;

		switch (other->type())
		{
		case Boolean::type_id:
			result = value < (IS(other, TRUE) ? 1 : 0);
			break;

		case I32::type_id:
			result = value < static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value < static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value < static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value < static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = rich_compare(static_cast<Pair*>(other)->key());
			break;

		default:
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I64::rich_compare : "
				"the compare operator cannot be used"
				"to compare I64 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : compare with invalid data type");
		}
		}

		SAFE(other);
		return result;
	}


	Object* I64::add(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new I64(
				value + (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I64(
				value + static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value + static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value + static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I64::add : the add operator cannot be used"
				"to add I64 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* I64::sub(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new I64(
				value - (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I64(
				value - static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value - static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value - static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I64::sub : the sub operator cannot be used"
				"to substract I64 from %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* I64::mul(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new I64(
				value * (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new I64(
				value * static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new I64(
				value * static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value * static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value * static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I64::mul : the mul operator cannot be used"
				"to multiply I64 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* I64::div(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new I64(
				value / (IS(other, TRUE) ? 1 : 0)
			);
		else if (other_type == I32::type_id)
			result = new F32(
				f32(value) / static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F32(
				f32(value) / static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value / static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value / static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in I64::div : the div operator cannot be used"
				"to divide I64 by %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

#pragma endregion

#pragma endregion

#pragma region F32

#pragma region Constructor & Destructor

	F32::F32() : value(0.f) {}

	F32::F32(f32 value) : value(value) {}

	F32::F32(const F32& source) : value(source.value) {}

	F32::F32(Object* source) : value(init_from_object(source)) {}

	f32 F32::init_from_object(Object* source)
	{
		f32 result;
		i64 source_type = source->type();

		if(source_type == Boolean::type_id)
			result = IS(source, TRUE) ? 1.f : 0.f;
		else if (source_type == I32::type_id)
			result = static_cast<const I32*>(source)->value;
		else if (source_type == I64::type_id)
			result = static_cast<const I64*>(source)->value;
		else if (source_type == F32::type_id)
			result = static_cast<const F32*>(source)->value;
		else if (source_type == F64::type_id)
			result = static_cast<const F64*>(source)->value;
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F32::F32(Object* other) : "
				" data type %s of 'source' is invalid !",
				source->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : data type invalid");
		}

		SAFE(source);
		return result;
	}

#pragma endregion

#pragma region Standard Methods

	std::string F32::class_name()
	{
		return "F32";
	}

	i64 F32::type()
	{
		return F32::type_id;
	}

	void F32::output()
	{
		std::cout << value;
	}

	void F32::output(std::string& out)
	{
		out = std::to_string(value);
	}

	u64 F32::hash()
	{
		return i64(*reinterpret_cast<const i32*>(&value));
	}

	Object* F32::copy() {
		return new F32(value);
	}

	u32 F32::bytes()
	{
		return sizeof(F32);
	}


	bool F32::equal(Object* other)
	{
		bool result = false;

		switch (other->type())
		{
		case Boolean::type_id:
			result = value == (IS(other, TRUE) ? 1.f : 0.f);
			break;

		case I32::type_id:
			result = value == static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value == static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value == static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value == static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = equal(static_cast<Pair*>(other)->key());
			break;
		}

		SAFE(other);
		return result;
	}

	i32 F32::rich_compare(Object* other)
	{
		i32 result;

		switch (other->type())
		{
		case Boolean::type_id:
			result = value < (IS(other, TRUE) ? 1.f : 0.f);
			break;

		case I32::type_id:
			result = value < static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value < static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value < static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value < static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = rich_compare(static_cast<Pair*>(other)->key());
			break;

		default:
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F32::rich_compare : "
				"the compare than operator cannot be used"
				"to compare F32 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : compare with invalid data type");
		}
		}

		SAFE(other);
		return result;
	}


	Object* F32::add(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new F32(
				value + (IS(other, TRUE) ? 1.f : 0.f)
			);
		else if (other_type == I32::type_id)
			result = new F32(
				value + static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F32(
				value + static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value + static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F32::add : the add operator cannot be used"
				"to add F32 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* F32::sub(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new F32(
				value - (IS(other, TRUE) ? 1.f : 0.f)
			);
		else if (other_type == I32::type_id)
			result = new F32(
				value - static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F32(
				value - static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value - static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F32::sub : the sub operator cannot be used"
				"to substract F32 from %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* F32::mul(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new F32(
				value * (IS(other, TRUE) ? 1.f : 0.f)
			);
		else if (other_type == I32::type_id)
			result = new F32(
				value * static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F32(
				value * static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value * static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value * static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F32::mul : the mul operator cannot be used"
				"to multiply F32 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* F32::div(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new F32(
				value / (IS(other, TRUE) ? 1.f : 0.f)
			);
		else if (other_type == I32::type_id)
			result = new F32(
				value / static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F32(
				value / static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F32(
				value / static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value / static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F32::div : the div operator cannot be used"
				"to divide F32 by %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

#pragma endregion

#pragma endregion

#pragma region F64

#pragma region Constructor & Destructor

	F64::F64() : value(0.) {}

	F64::F64(f64 value) : value(value) {}

	F64::F64(const F64& source) : value(source.value) {}

	F64::F64(Object* source) : value(init_from_object(source)) {}

	f64 F64::init_from_object(Object* source)
	{
		f64 result;
		i64 other_source = source->type();

		if(other_source == Boolean::type_id)
			result = IS(source, TRUE) ? 1. : 0.;
		else if (other_source == I32::type_id)
			result = static_cast<const I32*>(source)->value;
		else if (other_source == I64::type_id)
			result = static_cast<const I64*>(source)->value;
		else if (other_source == F32::type_id)
			result = static_cast<const F32*>(source)->value;
		else if (other_source == F64::type_id)
			result = static_cast<const F64*>(source)->value;
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F64::F64(Object* other) : "
				" data type %s of 'source' is invalid !",
				source->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : data type invalid");
		}

		SAFE(source);
		return result;
	}

#pragma endregion

#pragma region Standard Methods

	std::string F64::class_name()
	{
		return "F32";
	}

	i64 F64::type()
	{
		return F64::type_id;
	}

	void F64::output()
	{
		std::cout << value;
	}

	void F64::output(std::string& out)
	{
		out = std::to_string(value);
	}

	u64 F64::hash()
	{
		return *reinterpret_cast<const i64*>(&value);
	}

	Object* F64::copy() {
		return new F64(value);
	}

	u32 F64::bytes()
	{
		return sizeof(F64);
	}


	bool F64::equal(Object* other)
	{
		bool result = false;

		switch (other->type())
		{
		case Boolean::type_id:
			result = value == (IS(other, TRUE) ? 1. : 0.);
			break;

		case I32::type_id:
			result = value == static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value == static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value == static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value == static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = equal(static_cast<Pair*>(other)->key());
			break;
		}

		SAFE(other);
		return result;
	}

	i32 F64::rich_compare(Object* other)
	{
		i32 result;

		switch (other->type())
		{
		case Boolean::type_id:
			result = value < (IS(other, TRUE) ? 1. : 0.);
			break;

		case I32::type_id:
			result = value < static_cast<const I32*>(other)->value;
			break;

		case I64::type_id:
			result = value < static_cast<const I64*>(other)->value;
			break;

		case F32::type_id:
			result = value < static_cast<const F32*>(other)->value;
			break;

		case F64::type_id:
			result = value < static_cast<const F64*>(other)->value;
			break;

		case Pair::type_id:
			result = rich_compare(static_cast<Pair*>(other)->key());
			break;

		default:
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F64::rich_compare : "
				"the compare operator cannot be used"
				"to compare F64 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : compare with invalid data type");
		}
		}

		SAFE(other);
		return result;
	}


	Object* F64::add(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new F64(
				value + (IS(other, TRUE) ? 1. : 0.)
			);
		else if (other_type == I32::type_id)
			result = new F64(
				value + static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F64(
				value + static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F64(
				value + static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F64::add : the add operator cannot be used"
				"to add F64 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* F64::sub(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new F64(
				value - (IS(other, TRUE) ? 1. : 0.)
			);
		else if (other_type == I32::type_id)
			result = new F64(
				value - static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F64(
				value - static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F64(
				value - static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value + static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F64::sub : the sub operator cannot be used"
				"to substract F64 from %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* F64::mul(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new F64(
				value * (IS(other, TRUE) ? 1. : 0.)
			);
		else if (other_type == I32::type_id)
			result = new F64(
				value * static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F64(
				value * static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F64(
				value * static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value * static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F64::mul : the mul operator cannot be used"
				"to multiply F64 with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

	Object* F64::div(Object* other) {
		Object* result = nullptr;
		i64 other_type = other->type();

		if(other_type == Boolean::type_id)
			result = new F64(
				value / (IS(other, TRUE) ? 1. : 0.)
			);
		else if (other_type == I32::type_id)
			result = new F64(
				value / static_cast<const I32*>(other)->value
			);
		else if (other_type == I64::type_id)
			result = new F64(
				value / static_cast<const I64*>(other)->value
			);
		else if (other_type == F32::type_id)
			result = new F64(
				value / static_cast<const F32*>(other)->value
			);
		else if (other_type == F64::type_id)
			result = new F64(
				value / static_cast<const F64*>(other)->value
			);
		else
		{
			char content_error[128];
			sprintf_s(content_error,
				"Error in F64::div : the div operator cannot be used"
				"to divide F64 by %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : operator with invalid data type");
		}

		SAFE(other);
		return result;
	}

#pragma endregion

#pragma endregion

}