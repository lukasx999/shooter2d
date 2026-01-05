#pragma once

#include <algorithm>
#include <filesystem>
#include <ranges>
#include <span>
#include <print>

#include <gfx.h>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Object.hpp>

#include "GameObject.h"
#include "Entity.h"

class Map : public GameObject {
    tmx::Map m_map;
    std::unordered_map<const tmx::Tileset*, gfx::Texture> m_textures;

public:
    explicit Map(const char* path) {

        bool success = m_map.load(path);
        if (!success)
            throw std::runtime_error("failed to load map");

        load_tile_textures();
    }

    void update([[maybe_unused]] double dt) override { }
    void draw(gfx::Renderer& rd) const override;
    void resolve_collisions(const gfx::Window& window, std::span<std::reference_wrapper<Entity>> entities, double dt);

    // TODO: move out of this class
    void resolve_collisions_entities(Entity& e1, const Entity& e2, double dt) {
        gfx::Rect p = e2.get_hitbox();
        resolve_collision_rects(p, e1, dt);
    }

private:
    // get the scaling factor so the map fits the window
    [[nodiscard]] gfx::Vec get_map_scaling_factor(const gfx::Window& window) const;
    void for_each_tile(std::function<void(uint32_t gid, gfx::Vec dest)> fn) const;

    void load_tile_textures() {
        auto& tilesets = m_map.getTilesets();
        for (auto& tileset : tilesets) {

            auto tex_path = tileset.getImagePath();
            if (!std::filesystem::exists(tex_path)) {
                std::println("image doesnt exist");
                exit(1);
            }

            m_textures.emplace(&tileset, gfx::Texture(tex_path));

        }
    }

    [[nodiscard]] const tmx::Tileset& find_tileset(uint32_t gid) const {

        auto& tilesets = m_map.getTilesets();
        auto ts = std::ranges::find_if(tilesets, [&](const tmx::Tileset& ts) {
            return gid >= ts.getFirstGID() && gid <= ts.getLastGID();
        });

        assert(ts != tilesets.cend());
        return *ts;
    }

    [[nodiscard]] static constexpr gfx::Color tmx_color_to_gfx_color(tmx::Colour color) {
        // HACK: use a cleaner approach
        return *reinterpret_cast<gfx::Color*>(&color);
    }

    static void resolve_collision_rects(gfx::Rect b, Entity& entity, double dt) {

        // subtracted from the height of the collision hitbox, otherwise
        // the player would clip through the tile and trigger a wrong collision
        // it is set to the amount of pixels the player can move at the current frame
        float diff = entity.get_movement_speed() * dt;

        // width of the collision hitbox
        float collision_size = 1;

        // add a tiny collision rectangle for each side of the tile so we
        // know which tile was hit
        gfx::Rect left {
            b.x - collision_size,
            b.y + diff,
            collision_size,
            b.height - diff * 2,
        };

        gfx::Rect right {
            static_cast<float>(b.x) + b.width,
            b.y + diff,
            collision_size,
            b.height - diff * 2,
        };

        gfx::Rect top {
            b.x + diff,
            b.y - collision_size,
            b.width - diff * 2,
            collision_size,
        };

        gfx::Rect bottom {
            b.x + diff,
            static_cast<float>(b.y) + b.height,
            b.width - diff * 2,
            collision_size,
        };

        gfx::Vec pos = entity.get_position();

        auto a = entity.get_hitbox();

        if (a.check_collision(left))
            entity.set_position({ b.x - a.width / 2.0f - 1, pos.y });

        if (a.check_collision(right))
            entity.set_position({ b.x + b.width + a.width / 2.0f + 1, pos.y });

        if (a.check_collision(top))
            entity.set_position({ pos.x, b.y - a.height / 2.0f - 1 });

        if (a.check_collision(bottom))
            entity.set_position({ pos.x, b.y + b.height + a.height / 2.0f + 1 });
    }

};
