#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>

template <typename T>
T map(const T& val, const T& start, const T& end, const T& newStart, const T& newEnd) {
	T slope = (newEnd - newStart) / (end - start);
	return (newStart + slope * (val - start));
}

template <typename T>
T min(const T& a, const T& b) { return (a < b) ? a : b; }

template <typename T>
T max(const T& a, const T& b) { return (a > b) ? a : b; }

unsigned char charToHex(char c);

sf::Color hexToRgb(char* s);

class Fps {
private:
	unsigned int mFrame;
	unsigned int mFps;
	sf::Clock mClock;
public:
	Fps();
	void update();
	unsigned int getFps() const;
};
