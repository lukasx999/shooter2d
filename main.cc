#include <vector>

#include <gfx.hh>

// 2d twin stick shooter bullet hell game with procedurally generated levels

namespace {

enum class Direction { North, East, South, West };

class Projectile {
    gfx::Vec m_position;
    const gfx::Vec m_direction;
    const float m_travel_speed;

public:
    Projectile(gfx::Vec position, gfx::Vec direction, float travel_speed)
        : m_position(position)
        , m_direction(direction)
        , m_travel_speed(travel_speed)
    { }

    void update(float dt) {
        m_position += m_direction * m_travel_speed * dt;
    }

    void draw(gfx::Renderer& rd) const {
        rd.draw_circle(m_position, 5.0, gfx::Color::green());
    }

};

class ProjectileSystem {
    std::vector<Projectile> projectiles;

public:
    ProjectileSystem() = default;

    void draw(gfx::Renderer& rd) const {
        for (auto& projectile : projectiles) {
            projectile.draw(rd);
        }
    }

    void update(float dt) {
        for (auto& projectile : projectiles) {
            projectile.update(dt);
        }
    }

    void add(gfx::Vec position, gfx::Vec direction, float size) {
        projectiles.emplace_back(Projectile{ position, direction, size });
    }

};

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

} // namespace

int main() {

    gfx::Window window(1600, 900, "my window", gfx::WindowFlags::DisableVsync);
    gfx::Renderer rd(window);

    Player player({window.get_width()/2.0f, window.get_height()/2.0f});

    ProjectileSystem projectiles;

    rd.draw([&]() {
        float dt = rd.get_frame_time();

        rd.clear_background(gfx::Color::black());

        rd.draw_rectangle(0, 0, window.get_width(), window.get_height(), 0_deg, gfx::Color::grey());
        player.draw(rd);

        rd.draw_circle(window.get_mouse_pos(), 15.0, gfx::Color::red());
        projectiles.draw(rd);
        projectiles.update(dt);

        if (window.get_mouse_button_state(gfx::MouseButton::LMB).pressed()) {
            auto ppos = player.get_position();
            auto mpos = window.get_mouse_pos();

            auto direction = (mpos - ppos).normalized();
            projectiles.add(ppos, direction, 500.0);

        }

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
