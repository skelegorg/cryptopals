#include "util.h"
#include <cstdint>
#include <fstream>

int main() {
	std::string line;
	std::ifstream file("4.txt");

	std::vector<std::string> viable_strings;

	while(std::getline(file, line)) {
		bytemap bits = hex_to_bitset(line);
		for(uint8_t c = 0; c < 128; c++) {
			std::string decoded = bits_to_ascii(xor_repeating_byte(bits, std::byte{c}));
			float score = english_frequency_analysis(decoded);
			if(score > 0.0f) {
				viable_strings.push_back(decoded);
			}
		}
	}

	for(auto s : viable_strings) {
		std::cout << s << " | " << english_frequency_analysis(s) << std::endl;
	}
}
