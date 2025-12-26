#include "Entity.h"

Entity::Entity(const gfx::Window& window, gfx::Vec position, gfx::Texture texture)
    : m_window(window)
    , m_position(position)
    , m_texture(std::move(texture))
{
    // TODO: reset animation when changing direction
    m_sprite_attack_south.start();
    m_sprite_attack_north.start();
    m_sprite_attack_sidewards.start();
    m_sprite_idle_south.start();
    m_sprite_idle_sidewards.start();
    m_sprite_idle_north.start();
    m_sprite_walking_sidewards.start();
    m_sprite_walking_north.start();
    m_sprite_walking_south.start();
}

void Entity::move(Direction dir, double dt) {
    m_direction = dir;
    m_is_idle = false;

    switch (dir) {
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
    m_is_idle = true;
}

void Entity::draw(gfx::Renderer& rd) const {
    if (m_is_attacking) {
        m_sprite_attack_south.draw(rd, get_hitbox());
        return;
    }

    auto& current_sprite = get_current_sprite(this);

    if (m_direction == Direction::West)
        current_sprite.draw_mirrored(rd, get_hitbox());
    else
        current_sprite.draw(rd, get_hitbox());

    // rd.draw_rectangle(get_hitbox(), gfx::Color::red().set_alpha(0x7f));

}
