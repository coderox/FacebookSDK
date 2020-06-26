#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBGroup
    {
	    static std::any FromJson(
            std::wstring const& jsonText
        );

        std::wstring Id() const;
        void Id(std::wstring const& value);

        std::wstring Description() const;
        void Description(std::wstring const& value);

        std::wstring Email() const;
        void Email(std::wstring const& value);

        std::wstring Icon() const;
        void Icon(std::wstring const& value);

        std::wstring Link() const;
        void Link(std::wstring const& value);

        std::wstring Name() const;
        void Name(std::wstring const& value);

        std::wstring Privacy() const;
        void Privacy(std::wstring const& value);

    private:

        std::wstring _id;
        std::wstring _description;
        std::wstring _email;
        std::wstring _icon;
        std::wstring _link;
        std::wstring _name;
        std::wstring _privacy;
    };
}
