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

#ifndef CHAPP_COMMON_API_HANDLER
#define CHAPP_COMMON_API_HANDLER

#include <chapp.pb.h>
#include <User.hpp>

namespace Chapp {

    class APIHandler {

    public:
        static API::ReqResp&& handleRequest(User& from, API::ReqResp &req);

    private:

    #define defMethod_thefuck(mtd, x)\
        static API::ReqResp&& x##mtd(User& from, const API::RQ##mtd &r)

    #define defMethod(mtd) defMethod_thefuck(mtd, )

        defMethod(sendMessage);
        defMethod(joinGroup);
        defMethod(leaveGroup);
        defMethod(inviteUser);
        defMethod(createGroup);
        defMethod(listGroups);
        defMethod(listUsers);

    #undef defMethod
    #undef defMethod_thefuck

    };
}
#endif // CHAPP_COMMON_API_HANDLER
