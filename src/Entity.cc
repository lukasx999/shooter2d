#include "Entity.h"

Entity::Entity(gfx::Vec position, int width, int height)
: PhysicalObject(position, width, height)
{ }

void Entity::walk(Direction direction, double dt) {

    if (m_state == State::Attacking) return;

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

    float step = m_movement_speed * dt;

    switch (direction) {
        using enum Direction;

        case North:
            m_position.y -= step;
            break;

        case East:
            m_position.x += step;
            break;

        case South:
            m_position.y += step;
            break;

        case West:
            m_position.x -= step;
            break;

        case NorthEast:
            m_position.y -= step;
            m_position.x += step;
            break;

        case SouthEast:
            m_position.x += step;
            m_position.y += step;
            break;

        case SouthWest:
            m_position.x -= step;
            m_position.y += step;
            break;

        case NorthWest:
            m_position.x -= step;
            m_position.y -= step;
            break;
    }
}

void Entity::update([[maybe_unused]] double dt) {

    if (m_is_first_iteration) {
        on_state_change(State::Idle);
        m_is_first_iteration = false;
    }

    switch (m_state) {
        using enum State;

        case Attacking:
            if (is_attack_done())
                m_state = State::Idle;
            break;

        case Idle:
            break;

        case Walking:

            // if the entity has not walked in the previous iteration of the game loop
            // we can assume it has stopped walking, and is therefore idle
            // m_idle_lock is here to avoid calling on_state_change() more than once
            if (m_is_idle && !m_idle_lock) {
                on_state_change(State::Idle);
                m_state = State::Idle;
                m_idle_lock = true;
            }

            m_is_idle = true;
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
