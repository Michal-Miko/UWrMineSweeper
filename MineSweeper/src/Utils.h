#pragma once

#include <SFML/Graphics/Color.hpp>

namespace mm {
	template<typename T>
	T map(const T & val, const T & start, const T & end, const T & newStart, const T & newEnd) {
		T slope = (T)(newEnd - newStart) / (end - start);
		return (T)(newStart + round(slope * (val - start)));
	}

	template<typename T>
	T min(const T & a, const T & b) {
		return (a < b) ? a : b;
	}

	template<typename T>
	T max(const T & a, const T & b) {
		return (a > b) ? a : b;
	}

	unsigned char charToHex(char c);

	sf::Color hexToRGB(char * s);
}
