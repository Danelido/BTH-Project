#ifndef _RANDOM_NUMBER_GENERATOR_H
#define _RANDOM_NUMBER_GENERATOR_H
#include <cstdlib>
#include <GLM/glm.hpp>
namespace RandomNum {

	template<typename T>
	inline T single(const T& low, const T& high)
	{
		if (low == high)
		{
			return high;
		}

		T magicNumber = (rand()) % (int)(high - low + 1) + low;
		return magicNumber;
	}

	// Randomize ALL three components between the specified value
	template<typename T>
	inline glm::vec3 vec3(const T& low, const T& high)
	{
		glm::vec3 magicVector = glm::vec3(single(low, high), single(low, high), single(low, high));

		return magicVector;
	}

	template<typename T>
	inline glm::vec3 vec3(const T& lx, const T& hx, const T& ly, const T& hy, const T& lz, const T& hz)
	{
		glm::vec3 magicVector = glm::vec3(single(lx, hx), single(ly, hy), single(lz, hz));

		return magicVector;
	}

}

#endif
