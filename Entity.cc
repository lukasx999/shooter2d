#include "Entity.h"
#include <print>

Entity::Entity(const gfx::Window& window, gfx::Vec position, gfx::Texture texture, int width, int height)
    : m_window(window)
    , m_position(position)
    , m_texture(std::move(texture))
    , m_width(width)
    , m_height(height)
{ }

gfx::Rect Entity::get_hitbox() const {
    return {
        m_position.x - m_width / 2.0f,
        m_position.y - m_height / 2.0f,
        static_cast<float>(m_width),
        static_cast<float>(m_height),
    };
}

void Entity::walk(Direction direction, double dt) {

    bool direction_has_changed = direction != m_direction;
    if (direction_has_changed)
        on_direction_change(direction);

    bool state_has_changed = m_state != State::Walking;
    if (state_has_changed)
        on_state_change(State::Walking);

    m_state = State::Walking;
    m_direction = direction;

    switch (direction) {
        using enum Direction;
        case North: m_position.y -= m_movement_speed * dt; break;
        case East:  m_position.x += m_movement_speed * dt; break;
        case South: m_position.y += m_movement_speed * dt; break;
        case West:  m_position.x -= m_movement_speed * dt; break;
    }
}

void Entity::update([[maybe_unused]] double dt) {
    // TODO: this is a bad approach
    m_state = State::Idle;
}

void Entity::attack() {

    bool state_has_changed = m_state != State::Attacking;
    if (state_has_changed)
        on_state_change(State::Attacking);

    m_state = State::Attacking;
}
