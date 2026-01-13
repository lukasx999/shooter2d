#include "StatePlaying.h"
#include "GameStates.h"

GameState* StatePlaying::update(double dt) {

    for (auto& obj : m_objects)
    obj.get().update(dt);

    if (m_window.get_key_state(gfx::Key::J).pressed())
        m_player.attack();

    if (m_window.get_key_state(gfx::Key::P).pressed())
        return &m_states.m_state_paused;

    for (auto& entity : m_entities)
    m_map.resolve_collisions(m_window, entity, dt);

    m_map.resolve_collisions_entities(m_enemy, m_player, dt);

    if (!m_health_pickup.is_consumed())
        m_health_pickup.check_collision_player(m_player);

    return nullptr;

}
