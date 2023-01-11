#include "Player.hpp"
#include "iostream"
#include "defines.hpp"

Player::Player(sf::Vector2f position, Grid& grid) : Entity(position),
m_raycaster(grid)
{
    m_angle = 0;

    // The number of rays to cast.
    m_numRays = RESX;

    // The angle range to cast the rays in.
    m_angleRange = 70.f;
}

// Handles input from the player and updates the Player.
void Player::handleInput()
{
    static sf::Vector2i lastMousePos = sf::Mouse::getPosition();
    sf::Vector2i diff;
    float length;

    // Set the velocity based on the current keyboard state.
    sf::Vector2f velocity(0.0f, 0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        velocity = getDirection();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        velocity = -getDirection();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        velocity = getDirection(m_angle - 90);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        velocity = getDirection(m_angle + 90);

    // Normalize the velocity to ensure a consistent speed.
    length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length > 0.0f)
        velocity /= length;

    // Rotate view accordingly to the mouse movement
    diff = lastMousePos - sf::Mouse::getPosition();
    if (diff.x != 0.0f || diff.y != 0.0f)
        m_angle -= diff.x * 0.5;

    // Set the velocity of the player.
    setVelocity(velocity);

    lastMousePos = sf::Mouse::getPosition();
}

// Returns the rays cast by the player.
Raycaster& Player::getRaycaster()
{
    return m_raycaster;
}

// Updates the Player for a single frame.
void Player::update(float dt)
{
    // Update the position of the player based on its velocity.
    handleInput();
    m_position = getPosition();
    m_position += (getVelocity() * dt) * 150.0f;
    setPosition(m_position);

    // Casts rays from the player and stores them in the raycaster object
    m_raycaster.setRays(m_raycaster.castRays(m_position,
    m_angle * RAD, m_numRays, m_angleRange * RAD));
    m_raycaster.computeWallSections();
}
