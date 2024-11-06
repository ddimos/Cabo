#pragma once

#include "ResourceIds.hpp"
#include <memory>

namespace sf
{
    class RenderWindow;
}

namespace cn::state
{
    class State;
    using StatePtr = std::unique_ptr<State>;

    struct Context
    {
        Context(sf::RenderWindow& _windowRef, TextureHolder& _textureHolderRef, FontHolder& _fontHolderRef)
            : windowRef(_windowRef), textureHolderRef(_textureHolderRef), fontHolderRef(_fontHolderRef) {}

        sf::RenderWindow& windowRef;
        TextureHolder& textureHolderRef;
        FontHolder& fontHolderRef;
    };
} // namespace cn::state
