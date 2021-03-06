#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <odingine/IMainGame.h>
#include <odingine/ResourceManager.h>
#include <random>

GameplayScreen::GameplayScreen(odingine::Window* window) : m_window(window) {
}

GameplayScreen::~GameplayScreen() {

}

int GameplayScreen::getNextScreenIndex() const {
    return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const {
    return SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::build() {
}

void GameplayScreen::destroy() {
}

void GameplayScreen::onEntry() {
    b2Vec2 gravity(0.0f, -9.81);
    m_world = std::make_unique<b2World>(gravity);

    // Make the ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -25.0f);
    b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
    // Make the ground fixture
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Make a bunch of boxes
    std::mt19937 randGenerator;
    std::uniform_real_distribution<float> xPos(-10.0, 10.0f);
    std::uniform_real_distribution<float> yPos(-10.0, 25.0f);
    std::uniform_real_distribution<float> size(0.5, 2.5f);
    std::uniform_int_distribution<int> color(50, 255);
    const int NUM_BOXES = 100;

    for (int i = 0; i < NUM_BOXES; i++) {
        odingine::ColorRGBA8 randColor;
        randColor.r = color(randGenerator);
        randColor.g = color(randGenerator);
        randColor.b = color(randGenerator);
        randColor.a = 255;
        Box newBox;
        newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)), randColor);
        m_boxes.push_back(newBox);
    }

    // Initialize spritebatch
    m_spriteBatch.init();

    // Shader init
    // Compile our color shader
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();

    // Load the texture
    m_texture = odingine::ResourceManager::getTexture("Assets/bricks_top.png");

    // Init camera
    m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
    m_camera.setScale(32.0f);
}

void GameplayScreen::onExit() {
}

void GameplayScreen::update() {
    m_camera.update();
    checkInput();

    // Update the physics simulation
    m_world->Step(1.0f / 60.0f, 6, 2);
}

void GameplayScreen::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_textureProgram.use();

    // Upload texture uniform
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    // Camera matrix
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    m_spriteBatch.begin();

    // Draw all the boxes
    for (auto& b : m_boxes) {
        glm::vec4 destRect;
        destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
        destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
        destRect.z = b.getDimensions().x;
        destRect.w = b.getDimensions().y;
        m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_texture.id, 0.0f, b.getColor(), b.getBody()->GetAngle());
    }

    m_spriteBatch.end();
    m_spriteBatch.renderBatch();
    m_textureProgram.unuse();
}

void GameplayScreen::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
        m_game->onSDLEvent(evnt);
    }
}
