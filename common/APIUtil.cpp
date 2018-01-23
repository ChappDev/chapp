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

#include <chapp.pb.h>
#include <APIUtil.hpp>
#include <Errors.hpp>

namespace Chapp::API::Util {
    ReqResp&& ErrorToAPIResp(Error err) {
        auto resp = API::ReqResp();
        resp.set_kind(API::ReqResp::resp);

        #define convertError_thefuck(err, x)\
            case Error::x##err: \
                return MakeAPIError(std::move(resp), 400, "ERR_SRV_" #err)

        #define convertError(err) convertError_thefuck(err, )

        switch (err) {
            case Error::Ok:
                resp.set_inline_type(ReqResp::okresp);
                return std::move(resp);

            convertError(NotInGroup);
            convertError(AlreadyInGroup);
            convertError(AlreadyInvited);
            convertError(InvalidUserId);
            convertError(InvalidGroupId);
            convertError(IncorrectHash);
        }

        #undef convertError
        #undef convertError_thefuck
    }
}
