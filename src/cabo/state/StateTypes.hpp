#pragma once

#include "ResourceIds.hpp"

#include <memory>
#include <cstdint>

namespace sf
{
    class RenderWindow;
}

namespace cn::state
{
    using StateId = uint8_t;
    inline constexpr StateId StateIdInvalid = 255;

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
