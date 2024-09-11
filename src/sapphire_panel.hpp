#pragma once
#include <map>
#include <string>

namespace Sapphire
{
    struct ComponentLocation
    {
        float cx;     // x-coordinate of control's center
        float cy;     // y-coordinate of control's center

        ComponentLocation(float _cx, float _cy)
            : cx(_cx)
            , cy(_cy)
            {}
    };

    ComponentLocation FindComponent(
        const std::string& modCode,
        const std::string& label);
}
