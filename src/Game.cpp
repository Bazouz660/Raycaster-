#include "iostream"
#include "Game.hpp"
#include "defines.hpp"
#include "Prototypes.hpp"
#include <string>

Game::Game()
    : m_window(sf::VideoMode(1920, 1080), "Game"), m_grid(loadGridFromFile("maps/level2.map")),
    m_player(sf::Vector2f(0, 0), m_grid)
{
    // Grid rendering setup
    m_gridRenderTexture.create(m_grid.getWidth() * m_grid.getTileSize(),
    m_grid.getHeight() * m_grid.getTileSize());
    m_gridRenderTexture.setSmooth(true);
    m_gridDrawable.setSize(sf::Vector2f(m_gridRenderTexture.getSize()));
    m_gridDrawable.setOrigin(m_gridDrawable.getSize() * 0.5f);
    m_gridDrawable.setScale(0.2, -0.2);
    m_gridDrawable.setPosition(m_gridDrawable.getGlobalBounds().width * 0.5,
    m_gridDrawable.getGlobalBounds().height * 0.5);
    m_gridDrawable.setTexture(&m_gridRenderTexture.getTexture());

    // Raycasting rendering setup
    m_raycastRenderTexture.create(RESX, RESY);
    m_raycastDrawable.setSize(sf::Vector2f(RESX, RESY));
    m_raycastDrawable.setOrigin(m_raycastDrawable.getSize() * 0.5f);
    m_raycastDrawable.setScale(3, 3);
    m_raycastDrawable.setPosition(m_window.getDefaultView().getCenter());

    m_player.setPosition(gridToScreen(1, 1, m_grid.getTileSize()));

    // Init test entity
    m_entity.setPosition(gridToScreen(15, 15, m_grid.getTileSize()));

    m_previousTime = m_clock.getElapsedTime();
    m_deltaTime = 0;
    updateView(&m_window);
}

void Game::run()
{
    while (m_window.isOpen()) {
        processEvents();
        if (m_window.isOpen() == false)
            return;
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
            return;
        }
        if (event.type == sf::Event::Resized)
            updateView(&m_window);  
    }
}

void Game::computeFrameTime()
{
    // Get the current time.
    sf::Time currentTime = m_clock.getElapsedTime();

    // Calculate the elapsed time since the last frame.
    m_deltaTime = currentTime.asSeconds() - m_previousTime.asSeconds();

    // Store the current time as the previous time.
    m_previousTime = currentTime;
}

float Game::getFrameTime()
{
    return m_deltaTime;
}

float Game::getFrameRate()
{
    return (1.0 / getFrameTime());
}

void Game::update()
{
    // Compute the elapsed time since the last frame
    computeFrameTime();

    m_ui.getText("fpsCounter").setString(std::to_string((int)getFrameRate()));

    // Update game logic here, using dt as the elapsed time
    //handleMouseInput(m_window);
    m_player.update(getFrameTime());
    m_player.handleCollisions(m_grid);
    m_entity.update(getFrameTime());
    m_entity.handleCollisions(m_grid);
}

void Game::render()
{
    // Clear the screen
    m_raycastRenderTexture.clear();
    m_gridRenderTexture.clear();
    m_window.clear();

    // Render game here
    std::vector<Raycaster::Ray> rays = m_player.getRaycaster().getRays();
    sf::VertexArray raysArray;
    std::vector<sf::RectangleShape>& wallSections = m_player.getRaycaster().getWallSections();
    raysArray.setPrimitiveType(sf::PrimitiveType::Lines);

    sf::RectangleShape test;
    test.setFillColor(sf::Color::White);
    test.setSize(sf::Vector2f(RESX, RESY));
    test.setTexture(m_player.getRaycaster().getFloorGroundTexture());
    test.setOrigin(RESX / 2, RESY / 2);
    test.setPosition(RESX / 2, RESY / 2);
    test.setScale(1, 1);

    m_raycastRenderTexture.draw(test);
    //m_raycastDrawable.setTexture(m_player.getRaycaster().getFloorGroundDrawable()->getTexture());
    //m_window.draw(m_raycastDrawable);

    for (int i = 0; i < rays.size(); i++) {
        raysArray.append(rays[i].vertices[0]);
        raysArray.append(rays[i].vertices[1]);
        m_raycastRenderTexture.draw(wallSections.at(i));
    }

    m_entity.draw3D(m_raycastRenderTexture, m_player.getPosition(), m_player.getAngle(), m_player.getFov() ,m_player.getRaycaster().getzBuffer());


    m_raycastDrawable.setTexture(&m_raycastRenderTexture.getTexture());
    m_window.draw(m_raycastDrawable);

    // Draw the grid.
    m_grid.draw(m_gridRenderTexture);
    m_player.draw(m_gridRenderTexture);
    m_entity.draw(m_gridRenderTexture);
    m_gridRenderTexture.draw(raysArray);
    m_gridDrawable.setTexture(&m_gridRenderTexture.getTexture());
    m_window.draw(m_gridDrawable);

    // Draw the UI
    m_ui.drawText(m_window);

    m_window.display();
}
