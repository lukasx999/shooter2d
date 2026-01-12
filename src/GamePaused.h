#pragma once

#include "GameObject.h"

class GamePaused : public GameObject {
    const gfx::Window& m_window;
    const gfx::Font& m_font;

public:
    GamePaused(const gfx::Window& window, const gfx::Font& font)
        : m_window(window)
        , m_font(font)
    { }

    void draw(gfx::Renderer& rd) const override {
        rd.draw_rectangle(0, 0, m_window.get_width(), m_window.get_height(), gfx::Color::black().set_alpha(128));
        rd.draw_text(0, 0, 50, "paused.", m_font, gfx::Color::white());
    }

    void update([[maybe_unused]] double dt) override {
    }

};
