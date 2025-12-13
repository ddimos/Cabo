#include "shared/game/Placement.hpp"
#include "core/Assert.hpp"

namespace
{
    constexpr float EDGE_OFFSET = 50.f;
    constexpr float OFFSET_1 = 450.f;
    constexpr float OFFSET_2 = 250.f;
    constexpr float POS_X_1 = EDGE_OFFSET;
    constexpr float POS_X_2 = 960.f - OFFSET_1;
    constexpr float POS_X_3 = 960.f - OFFSET_2;
    constexpr float POS_X_4 = 960.f;
    constexpr float POS_X_5 = 960.f + OFFSET_2;
    constexpr float POS_X_6 = 960.f + OFFSET_1;
    constexpr float POS_X_7 = 1920.f - EDGE_OFFSET;
    constexpr float POS_Y_1 = EDGE_OFFSET;
    constexpr float POS_Y_2 = 540.f;
    constexpr float POS_Y_3 = 1080.f - EDGE_OFFSET;
}

namespace cn::shared::game::placement
{

Placement getDeck()
{
    return { sf::Vector2f(810.f, 540.f), 0.f };
}

Placement getDiscard()
{
    return { sf::Vector2f(1110.f, 540.f), 0.f };
}

std::vector<Placement> getParticipant(unsigned _participantNumber)
{
    switch (_participantNumber)
    {
    case 1:
        return {
            {{ POS_X_4, POS_Y_3 }, 0.f}
        };
    case 2:
        return {
            {{ POS_X_4, POS_Y_3 }, 0.f},
            {{ POS_X_4, POS_Y_1 }, 180.f}
        };
    case 3:
        return {
            {{ POS_X_4, POS_Y_3 }, 0.f},
            {{ POS_X_3, POS_Y_1 }, 180.f}, {{ POS_X_5, POS_Y_1 }, 180.f}
        };
    case 4:
        return {
            {{ POS_X_3, POS_Y_3 }, 0.f},   {{ POS_X_5, POS_Y_3 }, 0.f},
            {{ POS_X_3, POS_Y_1 }, 180.f}, {{ POS_X_5, POS_Y_1 }, 180.f}
        };
    case 5:
        return {
            {{ POS_X_3, POS_Y_3 }, 0.f},   {{ POS_X_5, POS_Y_3 }, 0.f},
            {{ POS_X_1, POS_Y_2 }, 90.f},  {{ POS_X_7, POS_Y_2 }, -90.f},
            {{ POS_X_4, POS_Y_1 }, 180.f}
        };
    case 6:
        return {
            {{ POS_X_3, POS_Y_3 }, 0.f},   {{ POS_X_5, POS_Y_3 }, 0.f},
            {{ POS_X_1, POS_Y_2 }, 90.f},  {{ POS_X_7, POS_Y_2 }, -90.f},
            {{ POS_X_3, POS_Y_1 }, 180.f}, {{ POS_X_5, POS_Y_1 }, 180.f}
        };
    case 7:
        return {
            {{ POS_X_2, POS_Y_3 }, 0.f},   {{ POS_X_4, POS_Y_3 }, 0.f},   {{ POS_X_6, POS_Y_3 }, 0.f},
            {{ POS_X_1, POS_Y_2 }, 90.f},  {{ POS_X_7, POS_Y_2 }, -90.f},
            {{ POS_X_3, POS_Y_1 }, 180.f}, {{ POS_X_5, POS_Y_1 }, 180.f}
        };
    case 8:
        return {
            {{ POS_X_2, POS_Y_3 }, 0.f},   {{ POS_X_4, POS_Y_3 }, 0.f},   {{ POS_X_6, POS_Y_3 }, 0.f},
            {{ POS_X_1, POS_Y_2 }, 90.f},  {{ POS_X_7, POS_Y_2 }, -90.f},
            {{ POS_X_2, POS_Y_1 }, 180.f}, {{ POS_X_4, POS_Y_1 }, 180.f}, {{ POS_X_6, POS_Y_1 }, 180.f}
        };
    
    default:
        CN_ASSERT(false);
        return {};
    }
}

Placement getButton(TableButtonType _type)
{
    float y = 0;
    switch (_type)
    {
    case TableButtonType::Deal:
        y = 540.f - 30.f;
        break;
    case TableButtonType::ResetAndShuffle:
        y = 540.f;
        break;
    case TableButtonType::ShuffleFromDiscard:
        y = 540.f + 30.f;
        break;
    }
    return {{ 960.f, y }, 0.f };
}

} // namespace cn::shared::game::placement
