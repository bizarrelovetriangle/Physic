#include <cstddef>

#ifndef ARRAY_EXTENTIONS
#define ARRAY_EXTENTIONS

class array_extentions
{
public:
	template<class T, std::size_t N>
	constexpr static std::size_t array_size(T(&)[N])
	{
		return N;
	}
}; 

#endif

