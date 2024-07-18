#include "util.h"

typedef std::vector<std::byte> bytemap;

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


int main() {
	std::vector<std::byte> hex1 = hex_to_bitset("1c0111001f010100061a024b53535009181c");
	std::vector<std::byte> hex2 = hex_to_bitset("686974207468652062756c6c277320657965");
	std::vector<std::byte> result = xor_equal_length_bitsets(hex1, hex2);
	std::cout << bitset_to_hex(result) << std::endl;
}
