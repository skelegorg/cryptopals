#include "util.h"

int main() {
	std::string unencrypted = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	bytemap data = ascii_to_bits(unencrypted);
	bytemap key = ascii_to_bits("ICE");

	bytemap encrypted = xor_repeating_key(data, key);
	std::cout << bitset_to_hex(encrypted) << std::endl;
	
}
