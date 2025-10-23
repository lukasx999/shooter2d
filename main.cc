#include <print>
#include <tuple>
#include <fstream>
#include <ranges>
#include <filesystem>

#include <gfx.hh>

#include "player.hh"
#include "projectiles.hh"
#include "world.hh"

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>

// 2d twin stick shooter bullet hell game with procedurally generated levels

namespace {

class Game {
    gfx::Renderer& m_renderer;
    gfx::Window& m_window;
    gfx::Font m_font;
    Player m_player;
    World m_world;
    ProjectileSystem m_projectiles;

    double m_last_shot = 0.0;
    static constexpr double m_shot_delay = 0.25;

public:
    explicit Game(gfx::Renderer& renderer, gfx::Window& window)
        : m_renderer(renderer)
        , m_window(window)
        , m_font(m_renderer.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_player({ m_window.get_width() / 2.0f, m_window.get_height() / 2.0f })
        , m_world(m_window)
    { }

    void draw(gfx::Renderer& rd) const {

        rd.set_camera(m_player.get_position());
        rd.with_camera([&] {
            m_world.draw(rd);
            m_player.draw(rd);
            m_projectiles.draw(rd);
        });

        int size = 50;
        auto text = "Health: 0";
        int text_width = m_font.measure_text(text, size);
        rd.draw_text(m_window.get_width()/2.0 - text_width/2.0, size, size, text, m_font, gfx::Color::red());

    }

    void update(double dt) {
        m_projectiles.update(dt);


        bool can_shoot = m_window.get_time() > m_last_shot + m_shot_delay;

        if (m_window.get_mouse_button_state(gfx::MouseButton::Left).pressed() && can_shoot) {
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

        m_world.resolve_collisions(m_player, dt);

    }

};

} // namespace

int main() {

    tmx::Map map;
    bool success = map.load("./assets/map.tmx");
    if (!success) {
        std::println("failed to load map");
        return EXIT_FAILURE;
    }

    auto tile_size = map.getTileSize();

    auto& layers = map.getLayers();
    auto& layer = layers[0];
    auto tilesets = map.getTilesets();
    auto layer_size = layer->getSize();

    auto& tileset = tilesets[0];
    uint32_t first_gid = tileset.getFirstGID();
    uint32_t last_gid = tileset.getLastGID();

    auto tileset_size = tileset.getTileSize();
    auto tileset_columns = tileset.getColumnCount();
    auto tileset_rows = tileset.getTileCount() / tileset_columns;

    auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();

    auto path = tileset.getImagePath();
    if (!std::filesystem::exists(path)) {
        std::println("image doesnt exist");
        return EXIT_FAILURE;
    }

    std::ifstream image(path);
    std::vector<char> img(std::istreambuf_iterator<char>(image), {});


    gfx::Window window(1600, 900, "shooter2d", gfx::WindowFlags::None);
    gfx::Renderer renderer(window);

    gfx::Texture tex(path);

    Game game(renderer, window);

    renderer.draw([&] {

        renderer.clear_background(gfx::Color::black());

        // double dt = renderer.get_frame_time();
        // game.draw(renderer);
        // game.update(dt);

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();

        renderer.draw_texture(0, 0, tileset_size.x * tileset_columns, tileset_size.y * tileset_rows, 0_deg, tex);

        for (auto&& [i, tile] : tiles | std::views::enumerate) {
            uint32_t id = tile.ID;

            int dest_x = i % layer_size.x;
            int dest_y = i / layer_size.x;

            if (id == 0) {
            }

            if (id >= first_gid && id <= last_gid) {

                uint32_t local_id = id - first_gid;
                int src_x = local_id % tileset_columns;
                int src_y = local_id / tileset_columns;

                renderer.draw_rectangle(
                    src_x*tileset_size.x,
                    src_y*tileset_size.y,
                    tileset_size.x,
                    tileset_size.y,
                    gfx::Color::white().set_alpha(245)
                );

            }

            // renderer.draw_rectangle(dest_x*tile_size.x, dest_y*tile_size.y, tile_size.x, tile_size.y, color);

        }

        // int n = 300;
        // int size = 5;
        // int spacing = 5;
        // for (int x = 0; x < n; ++x) {
        //     for (int y = 0; y < n; ++y) {
        //         renderer.draw_circle(x*(size+spacing), y*(size+spacing), size, gfx::Color::blue());
        //     }
        // }


    });

}
