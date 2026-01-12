#pragma once

#include "Entity.h"

class SpriteEntity : public Entity {
public:
    SpriteEntity(gfx::Vec position, int width, int height)
        : Entity(position, width, height)
    { }

    void draw(gfx::Renderer& rd) const override {

        Entity::draw(rd);

        auto& sprite = get_current_sprite(m_direction, m_state);

        bool is_facing_left = m_direction == Direction::West ||
            m_direction == Direction::NorthWest ||
            m_direction == Direction::SouthWest;

        if (is_facing_left)
            sprite.draw_mirrored(rd, get_hitbox());
        else
            sprite.draw(rd, get_hitbox());

    }

    void on_direction_change(Direction new_direction) override {
        auto& new_sprite = get_current_sprite(new_direction, m_state);
        new_sprite.reset();
        new_sprite.start();
    }

    void on_state_change(State new_state) override {
        auto& new_sprite = get_current_sprite(m_direction, new_state);
        new_sprite.reset();
        new_sprite.start();
    }

    bool is_attack_done() const override {
        return get_current_sprite(m_direction, m_state).is_done();
    }

private:
    [[nodiscard]] virtual const gfx::AnimatedTexture& get_current_sprite(Direction direction, State state) const = 0;

    [[nodiscard]] gfx::AnimatedTexture& get_current_sprite(Direction direction, State state) {
        const auto* const_this = static_cast<const SpriteEntity*>(this);
        const auto& sprite = const_this->get_current_sprite(direction, state);
        return const_cast<gfx::AnimatedTexture&>(sprite);
    }

};
