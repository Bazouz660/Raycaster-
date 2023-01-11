#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "Player.hpp"
#include "defines.hpp"
#include "ResourceManager.hpp"
#include "Ui.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void computeFrameTime();
    float getFrameTime();
    float getFrameRate();

    // System
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    sf::Time m_previousTime;

    // Ui
    UI m_ui;

    // World
    Grid m_grid;
    Player m_player;

    // Grid rendering
    sf::RenderTexture m_gridRenderTexture;
    sf::RectangleShape m_gridDrawable;

    // Raycasting rendering
    sf::RenderTexture m_raycastRenderTexture;
    sf::RectangleShape m_raycastDrawable;

    float m_deltaTime;
};

#endif // GAME_H
