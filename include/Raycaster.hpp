#include <SFML/Graphics.hpp>
#include <math.h>
#include "Grid.hpp" 
#include "defines.hpp"

// The Raycaster class handles the casting of rays.
class Raycaster {
public:
    // The Ray struct represents a ray.
    struct Ray {
        sf::Vector2f origin;
        sf::Vector2f direction;
        sf::Color color;
        sf::Vertex vertices[2];
        double wallX;
        bool side;
        float length;
    };

    // Constructs a raycaster with the given grid.
    Raycaster(Grid& grid);
    ~Raycaster();

    // Casts a ray from the given position at the given angle.
    Ray castRay(sf::Vector2f position, float angle, float refAngle, int x);
    std::vector<Ray> getRays();
    void setRays(std::vector<Ray>);
    std::vector<Ray> castRays(sf::Vector2f position,
    float angle, int numRays, float angleRange);
    void computeWallSections();
    void computeFloorGroundVertices();
    std::vector<sf::RectangleShape>& getWallSections();
    sf::Texture* getFloorGroundTexture();

private:
    double getStepX(sf::Texture* texture, Ray ray);
    // The grid to cast rays against.
    Grid& m_grid;
    std::vector<Ray> m_rays;
    std::vector<sf::RectangleShape> m_wallSections;
    sf::Image m_floorGroundImage;
    sf::Texture m_floorGroundTexture;
};
