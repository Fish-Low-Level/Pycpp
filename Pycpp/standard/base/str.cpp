#include "str.hpp"
#include "pair.hpp"
#include "var.hpp"

namespace pycpp
{

#pragma region Definition : Str

	#pragma region Constructors & Destructors

	Str::Str() : string() {}

	Str::Str(Str& source) : string(source.string) {}

	Str::Str(Object* source)
	{
		source->output(string);
		SAFE(source);
	}

	Str::Str(const char* content) : string(content) {}

	#pragma endregion

	#pragma region Standard Methods



	#pragma endregion

	#pragma region Override Object

	i64 Str::type()
	{
		return Str::type_id;
	}

	std::string Str::class_name()
	{
		return "Str";
	}

	void Str::output()
	{
		std::cout << string;
	}

	void Str::output(std::string& out)
	{
		out = string;
	}

	void Str::info()
	{
		std::string content = "\"" + string + "\"";
		std::cout << content;
	}

	void Str::info(std::string& output)
	{
		output = "\"" + string + "\"";
	}

	u64 Str::hash()
	{
		// TODO
	}

	u32 Str::bytes()
	{
		return sizeof(Str);
	}

	Object* Str::copy()
	{
		return new Str(*this);
	}

	// So sánh Str với Str.
	bool Str::equal(Object* other)
	{
		if (this == other)
			return true;
		bool result = false;

		switch (other->type())
		{
		case Str::type_id:
			result = dynamic_cast<Str*>(other)->string.compare(string) == 0;
			break;

		case Pair::type_id:
			result = equal(dynamic_cast<Pair*>(other)->key());
			break;
		}

		SAFE(other);
		return result;
	}

	i32 Str::rich_compare(Object* other)
	{
		if (this == other)
			return 0;
		i32 result;

		switch (other->type())
		{
		case Str::type_id:
			result = string.compare(dynamic_cast<Str*>(other)->string);
			break;

		case Pair::type_id:
			result = rich_compare(dynamic_cast<Pair*>(other)->key());

		default:
		{
			char content_error[128];
			std::sprintf(content_error,
				"Error in Str::rich_compare : "
				"the compare operator cannot be used"
				"to compare Str with %s !",
				other->class_name().c_str());

			std::cout << content_error << std::endl;
			throw std::exception("Error : compare with invalid data type");
		}
		}

		SAFE(other);
		return result;
	}

	#pragma endregion

	#pragma region Override Iterable



	#pragma endregion

#pragma endregion

}