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

#ifndef CHAPP_COMMON_PHASH_H
#define CHAPP_COMMON_PHASH_H

#include <array>
#include <random>

namespace Chapp {
    using std::array;
    using std::string;

    namespace Util {
        // https://stackoverflow.com/a/41154116/5279817
        // TODO(stek): Crypto safe stuff
        template<class Iter, class int_t>
        void fill_with_random_values(Iter start, Iter end, int_t min, int_t max)
        {
            static std::random_device rd;    // you only need to initialize it once
            static std::mt19937 mte(rd());   // this is a relative big object to create

            std::uniform_int_distribution<int_t> dist(min, max);

            std::generate(start, end, [&] () { return dist(mte); });
        }
    }

    class Phash {
        static constexpr std::size_t PHASH_SIZE = 20;
        using uarr_t = array<uint8_t, PHASH_SIZE>;

    public:
        Phash() = default;
        Phash(const Phash&) = default;
        Phash& operator=(const Phash&) = default;
        Phash(Phash&&) = default;
        Phash& operator=(Phash&&) = default;
        ~Phash() = default;

        bool operator==(const Phash &other) const {
            return this->uarr == other.uarr;
        }

        void randFill() {
            // 1 BECAUSE "IT SHOULD BE CONVERTIBLE TO STRING AND HAVE NO \0"
            // I HATE YOU, GEORGE
            Util::fill_with_random_values(uarr.begin(), uarr.end(), 1, UINT8_MAX);
        }

        explicit Phash(const string& str) {
            if (str.size() != PHASH_SIZE) {
                throw std::invalid_argument("Invalid str size");
            }

            auto c_str = str.c_str();
            std::copy(c_str, c_str + PHASH_SIZE, uarr.begin());
        }

        explicit operator string() {
            return std::string(uarr.begin(), uarr.end());
        }

    private:
        uarr_t uarr{};

    };

    using phash = Phash;

    inline phash gen_rand_phash() {
        phash ret{};
        ret.randFill();
        return ret;
    }

}

#endif
