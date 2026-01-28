#pragma once

#include <gfx/gfx.h>

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
    explicit Map(const char* path);

    void update([[maybe_unused]] double dt) override { }
    void draw(gfx::Renderer& rd) const override;
    void resolve_collisions(const gfx::Window& window, Entity& entity, double dt);

    // TODO: move out of this class
    void resolve_collisions_entities(Entity& e1, const Entity& e2, double dt) {
        gfx::Rect p = e2.get_hitbox();
        resolve_collision_rects(p, e1, dt);
    }

private:
    // get the scaling factor so the map fits the window
    [[nodiscard]] gfx::Vec get_map_scaling_factor(const gfx::Surface& surface) const;
    void for_each_tile(std::function<void(uint32_t gid, gfx::Vec dest)> fn) const;

    void load_tile_textures();

    [[nodiscard]] const tmx::Tileset& find_tileset(uint32_t gid) const;

    [[nodiscard]] static constexpr gfx::Color tmx_color_to_gfx_color(tmx::Colour color) {
        // HACK: use a cleaner approach
        return *reinterpret_cast<gfx::Color*>(&color);
    }

    static void resolve_collision_rects(gfx::Rect b, Entity& entity, double dt);

};
