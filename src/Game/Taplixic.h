#pragma once

#include "../Engine/Core/Types.h"

#include "../Engine/Core/Application.h"
#include "../Engine/Core/Window.h"
#include "../Engine/Core/Input.h"

#include "../Engine/Graphics/Renderer.h"
#include "../Engine/Graphics/Texture/Texture.h"
#include "../Engine/Graphics/Mesh/CubeMesh.h"
#include "../Engine/Graphics/Mesh/Mesh.h"

#include "../Engine/Graphics/Camera/Camera.h"

#include "../Engine/Graphics/Shader/Shader.h"
#include "../Engine/Graphics/Shader/StaticShader.h"

#include "Level/Level.h"
#include "Level/LevelRenderer.h"

#include "Level/Entity/Entity.h"
#include "Level/Entity/Player/Player.h"

#include "Gui/Gui.h"

class Taplixic : public Application {
public:
	Taplixic();

protected:
	void update(float deltaTime) override;
	void render() override;
    void onResize(int width, int height) override;

    void moveCameraToPlayer();

private:
    Camera m_camera;
    Camera m_guiCamera;

    StaticShader m_shader;
    StaticShader m_guiShader;

    Renderer m_renderer;
    
    Level m_level;
    LevelRenderer m_levelRenderer;

    Player m_player;

    Gui m_gui;
};