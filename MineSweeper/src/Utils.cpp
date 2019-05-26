#include "Utils.h"

unsigned char mm::charToHex(char c) {
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else if (c >= '0' && c <= '9')
		return c - '0';
	return 0;
}

sf::Color mm::hexToRGB(char * s) {
	sf::Uint8 r = charToHex(s[0]) * 16 + charToHex(s[1]);
	sf::Uint8 g = charToHex(s[2]) * 16 + charToHex(s[3]);
	sf::Uint8 b = charToHex(s[4]) * 16 + charToHex(s[5]);
	return sf::Color(r, g, b);
}