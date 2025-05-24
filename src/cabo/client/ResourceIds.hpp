#pragma once

#include "core/save/Save.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "core/ResourceHolder.hpp"

namespace cn::client
{

enum class TextureIds 
{
    Background,
    BackButton,
    CaboButton,
    Cards,
    DecideButtons,
    Field,
    FinishButton,
    MainMenuStartButton,
    MainMenuJoinButton,
    Table,
    YesNoButtons
};

enum class FontIds
{
    Main,
};

enum class SaveIds
{
    PlayerName,
    ServerAddress
};

using TextureHolder = core::ResourceHolder<sf::Texture, TextureIds>;
using FontHolder = core::ResourceHolder<sf::Font, FontIds>;
using SaveHolder = core::ResourceHolder<core::save::Save, SaveIds>;

} // namespace cn::client
