#include <print>

#include <gfx.hh>

#include "player.hh"
#include "projectiles.hh"
#include "world.hh"

// 2d twin stick shooter bullet hell game with procedurally generated levels

namespace {

class Game {
    gfx::Window& m_window;
    Player m_player{{m_window.get_width()/2.0f, m_window.get_height()/2.0f}};
    World m_world;
    ProjectileSystem m_projectiles;

    double m_last_shot = 0.0;
    static constexpr double m_shot_delay = 0.25;

public:
    explicit Game(gfx::Window& window)
    : m_window(window)
    { }

    void draw(gfx::Renderer& rd) const {
        m_world.draw(rd);
        m_player.draw(rd);
        m_projectiles.draw(rd);

    }

    void update(float dt) {
        m_projectiles.update(dt);

        m_world.resolve_collisions(m_player);

        bool can_shoot = m_window.get_time() > m_last_shot + m_shot_delay;

        if (m_window.get_mouse_button_state(gfx::MouseButton::LMB).pressed() && can_shoot) {
            m_last_shot = m_window.get_time();
            auto ppos = m_player.get_position();
            auto mpos = m_window.get_mouse_pos();

            auto direction = (mpos - ppos).normalized();
            m_projectiles.add(ppos, direction, 500.0);

        }

        if (m_window.get_key_state(gfx::Key::W).pressed())
            m_player.move(Direction::North, dt);

        if (m_window.get_key_state(gfx::Key::S).pressed())
            m_player.move(Direction::South, dt);

        if (m_window.get_key_state(gfx::Key::D).pressed())
            m_player.move(Direction::East, dt);

        if (m_window.get_key_state(gfx::Key::A).pressed())
            m_player.move(Direction::West, dt);

        if (m_window.get_key_state(gfx::Key::Escape).pressed())
            m_window.close();

    }

};

} // namespace

int main() {

    gfx::Window window(1600, 900, "my window", gfx::WindowFlags::DisableVsync);
    gfx::Renderer rd(window);

    Game game(window);

    rd.draw([&]() {

        rd.clear_background(gfx::Color::black());

        float dt = rd.get_frame_time();
        game.draw(rd);
        game.update(dt);

    });

}
