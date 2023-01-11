#include <SFML/Graphics.hpp>
#include <math.h>
#include "Grid.hpp"

class Entity
{
public:
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

    // Updates the Entity for a single frame.
    virtual void update(float dt);

    // Draws the Entity to the given window.
    virtual void draw(sf::RenderTarget &window);

protected:
    // The current position of the Entity.
    sf::Vector2f m_position;

    // The current velocity of the Entity.
    sf::Vector2f m_velocity;

    // The current direction vector of the Entity.
    sf::Vector2f m_direction;

    // The current angle of the Entity.
    float m_angle;

    // The hitbox of the Entity.
    sf::RectangleShape m_hitbox;

    // The sprite of the Entity.
    sf::Sprite m_sprite;
};
