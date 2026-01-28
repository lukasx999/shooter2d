#pragma once

#include <gfx/gfx.h>

#include "GameObject.h"

class PhysicalObject : public GameObject {
protected:
    gfx::Vec m_position;
    const int m_width;
    const int m_height;
    bool m_show_hitbox = true;

public:
    PhysicalObject(gfx::Vec position, int width, int height)
        : m_position(position)
        , m_width(width)
        , m_height(height)
    { }

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    [[nodiscard]] gfx::Rect get_hitbox() const {
        return {
            m_position.x - m_width / 2.0f,
            m_position.y - m_height / 2.0f,
            static_cast<float>(m_width),
            static_cast<float>(m_height),
        };
    }

    void set_position(gfx::Vec position) {
        m_position = position;
    }

    void show_hitbox(bool show_hitbox) {
        m_show_hitbox = show_hitbox;
    }

    void draw(gfx::Renderer& rd) const override {
        if (m_show_hitbox)
            rd.draw_rectangle(get_hitbox(), gfx::Color::red().set_alpha(128));
    }

    void update([[maybe_unused]] double dt) override { }

};
