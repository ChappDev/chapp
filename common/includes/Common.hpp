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

#ifndef CHAPP_COMMON_COMMON_H
#define CHAPP_COMMON_COMMON_H

#include <array>
#include <random>
#include <string>

#include <cinttypes>

namespace Chapp {

    using std::array;
    using std::string;

    // TODO(stek): Turn into a class
    typedef array<uint8_t, 20> phash;
    phash gen_rand_phash();

    enum class GroupType : char {
        Public = 0, // Listed, can be joined freely
        Private,    // Listed, but Password protected
        Protected,  // Unlisted, can be only invited to
    };

    /*!
     * @brief Minimal group struct, used in API
     */
    struct MiniGroup {
        int32_t id{};     /**< Group id*/
        string name;    /**< Group name*/
        GroupType type; /**< Group type*/

        MiniGroup() = default;
    };

    /*!
     * @brief Struct representing "invite" which allows user to join group
     */
    struct GroupInvite {
        phash hash{};       /**< hash for group:id */
        int32_t for_uid{};  /**< uid for which this invite is made */
        MiniGroup group;  /**< group for which this invite is made */

        GroupInvite() = default;
    };

    /*!
     * @brief Minimal group struct, used in API
     */
    struct MiniUser {
        int32_t id{};      /**< User id*/
        string username; /**< Username*/

        MiniUser() = default;
    };

    /*!
     * @brief Abstract message, used in API
     */
    struct Message {
        int32_t from_uid; /**< Message author */
        int32_t to_gid;   /**< Group to which this message is sent*/

        Message() = default;
    };

    /*!
     * @brief Normal text message, used in API
     */
    struct TextMessage : Message {
        string text; /**< Message contents */

        TextMessage() = default;
    };

    // https://stackoverflow.com/a/41154116/5279817
    // TODO(stek): Crypto safe stuff
    template< class Iter, class int_t >
    void fill_with_random_values( Iter start, Iter end, int_t min, int_t max)
    {
        static std::random_device rd;    // you only need to initialize it once
        static std::mt19937 mte(rd());   // this is a relative big object to create

        std::uniform_int_distribution<int_t> dist(min, max);

        std::generate(start, end, [&] () { return dist(mte); });
    }
} // namespace Chapp

#endif
