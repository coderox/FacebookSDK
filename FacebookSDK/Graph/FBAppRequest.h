#pragma once

#include "FBUser.h"

namespace winsdkfb::Graph
{
    struct FBAppRequest
    {
        static std::any FromJson(
            std::wstring const& jsonText
        );

        std::wstring ActionType() const;
        void ActionType(std::wstring const& value);

        std::wstring CreatedTime() const;
        void CreatedTime(std::wstring const& value);

        std::wstring Data() const;
        void Data(std::wstring const& value);

        winsdkfb::Graph::FBUser From() const;
        void From(winsdkfb::Graph::FBUser value);

        std::wstring Id() const;
        void Id(std::wstring const& value);

        std::wstring Message() const;
        void Message(std::wstring const& value);

        winsdkfb::Graph::FBUser To() const;
        void To(winsdkfb::Graph::FBUser value);

    private:

        std::wstring _action_type;
        std::wstring _created_time;
        std::wstring _data;
        winsdkfb::Graph::FBUser _from;
        std::wstring _id;
        std::wstring _message;
        winsdkfb::Graph::FBUser _to;
    };
}
