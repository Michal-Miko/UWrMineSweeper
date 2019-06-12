#include "Utils.h"

unsigned char charToHex(const char c) {
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if (c >= '0' && c <= '9')
		return c - '0';
	return 0;
}

sf::Color hexToRgb(char* s) {
	const sf::Uint8 r = charToHex(s[0]) * 16 + charToHex(s[1]);
	const sf::Uint8 g = charToHex(s[2]) * 16 + charToHex(s[3]);
	const sf::Uint8 b = charToHex(s[4]) * 16 + charToHex(s[5]);
	return { r, g, b };
}

Fps::Fps() : mFrame(0), mFps(0) {}

void Fps::update() {
	if (mClock.getElapsedTime().asSeconds() >= 1.f) {
		mFps = mFrame;
		mFrame = 0;
		mClock.restart();
	}

	++mFrame;
}

unsigned int Fps::getFps() const { return mFps; }
