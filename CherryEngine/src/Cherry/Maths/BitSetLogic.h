#pragma once
#include <bitset>

template <size_t T>
bool same_bit_comparison(const std::bitset<T>& left, const std::bitset<T>& right)
{
	for (size_t i = 0; i < T; i++)
	{
		if (left[i] & right[i])
			return true;
	}
	return false;
}