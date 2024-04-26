#pragma once

#include "iostream"

namespace pycpp
{

	void __pycpp_error_dont_support_method(const char* block_location);
	void __pycpp_binding_error
	(const char* block_location, const char* binding_from, const char* binding_to);
	void __pycpp_error_out_of_bound_error(const char* block_location);
	void __pycpp_object_is_none(const char* block_location, const char* class_name);
	void __pycpp_error_retrieve_from_empty_container(const char* block_location, const char* class_name);

	/// <summary>
	/// Cảnh báo lên "console" nếu Cast không thành công.
	/// (!) : định nghĩa macro "DISABLED_WARNING_CONSOLE" để tắt cảnh báo này.
	/// </summary>
	/// <typeparam name="Source">Trước khi Cast</typeparam>
	/// <typeparam name="Desination">Sau khi Cast</typeparam>
	template<typename Source, typename Desination>
	inline void __pycpp_warning_lost_object_reference()
	{
#ifndef DISABLED_WARNING_CONSOLE
		char content_warning[128];
		sprintf_s(content_warning,
			"Warning : lost object reference when cast \"%s\" to \"%s\" !",
			typeid(Source).name(), typeid(Desination).name());

		std::cout << content_warning << std::endl;
#endif
	}
}