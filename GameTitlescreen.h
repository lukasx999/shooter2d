#pragma once

#include <gfx.h>
#include "GameObject.h"

class GameTitlescreen : public IGameObject {
    const gfx::Window& m_window;
    const gfx::Font& m_font;

    gfx::ui::Button m_button { m_window, m_font, "start game", { 0, 0, 500, 100 }, 50 };

public:
    GameTitlescreen(const gfx::Window& window, const gfx::Font& font)
        : m_window(window)
        , m_font(font)
    {
        m_button.on_press([&] {
            // TODO: transition state
        });
    }

    void draw(gfx::Renderer& rd) const override {
        rd.draw_text_centered(rd.get_window().get_width()/2.0, 0, 50, "epic game", m_font, gfx::Color::white());
        m_button.draw(rd);
    }

    void update([[maybe_unused]] double dt) override {
        m_button.update();
    }

};
