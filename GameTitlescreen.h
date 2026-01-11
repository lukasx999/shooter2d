#pragma once

#include <gfx.h>
#include "GameObject.h"
#include "misc.h"

using namespace std::chrono_literals;

class GameTitlescreen : public IGameObject {
    const gfx::Window& m_window;
    const gfx::Font& m_font;

    gfx::ui::Button m_button { m_window, m_font, "start game", { 0, 0, 500, 100 }, 50 };
    gfx::Animation<int> m_animation{ 0, m_window.get_width(), 5s, gfx::interpolators::ease_in_out_circ };

public:
    GameTitlescreen(const gfx::Window& window, const gfx::Font& font)
        : m_window(window)
        , m_font(font)
    {
        m_button.on_press([&] {
            // TODO: transition state
            m_animation.start();
        });
    }

    void draw(gfx::Renderer& rd) const override {
        rd.draw_text_centered(rd.get_window().get_width()/2.0, 0, 50, "epic game", m_font, gfx::Color::white());
        rd.draw_rectangle(0, 100, m_animation.get(), 50, gfx::Color::red());
        m_button.draw(rd);
    }

    void update([[maybe_unused]] double dt) override {
        m_button.update();
    }

};
