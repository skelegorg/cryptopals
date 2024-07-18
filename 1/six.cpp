#include "util.h"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>

#include <algorithm>

int hamming_distance(bytemap s1, bytemap s2) {
	int counter = 0;
	for(int i = 0; i < s1.size(); i++) {
		std::byte checker{0b10000000};
		while(std::to_integer<int>(checker) > 0) {
			if(std::to_integer<int>((s1[i] & checker) ^ (s2[i] & checker)) > 0) {
				counter++;
			}
			checker >>= 1;
		}
	}
	return counter;
}

bytemap base64decode(std::string data) {
	bytemap result;
	std::vector<bool> rawbits;

	int padding = 0;
	for(char c : data) {
		uint8_t base64num;
		if(c >= 48 && c <= 57) {
			base64num = c + 4;
		} else if (c >= 65 && c <= 90) {
			base64num = c - 65;
		} else if (c >= 97 && c <= 122) {
			base64num = c - 71;
		} else if (c == 43) {
			base64num = 62;
		} else if (c == 47) {
			base64num = 63;
		} else if (c == 61){
			padding++;
		} else {
			std::cout << "(-): base64decode: illegal character" << std::endl;
		}
		std::byte tmpb{base64num};
		std::byte checker{0b00100000};
		for(int i = 2; i < 8; i++) {
			// check last 6 bits of the number and add them to the rawbits
			rawbits.push_back((std::to_integer<int>(checker & tmpb) > 0));
			checker >>= 1;
		}
	}

	// = discarded, fill in missing least significant bits
	while(rawbits.size() % 8 != 0) {
		rawbits.push_back(false);
	}

	// parse by 8s
	for(int i = 0; i < (rawbits.size() / 8); i++) {
		std::byte newb{0};
		for(int j = 0; j < 8; j++) {
			if(j < 7) {
				if(rawbits[(i * 8) + j]) {
					newb |= std::byte{0b1};
				}
				newb <<= 1;
			} else {
				if(rawbits[(i * 8) + j]) {
					newb |= std::byte{0b1};
				}
			}
		}
		result.push_back(newb);
	}

	return result;
}


struct Entry {
	int keylen;
	float normalized_ham;
};
#include <algorithm>
bool entrysorter(Entry const& lhs, Entry const& rhs) {
	return lhs.normalized_ham< rhs.normalized_ham;
}

int main() {
	print_bitset(base64decode("Jk8DCkkcC3hFMQIEC0EbAVIqCFZBO1IdBgZUVA4QTgUWSR4QJwwRTWM="));
}

/*
int main() {
	std::string line;
	std::ifstream file("6.txt");
	bytemap rawdata;

	while(std::getline(file, line)) {
		bytemap bits = base64decode(line);
		for(std::byte b : bits) {
			rawdata.push_back(b);
		}
	}

	print_bitset(rawdata);

	std::vector<Entry> entries;
	
	for(int keysize = 2; keysize < 40; keysize++) {
		Entry ent;
		ent.keylen = keysize;
		bytemap block1;
		bytemap block2;
		for(int i = 0; i < keysize; i++) {
			block1.push_back(rawdata[i]);
		}
		for(int i = keysize; i < (keysize * 2); i++) {
			block2.push_back(rawdata[i]);
		}
		ent.normalized_ham = (float)hamming_distance(block1, block2) / (float)keysize;
		entries.push_back(ent);
	}

	std::sort(entries.begin(), entries.end(), &entrysorter);

	// check the top 3 key sizes
	for(int i = 0; i < 3; i++) {
		int keysize = entries[i].keylen;
		std::vector<bytemap> split_bits;

		// initialize each section's bytemaps
		for(int j = 0; j < keysize; j++) {
			bytemap newbmap;
			newbmap.push_back(rawdata[j]);
			split_bits.push_back(newbmap);
		}

		// NOTE: check this shit its probably whats broken tbh
		// populate each section's bytemap
		for(int j = 1; j < (rawdata.size() / keysize); j++) {
			for(int k = 0; k < keysize; k++) {
				if((j * 8) + k < rawdata.size()) {
					split_bits[k].push_back(rawdata[(j * 8) + k]);
				} else {
					break;
				}
			}
		}

	
		
		std::string fullkey;

		// solve each split as one single byte xor
		for(bytemap bmp : split_bits) {
			bytemap xored;
			std::vector<Entry> entries;
			for(uint8_t key = 0; key < 128; key++) {
				Entry currententry;
				xored = xor_repeating_byte(bmp, std::byte{key});
				currententry.keylen = key;
				currententry.normalized_ham = english_frequency_analysis(bits_to_ascii(xored));
				entries.push_back(currententry);
			}
			std::sort(entries.begin(), entries.end(), &entrysorter);
			fullkey.push_back(entries[0].keylen);
		}

		std::cout << "for keysize " << keysize << " key is most likely " << fullkey << std::endl;

	}

}
*/
