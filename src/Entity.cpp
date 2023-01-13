#include "Entity.hpp"
#include "ResourceManager.hpp"
#include "defines.hpp"
#include "Prototypes.hpp"
#include "iostream"

Entity::Entity()
{
    m_hitbox.setSize(sf::Vector2f(32.0f, 32.0f));
    m_hitbox.setOrigin(m_hitbox.getSize() * 0.5f);
    m_hitbox.setPosition(0, 0);
    m_hitbox.setOutlineThickness(1);
    m_hitbox.setOutlineColor(sf::Color::Red);
    m_hitbox.setFillColor(sf::Color::Green);
    m_position = sf::Vector2f(0, 0);
    m_velocity = sf::Vector2f(0, 0);

    m_sprite.setTexture(*ResourceManager::getInstance().getTexture("elgato"));
}

// Constructs a new Entity with the given starting position.
Entity::Entity(sf::Vector2f position)
    : m_position(position), m_velocity(0.0f, 0.0f)
{
    // Set the size and position of the hitbox.
    m_hitbox.setSize(sf::Vector2f(32.0f, 32.0f));
    m_hitbox.setOrigin(m_hitbox.getSize() * 0.5f);
    m_hitbox.setPosition(position);
    m_hitbox.setOutlineThickness(1);
    m_hitbox.setOutlineColor(sf::Color::Red);
    m_hitbox.setFillColor(sf::Color::Green);

    // Set the texture of the sprite.
    m_sprite.setTexture(*ResourceManager::getInstance().getTexture("elgato"));
}

// Handles collisions with the tiles in the grid.
bool Entity::handleCollisions(Grid &grid)
{
    // Get the bounds of the entity.
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();

    // Calculate the indices of the tiles that the entity is colliding with.
    int minRow = std::max(0, (int)(bounds.top / grid.getTileSize()));
    int maxRow = std::min((int)(grid.getHeight() - 1),
    (int)((bounds.top + bounds.height) / grid.getTileSize()));
    int minColumn = std::max(0, (int)(bounds.left / grid.getTileSize()));
    int maxColumn = std::min((int)(grid.getWidth() - 1),
    (int)((bounds.left + bounds.width) / grid.getTileSize()));

    // Check for collisions with the tiles in the grid.
    for (int row = minRow; row <= maxRow; ++row) {
        for (int column = minColumn; column <= maxColumn; ++column) {
            // Skip empty tiles.
            if (grid.getTile(column, row).type == 0)
                continue;

            // Get the bounds of the tile.
            sf::FloatRect tileBounds(column * grid.getTileSize(),
            row * grid.getTileSize(), grid.getTileSize(), grid.getTileSize());

            // Check for a collision with the tile.
            sf::FloatRect intersect;
            if (bounds.intersects(tileBounds, intersect)) {
                // Calculate the overlap between the entity and the tile.
                sf::Vector2f overlap(intersect.width, intersect.height);

                // Check which axis has the smallest overlap.
                if (overlap.x < overlap.y) {
                    // Resolve the collision along the x-axis.
                    if (bounds.left < tileBounds.left) {
                        // Entity is to the left of the tile.
                        m_position.x -= overlap.x;
                        m_velocity.x = 0;
                    } else {
                        // Entity is to the right of the tile.
                        m_position.x += overlap.x;
                        m_velocity.x = 0;
                    }
                } else {
                    // Resolve the collision along the y-axis.
                    if (bounds.top < tileBounds.top) {
                        // Entity is above the tile.
                        m_position.y -= overlap.y;
                        m_velocity.y = 0;
                    } else {
                        // Entity is below the tile.
                        m_position.y += overlap.y;
                        m_velocity.y = 0;
                    }
                }
            }
        }
    }
    return false;
}

// Returns the current position of the Entity.
sf::Vector2f Entity::getPosition() const
{
    return m_position;
}

// Sets the current position of the Entity.
void Entity::setPosition(sf::Vector2f position)
{
    m_position = position;
    m_hitbox.setPosition(position);
}

// Returns the current velocity of the Entity.
sf::Vector2f Entity::getVelocity() const
{
    return m_velocity;
}

// Sets the current velocity of the Entity.
void Entity::setVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

// Updates the Entity for a single frame.
void Entity::update(float dt)
{
    // Update the position based on the velocity.
    m_position += m_velocity * dt;
    m_hitbox.setPosition(m_position);
    m_sprite.setPosition(m_position);
}

sf::Vector2f Entity::getDirection()
{
    m_direction = sf::Vector2f(std::cos(m_angle * RAD), std::sin(m_angle * RAD));
    return m_direction;
}

sf::Vector2f Entity::getDirection(float angle)
{
    m_direction = sf::Vector2f(std::cos(angle * RAD), std::sin(angle * RAD));
    return m_direction;
}

// Draws the Entity to the given window.
void Entity::draw(sf::RenderTarget &target)
{
    // Derived classes should override this function to draw themselves to the window.
    //window.draw(m_sprite);
    target.draw(m_hitbox);
    //target.draw(m_sprite);
}

float Entity::getAngle()
{
    return m_angle;
}

static double getStepTexX(const sf::Texture *texture, float perc)
{
    int tex_size = texture->getSize().x;
    double texX = (perc * (double)tex_size);

    return texX;
}

void Entity::splitDraw(sf::RenderTarget &target, sf::Vector2f scale,
float* zBuffer, sf::Vector2f cameraPos, float cameraAngle)
{
    sf::FloatRect bound = m_sprite.getGlobalBounds();
    int nb_cols = bound.width;
    sf::Texture* texture = (sf::Texture*)m_sprite.getTexture();
    sf::Vector2u textureSize = texture->getSize();
    sf::RectangleShape spriteStripe;
    sf::Vector2f screenPos = m_sprite.getPosition();

    m_sprite.setScale(scale);
    for (int i = 0; i < nb_cols; i++) {
        spriteStripe.setSize(sf::Vector2f(1, textureSize.y));
        spriteStripe.setScale(sf::Vector2f(1, scale.y));
        spriteStripe.setTexture(texture);

        spriteStripe.setTextureRect(sf::IntRect(getStepTexX(texture, ((float)i / (float)nb_cols)), 0, 1, textureSize.y));

        spriteStripe.setPosition(sf::Vector2f(screenPos.x + (i - (nb_cols / 2)), screenPos.y - ((textureSize.y * scale.y))));

        if ((m_distanceToPlayer) < zBuffer[(int)clamp(0, RESX - 1,
        (int)screenPos.x + (i - (nb_cols / 2)) - 1)])
            target.draw(spriteStripe);
    }
}


float Entity::getSpriteXPosition(sf::Vector2f playerPosition, float playerRotation, float fov)
{
    // Get angle between player and sprite
    float angle = atan2(m_position.y - playerPosition.y, m_position.x - playerPosition.x);

    // Convert angle to degrees
    angle = angle * 180.0f / PI;

    // Normalize angle to be between 0 and 360
    while (angle < 0)
        angle += 360.0f;
    while (angle >= 360.0f)
        angle -= 360.0f;

    // Get angle between player's forward vector and sprite
    float spriteAngle = angle - playerRotation;

    // Normalize angle to be between -FOV/2 and FOV/2
    while (spriteAngle < -fov / 2.0f)
        spriteAngle += 360.0f;
    while (spriteAngle >= fov / 2.0f)
        spriteAngle -= 360.0f;

    // Calculate x position on screen
    float x = (spriteAngle + fov / 2.0f) / fov * RESX;

    return x;
}

void Entity::draw3D(sf::RenderTarget &target, sf::Vector2f cameraPos, float cameraAngle, float fov, float* zBuffer)
{
    sf::Vector2f s_pos;
    double distance = DistanceToPlane(cameraPos, cameraAngle, m_position);
    sf::Vector2f scale = sf::Vector2f((300.0f / distance),
    -(300.0f / distance));

    m_distanceToPlayer = distance;
    s_pos.x = getSpriteXPosition(cameraPos, cameraAngle, fov);
    std::cout << s_pos.x << std::endl;
    s_pos.y = (RESY / 2.0) + ((m_sprite.getTexture()->getSize().y / 2) * scale.y);
    m_sprite.setPosition(s_pos);
    m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2);
    splitDraw(target, scale, zBuffer, cameraPos, cameraAngle);
}