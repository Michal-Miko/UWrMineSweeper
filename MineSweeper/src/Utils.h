#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>

template<typename T>
T map(const T & val, const T & start, const T & end, const T & newStart, const T & newEnd) {
	T slope = (T)(newEnd - newStart) / (end - start);
	return (T)(newStart + slope * (val - start));
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

class FPS {
private:
	unsigned int mFrame;
	unsigned int mFps;
	sf::Clock mClock;
public:
	FPS();
	void update();
	const unsigned int getFPS() const;
};
