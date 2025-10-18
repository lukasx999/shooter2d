#include <gfx.hh>

// 2d twin stick shooter bullet hell game with procedurally generated levels

enum class Direction { North, East, South, West };

class Player {
    gfx::Vec m_position;
    static constexpr float m_movement_speed = 500.0;

public:
    Player(gfx::Vec position)
    : m_position(position)
    {
    }

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    void draw(gfx::Renderer& rd) const {
        rd.draw_circle(m_position, 50.0, gfx::Color::blue());
    }

    void move(Direction dir, float dt) {

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

};

int main() {

    gfx::Window window(1600, 900, "my window", gfx::WindowFlags::DisableVsync);
    gfx::Renderer rd(window);

    Player player({window.get_width()/2.0f, window.get_height()/2.0f});

    rd.draw([&]() {
        float dt = rd.get_frame_time();

        rd.clear_background(gfx::Color::black());

        rd.draw_rectangle(0, 0, window.get_width(), window.get_height(), 0_deg, gfx::Color::grey());
        player.draw(rd);

        if (window.get_key_state(gfx::Key::W).pressed())
            player.move(Direction::North, dt);

        if (window.get_key_state(gfx::Key::S).pressed())
            player.move(Direction::South, dt);

        if (window.get_key_state(gfx::Key::D).pressed())
            player.move(Direction::East, dt);

        if (window.get_key_state(gfx::Key::A).pressed())
            player.move(Direction::West, dt);

        if (window.get_key_state(gfx::Key::Escape).pressed()) {
            window.close();
        }

    });

}
