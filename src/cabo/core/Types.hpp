#pragma once

#include "core/event/Dispatcher.hpp"
#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::core
{

struct Context
{
    Context(sf::RenderWindow& _windowRef, TextureHolder& _textureHolderRef, FontHolder& _fontHolderRef, core::event::Dispatcher& _eventDispatcher)
        : windowRef(_windowRef), textureHolderRef(_textureHolderRef), fontHolderRef(_fontHolderRef), eventDispatcher(_eventDispatcher) {}

    sf::RenderWindow& windowRef;
    TextureHolder& textureHolderRef;
    FontHolder& fontHolderRef;
    core::event::Dispatcher& eventDispatcher;
};

} // namespace cn::core
