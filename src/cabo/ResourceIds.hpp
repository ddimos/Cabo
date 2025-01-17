#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "core/ResourceHolder.hpp"

namespace cn
{

enum class TextureIds 
{
    Background,
    Cards,
    MainMenuJoinButton,
    DecideButtons,
    MainMenuStartButton,
    Table
};

enum class FontIds
{
    Main,
};

using TextureHolder = core::ResourceHolder<sf::Texture, TextureIds>;
using FontHolder = core::ResourceHolder<sf::Font, FontIds>;

} // namespace cn
