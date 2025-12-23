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
    Cards,
    Field,
    FinishButton,
    MainMenuStartButton,
    MainMenuJoinButton,
    Paw,
    PrivateZone,
    Table,
    TableButtons
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
