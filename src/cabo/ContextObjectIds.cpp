#include "ContextObjectIds.hpp"

namespace cn
{
    
template <>
core::ContextObjectId core::getContextObjectId<sf::RenderWindow>()
{
    return context::id::Window;
}
template <>
core::ContextObjectId core::getContextObjectId<client::TextureHolder>()
{
    return context::id::TextureHolder;
}
template <>
core::ContextObjectId core::getContextObjectId<client::FontHolder>()
{
    return context::id::FontHolder;
}
template <>
core::ContextObjectId core::getContextObjectId<core::event::Dispatcher>()
{
    return context::id::EventDispatcher;
}
template <>
core::ContextObjectId core::getContextObjectId<net::Manager>()
{
    return context::id::NetManager;
}
template <>
core::ContextObjectId core::getContextObjectId<sf::Clock>()
{
    return context::id::SystemClock;
}
template <>
core::ContextObjectId core::getContextObjectId<client::player::Manager>()
{
    return context::id::PlayerManager;
}
template <>
core::ContextObjectId core::getContextObjectId<server::player::Manager>()
{
    return context::id::PlayerManager;
}

} // namespace cn
