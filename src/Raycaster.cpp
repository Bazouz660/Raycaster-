#include <SFML/Graphics.hpp>
#include <math.h>
#include "Raycaster.hpp"
#include "Prototypes.hpp"
#include "defines.hpp"
#include "ResourceManager.hpp"
#include <iostream>

Raycaster::Raycaster(Grid& grid)
    : m_grid(grid)
{
    m_floorGroundImage.create(RESX, RESY);

    for (int x = 0; x < RESX; x++) {
        for (int y = 0; y < RESY; y++) {
            m_floorGroundImage.setPixel(x, y, sf::Color::Transparent);
        }
    }
}

Raycaster::~Raycaster()
{
}

std::vector<Raycaster::Ray> Raycaster::castRays(sf::Vector2f position,
float angle, int nbRays, float angleRange)
{
    std::vector<Raycaster::Ray> rays;
    rays.reserve(nbRays);

    // Used calculate correct casting angle
    float halfLen = tan(angleRange / 2.0);
    float segLen = halfLen / (RESX / 2.0);

    for (int i = 0; i < nbRays; ++i) {
        
        // Calculate the angle of the ray.
        float rayAngle = angle + atan((segLen * i - halfLen));

        // Cast the ray.
        rays.push_back(castRay(position, rayAngle, angle, i));
    }
    return rays;
}

void Raycaster::setRays(std::vector<Raycaster::Ray> rays)
{
    m_rays = rays;
}

std::vector<Raycaster::Ray> Raycaster::getRays()
{
    return m_rays;
}

Raycaster::Ray Raycaster::castRay(sf::Vector2f position, float angle, float refAngle, int x)
{
    Ray ray;
    sf::Vector2f rayStart = position / (float)m_grid.getTileSize();
    sf::Vector2f rayDir = normalize(sf::Vector2f(std::cos(angle), std::sin(angle)));
    sf::Vector2f unitStepSize = {std::sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x)),
    std::sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y))};
    sf::Vector2i mapCheck = {(int)rayStart.x, (int)rayStart.y};
    sf::Vector2f rayLength1D;
    sf::Vector2i step;
    sf::Vector2f intersection;
    sf::Texture* texture = ResourceManager::getInstance().getTexture("wall");
    sf::Image* image = ResourceManager::getInstance().getTextureImage("wall");
    bool tileFound = false;
    float distance = 0.0f;
    float perpWallDist = 0.0f;

    if (rayDir.x < 0) {
        step.x = -1;
        rayLength1D.x = (rayStart.x - float(mapCheck.x)) * unitStepSize.x;
    } else {
        step.x = 1;
        rayLength1D.x = (float(mapCheck.x + 1) - rayStart.x) * unitStepSize.x;
    }
    if (rayDir.y < 0) {
        step.y = -1;
        rayLength1D.y = (rayStart.y - float(mapCheck.y)) * unitStepSize.y;
    } else {
        step.y = 1;
        rayLength1D.y = (float(mapCheck.y + 1) - rayStart.y) * unitStepSize.y;
    }

    while (!tileFound && distance < RENDER_DISTANCE) {
        if (rayLength1D.x < rayLength1D.y) {
            mapCheck.x += step.x;
            distance = rayLength1D.x;
            rayLength1D.x += unitStepSize.x;
            ray.side = 0;
        } else {
            mapCheck.y += step.y;
            distance = rayLength1D.y;
            rayLength1D.y += unitStepSize.y;
            ray.side = 1;
        }

        if (mapCheck.x >= 0 && mapCheck.x < m_grid.getWidth()
            && mapCheck.y >= 0 && mapCheck.y < m_grid.getHeight()) {
            if (m_grid.getTile(mapCheck.x, mapCheck.y).type != 0) {
                tileFound = true;
            }
        }
    }

    if (tileFound) {
        intersection = rayStart + rayDir * distance;
    } else
        intersection = rayStart;

    float angleFromRef = refAngle - angle;
    if (angleFromRef < 0)
        angleFromRef += 2 * PI;
    if (angleFromRef > 2 * PI)
        angleFromRef -= 2 * PI;

    ray.length = (distance * float(m_grid.getTileSize())) * std::cos(angleFromRef);

    // Calculate distance of perpendicular ray (Euclidean distance would give fisheye effect!)
    if(ray.side == 0)
        perpWallDist = (rayLength1D.x - unitStepSize.x);
    else
        perpWallDist = (rayLength1D.y - unitStepSize.y);

    //calculate value of wallX
    double wallX; //where exactly the wall was hit
    if (ray.side == 0) wallX = rayStart.y + perpWallDist * rayDir.y;
    else           wallX = rayStart.x + perpWallDist * rayDir.x;
    wallX -= floor((wallX));




    //Calculate height of line to draw on screen
    int lineHeight = (int)(RESY / (ray.length / m_grid.getTileSize()));

    //calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + RESY / 2;
    if(drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + RESY / 2;
    if(drawEnd >= RESY) drawEnd = RESY - 1;

    // FLOOR CASTING (vertical version, directly after drawing the vertical wall stripe for the current x)
    double floorXWall, floorYWall; // x, y position of the floor texel at the bottom of the wall

    // 4 different wall directions possible
    if (ray.side == 0 && rayDir.x > 0) {
        floorXWall = mapCheck.x;
        floorYWall = mapCheck.y + wallX;
    } else if (ray.side == 0 && rayDir.x < 0) {
        floorXWall = mapCheck.x + 1.0;
        floorYWall = mapCheck.y + wallX;
    }
    else if (ray.side == 1 && rayDir.y > 0) {
        floorXWall = mapCheck.x + wallX;
        floorYWall = mapCheck.y;
    } else {
        floorXWall = mapCheck.x + wallX;
        floorYWall = mapCheck.y + 1.0;
    }

    double distWall, currentDist;

    distWall = (ray.length / m_grid.getTileSize());

    if (drawEnd < 0)
    drawEnd = RESY; //becomes < 0 when the integer overflows

    //draw the floor from drawEnd to the bottom of the screen
    for(int y = drawEnd + 1; y < RESY; y++) {
        currentDist = RESY / (2.0 * y - RESY); //you could make a small lookup table for this instead

        double weight = (currentDist) / (distWall);

        double currentFloorX = weight * floorXWall + (1.0 - weight) * rayStart.x;
        double currentFloorY = weight * floorYWall + (1.0 - weight) * rayStart.y;

        int floorTexX, floorTexY;
        floorTexX = int(currentFloorX * texture->getSize().x * 1.25) % texture->getSize().x;
        floorTexY = int(currentFloorY * texture->getSize().y * 1.25) % texture->getSize().y;

        //floor
        m_floorGroundImage.setPixel(x, y, image->getPixel(floorTexX, floorTexY));
        //ceiling (symmetrical!)
        m_floorGroundImage.setPixel(x, RESY - y - 1, image->getPixel(floorTexX, floorTexY));
    }







    ray.wallX = wallX;
    ray.color = m_grid.getTile(mapCheck.x, mapCheck.y).color;;
    ray.direction = rayDir;
    ray.origin = position;
    ray.vertices[0].position = ray.origin;
    ray.vertices[0].color = ray.color;
    ray.vertices[1].position = (intersection * float(m_grid.getTileSize()));
    ray.vertices[1].color = ray.color;
    return ray;
}

double Raycaster::getStepX(sf::Texture* texture, Ray ray)
{
    int tex_size = texture->getSize().x;
    double texX = (ray.wallX * (double)tex_size);
    return texX;
}

sf::Texture* Raycaster::getFloorGroundTexture()
{
    return &m_floorGroundTexture;
}

void Raycaster::computeWallSections()
{
    int i = 0;
    std::vector<Ray> rays = getRays();
    sf::RectangleShape wallSection;
    float wallHeight = 83;
    sf::Texture* texture = ResourceManager::getInstance().getTexture("wall");
    sf::IntRect textureRect;
    m_floorGroundTexture.loadFromImage(m_floorGroundImage);
    
    m_wallSections.clear();
    for (auto& ray : rays) {
        wallSection.setFillColor(darkenColor(sf::Color::White, (300, 300.0 / ray.length)));
        if (ray.side == 1)
            wallSection.setFillColor(darkenColor(wallSection.getFillColor(), 0.5f));
        wallSection.setSize(sf::Vector2f(1, (RESY / ray.length) * wallHeight));
        wallSection.setOrigin(0.5, wallSection.getGlobalBounds().height * 0.5f);
        wallSection.setScale(1, -1);
        wallSection.setPosition(i, RESY / 2.0);

        textureRect = sf::IntRect(getStepX(texture, ray), 0, 1, texture->getSize().y);
        
        wallSection.setTexture(texture);
        wallSection.setTextureRect(textureRect);

        m_wallSections.push_back(wallSection);

        i++;
    }
}

std::vector<sf::RectangleShape>& Raycaster::getWallSections()
{
    return m_wallSections;
}