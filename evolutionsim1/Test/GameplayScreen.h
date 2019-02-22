#pragma once

#include "Box.h"
#include <odingine/IGameScreen.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <odingine/SpriteBatch.h>
#include <odingine/GLSLProgram.h>
#include <odingine/Camera2D.h>
#include <odingine/GLTexture.h>
#include <odingine/Window.h>

// Our custom gameplay screen that inherits from the IGameScreen
class GameplayScreen : public odingine::IGameScreen {
public:
    GameplayScreen(odingine::Window* window);
    ~GameplayScreen();

    virtual int getNextScreenIndex() const override;

    virtual int getPreviousScreenIndex() const override;

    virtual void build() override;

    virtual void destroy() override;

    virtual void onEntry() override;

    virtual void onExit() override;

    virtual void update() override;

    virtual void draw() override;

private:
    void checkInput();

    odingine::SpriteBatch m_spriteBatch;
    odingine::GLSLProgram m_textureProgram;
    odingine::Camera2D m_camera;
    odingine::GLTexture m_texture;
    odingine::Window* m_window;

    std::vector<Box> m_boxes;
    std::unique_ptr<b2World> m_world;
};

