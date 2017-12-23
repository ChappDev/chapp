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

#include <APIHandler.hpp>
#include <APIUtil.hpp>
#include <GroupFactory.hpp>
#include <GroupTypes.hpp>

namespace Chapp {
    using namespace API::Util;

    API::ReqResp&& APIHandler::handleRequest(User& from, API::ReqResp &req) {
        API::ReqResp resp;
        resp.set_kind(API::ReqResp::resp);

        if (req.kind() != API::ReqResp::req) {
            return MakeAPIError(std::move(resp), 400, "ERR_INVALID_KIND");
        }


#define handleMethod_thefuck(mtd, x)\
            case API::ReqResp::mtd##x:\
                return (x##mtd(from, req.GetExtension(API::RQ##mtd::inner)))

#define handleMethod(mtd) handleMethod_thefuck(mtd, )


        switch (req.inline_type()) {
            handleMethod(sendMessage);
            handleMethod(joinGroup);
            handleMethod(leaveGroup);
            handleMethod(inviteUser);
            handleMethod(createGroup);
            handleMethod(listGroups);
            handleMethod(listUsers);

            default:
                return MakeAPIError(std::move(resp), 400, "ERR_INVALID_INNER_TYPE");
        }

#undef handleMethod_thefuck
#undef handleMethod

    }


    #define implMethod_thefuck(mtd, x)\
        API::ReqResp&& APIHandler::x##mtd(User& from, const API::RQ##mtd &r)

    #define implMethod(mtd) implMethod_thefuck(mtd,)


    implMethod(sendMessage) {
        auto group = GroupFactory::Instance()->by_id(r.gid());
        if (group) {
            return ErrorToAPIResp(group->broadcast(from.id, r.msg()));
        } else {
            return ErrorToAPIResp(Error::InvalidGroupId);
        }
    }

    implMethod(joinGroup) {
        auto group = GroupFactory::Instance()->by_id(r.gid());

        if (group == nullptr) {
            return ErrorToAPIResp(Error::InvalidGroupId);
        }

        Phash phash;

        if (r.has_phash()) {
            try {
                phash = Phash(r.phash());
            } catch(std::invalid_argument&) {
                return ErrorToAPIResp(Error::IncorrectHash);
            }
        }

        return ErrorToAPIResp(group->join(from.id, phash));
    }

    implMethod(leaveGroup) {
        auto group = GroupFactory::Instance()->by_id(r.gid());

        if (group == nullptr) {
            return ErrorToAPIResp(Error::InvalidGroupId);
        }

        return ErrorToAPIResp(group->leave(from.id));
    }

    implMethod(inviteUser) {
        auto group = GroupFactory::Instance()->by_id(r.gid());

        if (group == nullptr) {
            return ErrorToAPIResp(Error::InvalidGroupId);
        }

        return ErrorToAPIResp(group->invite(from.id, r.uid()));
    }

    implMethod(createGroup) {
        API::ReqResp resp;
        resp.set_kind(API::ReqResp::resp);

        return MakeAPIError(std::move(resp), 501, "Not implemented");
    }

    implMethod(listGroups) {
        API::ReqResp resp;
        resp.set_kind(API::ReqResp::resp);

        return MakeAPIError(std::move(resp), 501, "Not implemented");
    }

    implMethod(listUsers) {
        auto group = GroupFactory::Instance()->by_id(r.gid());

        if (group == nullptr) {
            return ErrorToAPIResp(Error::InvalidGroupId);
        }

        if (!group->has_user(from.id)) {
            return ErrorToAPIResp(Error::NotInGroup);
        }

        auto list = group->list_users();

        API::ReqResp resp;
        resp.set_kind(API::ReqResp::resp);
        resp.set_inline_type(API::ReqResp::listUsers);
        auto rp_inner = resp.MutableExtension(API::RPlistUsers::inner);

        // isn't this dangerous?..
        google::protobuf::RepeatedPtrField<API::User> p_list(
                list.begin(), list.end()
        );
        rp_inner->mutable_users()->Swap(&p_list);

        return std::move(resp);
    }

    #undef implMethod
    #undef implMethod_thefuck
}