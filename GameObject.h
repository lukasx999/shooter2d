#pragma once

#include <gfx.h>

class IGameObject {
public:
    virtual ~IGameObject() = default;
    virtual void update(double dt) = 0;
    virtual void draw(gfx::Renderer& rd) const = 0;
};
