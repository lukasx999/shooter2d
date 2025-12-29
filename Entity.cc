#include "Entity.h"

Entity::Entity(const gfx::Window& window, gfx::Vec position, int width, int height)
    : m_width(width)
    , m_height(height)
    , m_window(window)
    , m_position(position)
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

    // just update the position when attacking while walking, so we dont
    // show the walking animation
    if (m_state != State::Attacking) {

        m_is_idle = false;
        m_idle_lock = false;

        bool direction_has_changed = direction != m_direction;
        // m_is_holding_walk_button prevents conflicting on_direction_change() calls
        // when multiple buttons are pressed at the same time
        if (direction_has_changed && !m_is_holding_walk_button)
            on_direction_change(direction);

        m_direction = direction;

        bool state_has_changed = m_state != State::Walking;
        if (state_has_changed)
            on_state_change(State::Walking);

        m_state = State::Walking;
        m_is_holding_walk_button = true;
    }

    float step = m_movement_speed * dt;

    switch (direction) {
        using enum Direction;
        case North: m_position.y -= step; break;
        case East:  m_position.x += step; break;
        case South: m_position.y += step; break;
        case West:  m_position.x -= step; break;
    }
}

void Entity::update([[maybe_unused]] double dt) {

    switch (m_state) {
        using enum State;

        case Attacking:
            if (is_attack_done())
                m_state = State::Idle;
            break;

        case Idle:
            break;

        case Walking:

            // if the entity has not walked in the last iteration of the game loop
            // we can assume it has stopped walking, and is therefore idle
            // m_idle_lock is here to avoid calling on_state_change() more than once
            if (m_is_idle && !m_idle_lock) {
                m_state = State::Idle;
                on_state_change(State::Idle);
                m_idle_lock = true;
            }

            m_is_idle = true;
            m_is_holding_walk_button = false;
            break;
    }
}

void Entity::attack() {
    if (m_state == State::Attacking) return;

    bool state_has_changed = m_state != State::Attacking;
    if (state_has_changed)
        on_state_change(State::Attacking);

    m_state = State::Attacking;
}
