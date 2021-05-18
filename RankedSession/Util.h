#pragma once
#include <vector>
#include <map>

namespace RankedSession
{
	template <typename T>
	bool VectorHasItem(std::vector<T>* vector, T item);
}

template <typename T>
inline bool RankedSession::VectorHasItem(std::vector<T>* vector, const T item)
{
	if (std::find(vector->begin(), vector->end(), item) == vector->end())
	{
		return false;
	}
	return true;
}