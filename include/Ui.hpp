#include <SFML/Graphics.hpp>

class UI
{
public:
    UI();
    void addText(const std::string &name, const sf::Text &text);
    void addSprite(const std::string &name, const sf::Sprite &sprite);
    void addRectangle(const std::string &name, const sf::RectangleShape &rectangle);
    sf::Text& getText(const std::string &name);
    sf::Sprite& getSprite(const std::string &name);
    sf::RectangleShape& getRectangle(const std::string &name);
    void removeText(const std::string &name);
    void removeSprite(const std::string &name);
    void removeRectangle(const std::string &name);
    void drawText(sf::RenderTarget &target) const;
    void drawSprite(sf::RenderTarget &target) const;
    void drawRectangle(sf::RenderTarget &target) const;

private:
    std::map<std::string, sf::Text> m_textElements;
    std::map<std::string, sf::Sprite> m_spriteElements;
    std::map<std::string, sf::RectangleShape> m_rectangleElements;
};
