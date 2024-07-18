#include "util.h"
#include <cstddef>
#include <cstdint>


int main() {
	bytemap data = hex_to_bitset("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");

	bytemap xored;
	float scores[128];
	uint8_t maxscore_index;
	float maxscore = 0;
	for(uint8_t i = 0; i < 128; i++) {
		xored = xor_repeating_byte(data, std::byte{i});
		std::cout << bits_to_ascii(xored) << " | " << english_frequency_analysis(bits_to_ascii(xored)) << std::endl;
		scores[i] = english_frequency_analysis(bits_to_ascii(xored));
		if(scores[i] > maxscore) {
			maxscore = scores[i];
			maxscore_index = i;
		}
	}

	std::cout << "max score letter: " << maxscore << std::endl;
	std::cout << bits_to_ascii(xor_repeating_byte(data, std::byte{maxscore_index})) << std::endl;
}
