#pragma once

#include <glm/glm.hpp>

class Player;

class PlayerController {
private:
    Player& m_player;

    float m_bobTime = 0.0f;
    glm::vec3 m_viewBobOffset{ 0.0f };

    float m_fov = 70.0f;

    glm::ivec3 m_breakingBlock{ 0 };
    bool m_hasBreakingBlock = false;
    float m_breakProgress = 0.0f;

    glm::ivec3 m_targetBlock;
    bool m_hasTargetBlock = false;

public:
    PlayerController(Player& player);

    void update(float delta);

    float getFov() const { return m_fov; }
    glm::vec3 getViewBobOffset() const { return m_viewBobOffset; }
   
    bool hasTargetBlock() const { return m_hasTargetBlock; }
    const glm::ivec3& getTargetBlock() const { return m_targetBlock; }

private:
    void updateBlockBreaking(float delta);
};