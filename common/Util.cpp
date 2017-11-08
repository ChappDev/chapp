// The MIT License (MIT)
//
// Copyright (c) 2017 Viktor Oreshkin <imselfish@stek29.rocks>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "Util.hpp"

#include <array>
#include <stdexcept>
#include <string>


namespace Chapp {
    namespace Util {
        using std::string;

        string hex_to_string(const string& input) {
            constexpr auto lut = "0123456789ABCDEF";
            size_t len = input.length();
            if ((len & 1) != 0u) {
                throw std::invalid_argument("odd length");
            }

            std::string output;
            output.reserve(len / 2);
            for (size_t i = 0; i < len; i += 2) {
                char a = input[i];
                const char* p = std::lower_bound(lut, lut + 16, a);
                if (*p != a) {
                    throw std::invalid_argument("not a hex digit");
                }

                char b = input[i + 1];
                const char* q = std::lower_bound(lut, lut + 16, b);
                if (*q != b) {
                    throw std::invalid_argument("not a hex digit");
                }

                output.push_back(((p - lut) << 4) | (q - lut));
            }

            return output;
        }

    }  // namespace Util
} // namespace Chapp
