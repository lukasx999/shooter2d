#pragma once

#include "Entity.h"

class SpriteEntity : public Entity {

private:
    [[nodiscard]] virtual gfx::AnimatedTexture& get_current_sprite(Direction direction, State state) = 0;
    [[nodiscard]] virtual const gfx::AnimatedTexture& get_current_sprite(Direction direction, State state) const = 0;

public:
    SpriteEntity(const gfx::Window& window, gfx::Vec position, int width, int height)
    : Entity(window, position, width, height)
    { }

    void draw(gfx::Renderer& rd) const override {

        auto& sprite = get_current_sprite(m_direction, m_state);

        if (m_direction == Direction::West)
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
        auto& sprite = get_current_sprite(m_direction, m_state);
        return sprite.is_done();
    }

};
