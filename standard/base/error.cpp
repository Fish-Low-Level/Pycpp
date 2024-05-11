#include "error.hpp"

#include <iostream>

namespace pycpp
{

#pragma region Error

	void __pycpp_error_dont_support_method(const char* block_location)
	{
		char content_error[128];
		sprintf_s(content_error,
			"Error in %s : doesn't support this method !\n",
			block_location);


		std::cout << content_error << std::endl;
		throw std::exception("Error : class doesn't support method !");
	}

	void __pycpp_binding_error
	(const char* block_location, const char* binding_from, const char* binding_to)
	{
		char content_error[128];
		sprintf_s(content_error,
			"Error in %s : unable to binding from %s to %s !",
			block_location, binding_from, binding_to);

		std::cout << content_error << std::endl;
		throw std::exception("Error : binding error !");
	}

	void __pycpp_error_out_of_bound(const char* block_location)
	{
		char content_error[128];
		sprintf_s(content_error,
			"Error in %s : index out of bound !", block_location);

		std::cout << content_error << std::endl;
		throw std::exception("Error : index out of bound !");
	}

	void __pycpp_object_is_none(const char* block_location, const char* class_name)
	{
		char content_error[128];
		sprintf_s(content_error,
			"Error in %s : called %s type on None object !", block_location, class_name);

		std::cout << content_error << std::endl;
		throw std::exception("Error : object is None !");
	}

	void __pycpp_error_retrieve_from_empty_container(const char* block_location, const char* class_name)
	{
		char content_error[128];
		sprintf_s(content_error,
			"Error in %s : retrieve from an empty %s !", block_location, class_name);

		std::cout << content_error << std::endl;
		throw std::exception("Error : retrieve from an empty container");
	}

#pragma endregion

}