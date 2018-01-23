#include <iostream>
#include <string>
#include <iomanip>
#include "Common.hpp"

#include <string>

using Chapp::Util::string_to_hex;

int main(void) {
    for (int i = 0; i < 10; i++) {
        auto hash1 = Chapp::Phash::RandFilled();
        std::cout << string_to_hex((std::string) hash1) << '\n';

        Chapp::Phash hash2((std::string) hash1);

        std::cout << "is equal? " << (hash1 == hash2) << '\n';

        std::cout << string_to_hex((std::string) hash2) << '\n';

        std::cout << std::endl;
    }

    return 0;
}
