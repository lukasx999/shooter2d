#pragma once

#include <chrono>

#include <gfx.h>
#include "GameObject.h"

template <typename T>
class Animation {
    T m_start;
    T m_end;
    double m_duration;
    double m_start_time = 0.0;

    enum class State {
        Idle,
        Running,
    } m_state = State::Idle;

public:
    Animation(T start, T end, double duration)
        : m_start(start)
        , m_end(end)
        , m_duration(duration)
    { }

    void start() {
        m_start_time = get_current_time();
        m_state = State::Running;
    }

    void reset() {
        m_start_time = 0.0;
        m_state = State::Idle;
    }

    [[nodiscard]] bool is_done() const {
        double diff = get_current_time() - m_start_time;
        return diff >= m_duration;
    }

    [[nodiscard]] T get() const {
        switch (m_state) {
            case State::Idle:
                return m_start;

            case State::Running:
                double diff = get_current_time() - m_start_time;

                if (is_done()) return m_end;

                double normalized = m_duration / diff; // 0..1
                return lerp(m_start, m_end, normalized);
        }
    }

private:
    [[nodiscard]] static constexpr T lerp(T start, T end, float t) {
        return start + (end-start) * t;
    }

    [[nodiscard]] static double get_current_time() {
        namespace chrono = std::chrono;
        auto now = chrono::steady_clock::now().time_since_epoch();
        return chrono::duration_cast<chrono::milliseconds>(now).count();
    }

};

class GameTitlescreen : public IGameObject {
    const gfx::Window& m_window;
    const gfx::Font& m_font;

    gfx::ui::Button m_button { m_window, m_font, "start game", { 0, 0, 500, 100 }, 50 };
    Animation<int> m_animation{ 0, m_window.get_width(), 5.0 };

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
        rd.draw_rectangle(0, 0, m_animation.get(), 50, gfx::Color::red());
        m_button.draw(rd);
    }

    void update([[maybe_unused]] double dt) override {
        m_button.update();
    }

};
