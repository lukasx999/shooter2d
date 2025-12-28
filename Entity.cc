#include "Entity.h"

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

    m_is_idle = false;
    m_idle_lock = false;

    bool direction_has_changed = direction != m_direction;
    if (direction_has_changed)
        on_direction_change(direction);

    m_direction = direction;

    bool state_has_changed = m_state != State::Walking;
    if (state_has_changed)
        on_state_change(State::Walking);

    m_state = State::Walking;

    switch (direction) {
        using enum Direction;
        case North: m_position.y -= m_movement_speed * dt; break;
        case East:  m_position.x += m_movement_speed * dt; break;
        case South: m_position.y += m_movement_speed * dt; break;
        case West:  m_position.x -= m_movement_speed * dt; break;
    }
}

void Entity::update([[maybe_unused]] double dt) {

    // if the entity has not walked in the last iteration of the game loop
    // we can assume it has stopped walking, and is therefore idle
    // m_idle_lock is here to avoid calling on_state_change() more than once
    if (m_is_idle && !m_idle_lock) {
        m_state = State::Idle;
        on_state_change(State::Idle);
        m_idle_lock = true;
    }

    m_is_idle = true;
}

void Entity::attack() {

    bool state_has_changed = m_state != State::Attacking;
    if (state_has_changed)
        on_state_change(State::Attacking);

    m_state = State::Attacking;
}
