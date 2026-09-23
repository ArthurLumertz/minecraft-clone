#pragma once

#include <array>

#include "../../../Engine/Core/Types.h"
#include "../../../Engine/Graphics/Texture/Region.h"
#include "../../../Engine/Graphics/Mesh/CubeMesh.h"

class Block {
public:
    static const Block Air;
    static const Block Grass;
    static const Block Dirt;
    static const Block Stone;
    static const Block Log;
    static const Block Leaves;
    static const Block Water;
    static const Block Sand;

    static const Block& get(u32 id);

    u32 getId() const { return m_id; }
    bool isSolid() const { return m_solid; }
    bool isOpaque() const { return m_opaque; }
    float getHardness() const { return m_hardness; }

    const Region& getRegion(Face face) const;

private:
    static std::array<const Block*, 256> s_blocks;

    Block(
        u32 id,
        const Region& top,
        const Region& bottom,
        const Region& side,
        float hardness,
        bool solid = true,
        bool opaque = false
    );

    u32 m_id;
    bool m_solid;
    bool m_opaque;

    float m_hardness;

    Region m_topRegion;
    Region m_bottomRegion;
    Region m_sideRegion;
};