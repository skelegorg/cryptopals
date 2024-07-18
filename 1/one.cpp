#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <bitset>
#include <map>

// type 'byte' comes from cstddef and is equal to unsigned char
#include <cstddef>

std::ostream& operator<<(std::ostream& os, std::byte b) {
	return os << std::bitset<8>(std::to_integer<int>(b));
}

void print_bitset(std::vector<std::byte> bitset) {
	for(std::byte b : bitset) {
		std::cout << b;
	}
	std::cout << std::endl;
}



std::vector<std::byte> hex_to_bitset(std::string hex) {
	std::map<char, int> hexadecimal {{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'a', 10},
		{'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}
	};

	std::vector<std::byte> bitset;

	int index = 0;
	uint8_t newbyte = 0; 

	if((hex.size() % 2) != 0) {
		hex.insert(0, "0");
	}

	for(char c : hex) {
		if(index % 2 == 1) {
			newbyte |= hexadecimal[c];
			bitset.push_back(std::byte{newbyte});
			newbyte = 0;
		} else {
			newbyte |= hexadecimal[c];
			newbyte = newbyte << 4;
		}
		index++;
	}
	return bitset;
}

std::string base64encode(std::vector<std::byte> bitset) {
	std::string encoded_string;

	// translate bitset into a vec<bool> for ease
	int cmp = 0;
	std::vector<bool> bits;
	for(std::byte b : bitset) {
		std::byte checker{0b10000000};
		while(std::to_integer<int>(checker) > 0) {
			bits.push_back(std::to_integer<int>(b & checker) > 0);
			checker >>= 1;
		}
	}

	int padding = bits.size() % 6 == 0 ? 0 : (6 - bits.size() % 6);
	for(int i = 0; i < padding; i++) {
		bits.push_back(false);
	}

	std::vector<uint8_t> unencoded_chars;

	uint8_t newbyte = 0;
	int bitcounter = 0;
	while(bitcounter < bits.size()) {
		newbyte |= bits[bitcounter];
		bitcounter++;
		if(bitcounter % 6 == 0) {
			unencoded_chars.push_back(newbyte);
			newbyte = 0;
		}
		newbyte <<= 1;
	}

	for(uint8_t b : unencoded_chars) {
		if(b >= 0 && b <= 25) {
			// capital letter
			encoded_string.push_back(b + 65);
		} else if(b >= 26 && b <= 51) {
			encoded_string.push_back(b + 71);
		} else if(b >= 52 && b <= 61) {
			encoded_string.push_back(b - 4);
		} else if(b == 62) {
			encoded_string.push_back('+');
		} else if(b == 63) {
			encoded_string.push_back('/');
		} else {
			std::cout << "(-): base64encode: illegal character" << std::endl;
		}
	}
	
	for(int i = 0; i < 2; i++) {
		if(encoded_string.size() % 4 != 0) {
			encoded_string.push_back('=');
		} else {
			break;
		}
	}

	return encoded_string;
}

int main() {
	auto bitset = hex_to_bitset("49276d206b696c6c69e6726f72206c696b65206120706f69736f6e6f7573206d7368726f6f");
	std::cout << base64encode(bitset) << std::endl;
	return 0;
}
