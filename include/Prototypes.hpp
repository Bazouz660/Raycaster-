#include "Grid.hpp"

void updateView(sf::RenderWindow* window);

sf::Vector2f gridToScreen(int row, int column, int tileSize);
sf::Vector2i screenToGrid(sf::Vector2f position, int tileSize);

void handleMouseInput(sf::RenderWindow& window);

std::vector<std::string> split(const std::string& str, char delimiter);

Grid loadGridFromFile(const std::string& filename);

sf::Vector2f normalize(const sf::Vector2f& v);

sf::Color SmoothColor(sf::Color c1, sf::Color c2, double prog);
sf::Color darkenColor(sf::Color c1, double prog);

std::string removeExtension(const std::string& filename);

sf::Text createText(const std::string& str, sf::Font& font, int fontSize);

float distFrom(sf::Vector2f p1, sf::Vector2f p2);
double clamp(double min, double max, double value);

float DistanceToPlane(sf::Vector2f pos1, float angle, sf::Vector2f pos2);
