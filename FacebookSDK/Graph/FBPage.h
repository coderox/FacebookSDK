#pragma once

#include "../FBResult.h"

namespace winsdkfb::Graph
{
    struct FBPage 
    {
		FBPage() = default;
    	
        static std::any FromJson(
            std::wstring const& jsonText
        );

        std::wstring Id() const;
        void Id(std::wstring const& value);

        std::wstring Category() const;
        void Category(std::wstring const& value);

        int CheckIns() const;
        void CheckIns(int const& value);

        std::wstring Description() const;
        void Description(std::wstring const& value);

        int Likes() const;
        void Likes(int const& value);

        std::wstring Link() const;
        void Link(std::wstring const& value);

        std::wstring Name() const;
        void Name(std::wstring const& value);

    private:

        std::wstring _id;
        std::wstring _category;
        int _checkins = 0;
        std::wstring _description;
        int _likes = 0;
        std::wstring _link;
        std::wstring _name;
    };
}
