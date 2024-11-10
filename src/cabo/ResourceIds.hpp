#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "core/ResourceHolder.hpp"

namespace cn
{

enum class TextureIds 
{
    Background,
    MainMenuStartButton
};

enum class FontIds
{
    Main,
};

using TextureHolder = core::ResourceHolder<sf::Texture, TextureIds>;
using FontHolder = core::ResourceHolder<sf::Font, FontIds>;

} // namespace cn
