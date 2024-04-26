#include "funtor.hpp"
#include "../collections/tuple.hpp"

namespace pycpp
{

#pragma region Lambda

	Lambda::Lambda(lambda_t lambda) : lambda(lambda) {}

	Object* Lambda::call(std::initializer_list<Object*> list)
	{
		Tuple* tup = new Tuple(list);

		INREF(tup);
		auto result = lambda(tup);

		DEREF(tup);
		SAFE(tup);
		return result;
	}

#pragma endregion

#pragma region Predicate

	Predicate::Predicate(lambda_t lambda) : lambda(lambda) {}

	bool Predicate::call(Object* param)
	{
		INREF(param);
		auto result = lambda(param);

		DEREF(param);
		SAFE(param);
		return result;
	}

	Object* Predicate::call(std::initializer_list<Object*> list)
	{
		auto it = list.begin();
		u32 size = (u32)list.size();
		Object* param = it[0];
		INREF(param);

		if (size > 1u)
		{
			for (u32 i = 1u; i < size; ++i)
				SAFE(it[i]);
		}

		bool result = lambda(param);
		DEREF(param);
		SAFE(param);

		return (result) ? TRUE : FALSE;
	}

#pragma endregion

#pragma region Mapping

	Mapping::Mapping(lambda_t lambda) : lambda(lambda) {}

	Object* Mapping::call(Object* param)
	{
		INREF(param);
		auto result = lambda(param);

		DEREF(param);
		SAFE(param);
		return result;
	}

	Object* Mapping::call(std::initializer_list<Object*> list)
	{
		auto it = list.begin();
		u32 size = (u32)list.size();
		Object* param = it[0];
		INREF(param);

		if (size > 1u)
		{
			for (u32 i = 1u; i < size; ++i)
				SAFE(it[i]);
		}

		Object* result = lambda(param);
		DEREF(param);
		SAFE(param);

		return result;
	}

#pragma endregion

#pragma region Comparer

	Comparer::Comparer(lambda_t lambda) : lambda(lambda) {}

	i32 Comparer::call(Object* param_0, Object* param_1)
	{
		INREF(param_0);
		INREF(param_1);
		i32 result = lambda(param_0, param_1);

		DEREF(param_0);
		SAFE(param_0);
		SAFE(param_0);
		SAFE(param_1);
		return result;
	}

	Object* Comparer::call(std::initializer_list<Object*> list)
	{
		auto it = list.begin();
		u32 size = (u32)list.size();

		if (size <= 1)
		{
			char content_error[256];
				
			sprintf_s(content_error,
				"Error in Comparer::call(std::initializer_list<>) : "
				"requires 2 parameters, while it only receives %d parameter !",
				size);
			std::cout << content_error << std::endl;

			throw std::exception("Error : Comparer requires 2 parameters to call");
			return nullptr;
		}

		Object* param_0 = it[0];
		Object* param_1 = it[1];
		INREF(param_0);
		INREF(param_1);

		if (size > 2u)
		{
			for (u32 i = 2u; i < size; ++i)
				SAFE(it[i]);
		}

		i32 result = lambda(param_0, param_1);

		DEREF(param_0);
		SAFE(param_0);
		DEREF(param_1);
		SAFE(param_1);
		return new I32(result);
	}

#pragma endregion

}