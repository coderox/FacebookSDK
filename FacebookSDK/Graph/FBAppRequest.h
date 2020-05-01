#pragma once

#include <winrt/base.h>

#include "FBUser.h"

namespace winsdkfb::Graph
{
    struct FBAppRequest
    {
        static std::shared_ptr<winsdkfb::Graph::FBAppRequest> FromJson(
            winrt::hstring const& JsonText
        );

        winrt::hstring ActionType();
        void ActionType(winrt::hstring const& value);

        winrt::hstring CreatedTime();
        void CreatedTime(winrt::hstring const& value);

        winrt::hstring Data();
        void Data(winrt::hstring const& value);

        std::shared_ptr<winsdkfb::Graph::FBUser> From();
        void From(std::shared_ptr<winsdkfb::Graph::FBUser> value);

        winrt::hstring Id();
        void Id(winrt::hstring const& value);

        winrt::hstring Message();
        void Message(winrt::hstring const& value);

        std::shared_ptr<winsdkfb::Graph::FBUser> To();
        void To(std::shared_ptr<winsdkfb::Graph::FBUser> value);

    private:

        winrt::hstring _action_type;
        winrt::hstring _created_time;
        winrt::hstring _data;
        std::shared_ptr<winsdkfb::Graph::FBUser> _from{ nullptr };
        winrt::hstring _id;
        winrt::hstring _message;
        std::shared_ptr<winsdkfb::Graph::FBUser> _to{ nullptr };
    };
}
