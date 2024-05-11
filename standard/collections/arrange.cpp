#include "arrange.hpp"

#pragma warning(disable : 4996)

namespace pycpp
{

#pragma region Definition : Range

	#pragma region Constructors & Destructors

	Range::Range(i32 end) : _start(0), _step(1), _end(end) {}

	Range::Range(i32 start, i32 end, i32 step) :
		_start(start), _step(step), _end(end)
	{
		if (step == 0)
		{
			std::cout << "Error in Range::Range(i32, i32, i32 step = 1) : "
				"step value is not allowed to be 0 !" << std::endl;

			throw std::exception("Error : rule of Range");
			return;
		}
	}

	#pragma endregion

	#pragma region Standard Methods

	i64 Range::type()
	{
		return Range::type_id;
	}

	std::string Range::class_name()
	{
		return "Range";
	}

	void Range::output()
	{
		std::string content;
		output(content);
		std::cout << content;
	}

	void Range::output(std::string& out)
	{
		char content[64];
		std::sprintf(content, "Range(%d, %d, %d)", _start, _end, _step);
		out = content;
	}

	u32 Range::bytes()
	{
		return sizeof(Range);
	}

	u64 Range::hash()
	{
		u64 seed = 0xabcdllu * u64(_step);
		seed ^= u64(_start) + (seed << 6) + (seed >> 2);
		seed ^= u64(_end) + (seed << 6) + (seed >> 2);
		return seed;
	}

	Object* Range::copy()
	{
		return new Range(*this);
	}


	Iterator* Range::first()
	{
		return new iterator(_start, _start, _end, _step);
	}

	Iterator* Range::last()
	{
		return new iterator(last_item(), _start, _end, _step);
	}

	#pragma endregion

	#pragma region Private Methods

	i32 Range::end_item()
	{
		i32 __end_item = _step > 0 ?
			_start + ((_end - _start - 1) / _step + 1) * _step :
			_start - ((_start - _end - 1) / _step - 1) * _step;
		return __end_item;
	}

	i32 Range::last_item()
	{
		i32 __last_item = _step > 0 ?
			_start + ((_end - _start - 1) / _step) * _step :
			_start - ((_start - _end - 1) / _step) * _step;
		return __last_item;
	}

	#pragma endregion

	#pragma region C++ Standard Methods

	Range::iterator Range::begin()
	{
		return Range::iterator(_start, _start, _end, _step);
	}

	Range::iterator Range::end()
	{
		i32 end_position = last_item();
		if (_end > _start)
			end_position += _step;
		else
			end_position -= _step;

		return Range::iterator(end_position, _start, _end, _step);
	}

	Range::iterator Range::rbegin()
	{
		return Range::iterator(last_item(), _start, _end, _step);
	}

	Range::iterator Range::rend()
	{
		i32 rend_position = last_item();
		if (_end > _start)
			rend_position -= _step;
		else
			rend_position += _step;

		return Range::iterator(rend_position, _start, _end, _step);
	}

	#pragma endregion

	#pragma region Iterator

	Range::iterator::iterator(i32 current, i32 start, i32 end, i32 step) :
		current(current), start(start), end(end), step(step), cur(nullptr) {}

	Range::iterator::~iterator()
	{
		if (cur != nullptr)
		{
			DEREF(cur);
			SAFE(cur);
		}
	}


	i64 Range::iterator::type()
	{
		return Range::iterator::type_id;
	}

	std::string Range::iterator::class_name()
	{
		return "Range::iterator";
	}

	void Range::iterator::output()
	{
		std::string content;
		output(content);
		std::cout << content;
	}

	void Range::iterator::output(std::string& out)
	{
		char content[64];
		std::sprintf(content, "Range::iterator(%d, <%d, %d, %d>)",
			current, start, end, step);
		out = content;
	}

	u64 Range::iterator::hash()
	{
		u64 seed = 0xabcdefu;
		seed ^= u64(start) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= u64(step) + 0x009e3779 + (seed << 6) + (seed >> 2);
		seed ^= u64(end) + 0x00009e37 + (seed << 6) + (seed >> 2);
		return seed;
	}

	u32 Range::iterator::bytes()
	{
		return sizeof(Range::iterator);
	}


	bool Range::iterator::is_valid()
	{
		bool __is_valid = step > 0 ? current < end : current > end;

		if (__is_valid && cur == nullptr)
		{
			cur = new I32(current);
			INREF(cur);
		}

		return __is_valid;
	}

	void Range::iterator::next()
	{
		if (cur == nullptr)
		{
			auto content_error = "Error in Range::iterator::next() : "
				"called next() on invalid Iterator OR "
				"you need to call is_valid() before calling next() !";

			std::cout << content_error << std::endl;
			throw std::exception("Error : invalid Iterator");
			return;
		}

		DEREF(cur);
		SAFE(cur);
		cur = nullptr;	// Khi lần gọi is_valid tới, 'cur' sẽ được gán.
		current += step;
	}

	void Range::iterator::prev()
	{
		if (cur == nullptr)
		{
			auto content_error = "Error in Range::iterator::prev() : "
				"called next() on invalid Iterator OR "
				"you need to call is_valid() before calling prev() !";

			std::cout << content_error << std::endl;
			throw std::exception("Error : invalid Iterator");
			return;
		}

		DEREF(cur);
		SAFE(cur);
		cur = nullptr;	// Khi lần gọi is_valid tới, 'cur' sẽ được gán.
		current -= step;
	}

	Object* Range::iterator::get()
	{
		if (is_valid() == false || cur == nullptr)
		{
			auto content_error = "Error in Range::iterator::get() : "
				"called next() on invalid Iterator OR "
				"you need to call is_valid() before calling get() !";

			std::cout << content_error << std::endl;
			throw std::exception("Error : invalid Iterator");
			return nullptr;
		}

		return cur;
	}


	i32 Range::iterator::operator*()
	{
		return current;
	}

	void Range::iterator::operator++()
	{
		current += step;
	}

	void Range::iterator::operator--()
	{
		current -= step;
	}

	bool Range::iterator::operator!=(const iterator& other)
	{
		return current != other.current;
	}

	#pragma endregion

#pragma endregion

}