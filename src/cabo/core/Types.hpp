#pragma once

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::core
{

struct Context
{
    Context(sf::RenderWindow& _windowRef, TextureHolder& _textureHolderRef, FontHolder& _fontHolderRef)
        : windowRef(_windowRef), textureHolderRef(_textureHolderRef), fontHolderRef(_fontHolderRef) {}

    sf::RenderWindow& windowRef;
    TextureHolder& textureHolderRef;
    FontHolder& fontHolderRef;
};

} // namespace cn::core
