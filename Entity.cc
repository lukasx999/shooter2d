#include "Entity.h"
#include <print>

Entity::Entity(const gfx::Window& window, gfx::Vec position, gfx::Texture texture)
    : m_window(window)
    , m_position(position)
    , m_texture(std::move(texture))
{
    // TODO: reset animation when changing direction
    m_sprite_idle_south.start();
    m_sprite_idle_sidewards.start();
    m_sprite_idle_north.start();
    m_sprite_walking_sidewards.start();
    m_sprite_walking_north.start();
    m_sprite_walking_south.start();
}

void Entity::walk(Direction direction, double dt) {

    m_state = State::Walking;
    m_direction = direction;

    switch (direction) {
        using enum Direction;

        case North:
            m_position.y -= m_movement_speed * dt;
            break;

        case East:
            m_position.x += m_movement_speed * dt;
            break;

        case South:
            m_position.y += m_movement_speed * dt;
            break;

        case West:
            m_position.x -= m_movement_speed * dt;
            break;
    }
}

gfx::Rect Entity::get_hitbox() const {

    float width = m_spritesheet.get_cell_width() * m_texture_scale;
    float height = m_spritesheet.get_cell_height() * m_texture_scale;

    return {
        m_position.x - width / 2.0f,
        m_position.y - height / 2.0f,
        width,
        height,
    };
}

void Entity::update([[maybe_unused]] double dt) {
    if (m_sprite_attacking_south.is_done()) {
        m_state = State::Idle;
        m_sprite_attacking_south.reset();
    }

    if (m_state != State::Attacking)
        m_state = State::Idle;
}

void Entity::draw(gfx::Renderer& rd) const {

    const gfx::AnimatedTexture& sprite = [&] {
        switch (m_state) {
            using enum State;
            using enum Direction;

            case Idle: switch (m_direction) {
                case North: return m_sprite_idle_north;
                case East:
                case West: return m_sprite_idle_sidewards;
                case South: return m_sprite_idle_south;
            }

            case Walking: switch (m_direction) {
                case North: return m_sprite_walking_north;
                case East:
                case West: return m_sprite_walking_sidewards;
                case South: return m_sprite_walking_south;
            }

            case Attacking: switch (m_direction) {
                case North: return m_sprite_attacking_north;
                case East:
                case West: return m_sprite_attacking_sidewards;
                case South: return m_sprite_attacking_south;
            }

        }
    }();

    if (m_direction == Direction::West)
        sprite.draw_mirrored(rd, get_hitbox());
    else
        sprite.draw(rd, get_hitbox());

    // rd.draw_rectangle(get_hitbox(), gfx::Color::red().set_alpha(0x7f));

}

void Entity::attack() {
    if (m_state == State::Attacking) return;

    m_state = State::Attacking;
    m_sprite_attacking_south.start();
}
