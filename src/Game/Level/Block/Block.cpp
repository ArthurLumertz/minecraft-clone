#include "Block.h"

const Block Block::Air(
    0,
    Region{ 0, 0, 0, 0 },
    Region{ 0, 0, 0, 0 },
    Region{ 0, 0, 0, 0 },
    0.0f
);

const Block Block::Grass(
    1,
    Region{ 0, 0, 16, 16 },
    Region{ 32, 0, 16, 16 },
    Region{ 16, 0, 16, 16 },
    0.6f
);

const Block Block::Dirt(
    2,
    Region{ 32, 0, 16, 16 },
    Region{ 32, 0, 16, 16 },
    Region{ 32, 0, 16, 16 },
    0.5f
);

const Block Block::Stone(
    3,
    Region{ 0, 16, 16, 16 },
    Region{ 0, 16, 16, 16 },
    Region{ 0, 16, 16, 16 },
    1.5f
);

const Block Block::Log(
    4,
    Region{ 64, 0, 16, 16 },
    Region{ 64, 0, 16, 16 },
    Region{ 48, 0, 16, 16 },
    2.0f
);

const Block Block::Leaves(
    5,
    Region{ 48, 16, 16, 16 },
    Region{ 48, 16, 16, 16 },
    Region{ 48, 16, 16, 16 },
    0.2f,
    true,
    true
);

const Block Block::Water(
    6,
    Region{ 0, 32, 16, 16 },
    Region{ 0, 32, 16, 16 },
    Region{ 0, 32, 16, 16 },
    -1.0f,
    false,
    false
);

const Block Block::Sand(
    7,
    Region{ 32, 16, 16, 16 },
    Region{ 32, 16, 16, 16 },
    Region{ 32, 16, 16, 16 },
    0.5f
);

std::array<const Block*, 256> Block::s_blocks{};

Block::Block(
    u32 id,
    const Region& top,
    const Region& bottom,
    const Region& side,
    float hardness,
    bool solid,
    bool opaque
)
    : m_id(id),
    m_solid(solid),
    m_opaque(opaque),
    m_hardness(hardness),
    m_topRegion(top),
    m_bottomRegion(bottom),
    m_sideRegion(side) {

    s_blocks[id] = this;
}

const Block& Block::get(u32 id) {
    if (id >= s_blocks.size() || s_blocks[id] == nullptr) {
        return Air;
    }

    return *s_blocks[id];
}

const Region& Block::getRegion(Face face) const {
    switch (face) {
    case Face::Top:
        return m_topRegion;

    case Face::Bottom:
        return m_bottomRegion;

    default:
        return m_sideRegion;
    }
}