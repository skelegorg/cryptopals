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

typedef std::vector<std::byte> bytemap;

// 1:1
std::ostream& operator<<(std::ostream& os, std::byte b) {
	return os << std::bitset<8>(std::to_integer<int>(b));
}

void print_bitset(std::vector<std::byte> bitset) {
	for(std::byte b : bitset) {
		std::cout << b << " ";
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

// 1:2
bytemap xor_equal_length_bitsets(bytemap b1, bytemap b2) {
	std::vector<std::byte> xor_buffer;
	for(int i = 0; i < b1.size(); i++) {
		xor_buffer.push_back(b1[i] ^ b2[i]);
	}
	return xor_buffer;
}

std::string bitset_to_hex(bytemap bytes) {
	std::map<int, char> hexadecimal {{0, '0'}, {1, '1'},
		{2, '2'}, {3, '3'}, {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'}, {8, '8'}, {9, '9'}, {10, 'a'},
		{11, 'b'}, {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'}
	};
	std::string hexstring;
	
	for(auto b : bytes) {
		int c2 = std::to_integer<int>(b & std::byte{0b1111});
		b >>= 4;
		int c1 = std::to_integer<int>(b & std::byte{0b1111});
		hexstring.push_back(hexadecimal[c1]);
		hexstring.push_back(hexadecimal[c2]);
	}
	return hexstring;
}

// 1:3
std::string bits_to_ascii(bytemap data) {
	std::string str;
	for(std::byte b : data) {
		str.push_back(std::to_integer<int>(b));
	}
	return str;
}

// note: THIS IS SHIT!!!!!!!!!! Sort by highest and it'll prob show up top 5.
float english_frequency_analysis(std::string text) {
	std::map<char, float> frequencies {
		{'a', 0.0855}, {'b', 0.0160}, {'c', 0.0316}, {'d', 0.0387}, {'e', 0.1210}, 
		{'f', 0.0218}, {'g', 0.0209}, {'h', 0.0496}, {'i', 0.0733}, {'j', 0.0022}, 
		{'k', 0.0081}, {'l', 0.0421}, {'m', 0.0253}, {'n', 0.0717}, {'o', 0.0747}, 
		{'p', 0.0207}, {'q', 0.0010}, {'r', 0.0633}, {'s', 0.0673}, {'t', 0.0894}, 
		{'u', 0.0268}, {'v', 0.0106}, {'w', 0.0183}, {'x', 0.0019}, {'y', 0.0172}, 
		{'z', 0.0011}
	};
	float total = 0;
	
	for(char c : text) {
		if(frequencies.count(std::tolower(c))) {
			if(c != std::tolower(c)) {
				// makes all caps strings less high
				total += (frequencies[std::tolower(c)] / 2.0f);
			} else {
				total += frequencies[c];
			}
		} else {
			total -= 0.1;
		}
	}
	return total;
}

bytemap xor_repeating_byte(bytemap data, std::byte rep) {
	bytemap modified;
	for(auto b : data) {
		modified.push_back(b ^ rep);
	}
	return modified;
}

// 1:5
bytemap ascii_to_bits(std::string s) {
	bytemap res;
	for(uint8_t c : s) {
		res.push_back(std::byte{c});
	}
	return res;
}

bytemap xor_repeating_key(bytemap data, bytemap key) {
	bytemap encrypted;
	for(int i = 0; i < data.size(); i++) {
		encrypted.push_back(data[i] ^ key[i % key.size()]);
	}
	return encrypted;
}

