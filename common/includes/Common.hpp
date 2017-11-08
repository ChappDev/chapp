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

namespace Chapp {
    // User/Group id
    using chapp_id_t = int;
} // namespace Chapp

#include "GroupTypes.hpp"
#include "Phash.hpp"

#include <string>

#include <cinttypes>

namespace Chapp {

    using std::string;

    /*!
     * @brief Minimal group struct, used in API
     */
    struct MiniGroup {
        chapp_id_t id;     /**< Group id*/
        string name;    /**< Group name*/
        GroupType type; /**< Group type*/

        MiniGroup() = delete;
    };

    /*!
     * @brief Struct representing "invite" which allows user to join group
     */
    struct GroupInvite {
        Phash hash{};       /**< hash for group:id */
        chapp_id_t for_uid;  /**< uid for which this invite is made */
        MiniGroup group;  /**< group for which this invite is made */

        GroupInvite() = delete;
    };

    /*!
     * @brief Minimal group struct, used in API
     */
    struct MiniUser {
        chapp_id_t id;      /**< User id*/
        string username; /**< Username*/

        MiniUser() = delete;
    };

    /*!
     * @brief Abstract message, used in API
     */
    struct Message {
        chapp_id_t from_uid; /**< Message author */
        chapp_id_t to_gid;   /**< Group to which this message is sent*/

        Message() = delete;
    };

    /*!
     * @brief Normal text message, used in API
     */
    struct TextMessage : Message {
        string text; /**< Message contents */

        TextMessage() = delete;
    };

} // namespace Chapp

#endif
