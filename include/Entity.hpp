#include <SFML/Graphics.hpp>
#include <math.h>
#include "Grid.hpp"

class Entity
{
public:
    Entity();
    // Constructs a new Entity with the given starting position.
    Entity(sf::Vector2f position);

    // Returns the current position of the Entity.
    sf::Vector2f getPosition() const;

    // Sets the current position of the Entity.
    void setPosition(sf::Vector2f position);

    // Returns the current velocity of the Entity.
    sf::Vector2f getVelocity() const;

    // Sets the current velocity of the Entity.
    void setVelocity(sf::Vector2f velocity);

    // Handles collisions with the tiles in the grid.
    bool handleCollisions(Grid &grid);

    sf::Vector2f getDirection();

    sf::Vector2f getDirection(float angle);

    float getAngle();

    // Updates the Entity for a single frame.
    virtual void update(float dt);

    // Draws the Entity to the given window.
    virtual void draw(sf::RenderTarget &target);

    virtual void draw3D(sf::RenderTarget &target, sf::Vector2f cameraPos, float cameraAngle, float fov, float* zbuffer);

protected:
    float getSpriteXPosition(sf::Vector2f playerPosition, float playerRotation, float fov);
    void splitDraw(sf::RenderTarget &window, sf::Vector2f scale,
    float* zBuffer, sf::Vector2f cameraPos, float cameraAngle);
    // The current position of the Entity.
    sf::Vector2f m_position;

    // The current velocity of the Entity.
    sf::Vector2f m_velocity;

    // The current direction vector of the Entity.
    sf::Vector2f m_direction;

    // The current angle of the Entity.
    float m_angle;

    float m_distanceToPlayer;

    // The hitbox of the Entity.
    sf::RectangleShape m_hitbox;

    // The sprite of the Entity.
    sf::Sprite m_sprite;
};
