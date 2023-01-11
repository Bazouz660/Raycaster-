#include "Ui.hpp"
#include "ResourceManager.hpp"
#include "Prototypes.hpp"

UI::UI()
{
    addText("fpsCounter", createText("FPS", *ResourceManager::getInstance().getFont("debugFont"), 35));
}

void UI::addText(const std::string &name, const sf::Text &text)
{
    m_textElements[name] = text;
}

void UI::addSprite(const std::string &name, const sf::Sprite &sprite)
{
    m_spriteElements[name] = sprite;
}

void UI::addRectangle(const std::string &name, const sf::RectangleShape &rectangle)
{
    m_rectangleElements[name] = rectangle;
}

sf::Text& UI::getText(const std::string &name)
{
    return m_textElements.at(name);
}

sf::Sprite& UI::getSprite(const std::string &name)
{
    return m_spriteElements.at(name);
}

sf::RectangleShape& UI::getRectangle(const std::string &name)
{
    return m_rectangleElements.at(name);
}

void UI::removeText(const std::string &name)
{
    m_textElements.erase(name);
}

void UI::removeSprite(const std::string &name)
{
    m_spriteElements.erase(name);
}

void UI::removeRectangle(const std::string &name)
{
    m_rectangleElements.erase(name);
}

void UI::drawText(sf::RenderTarget &target) const
{
    for (auto &[name, text] : m_textElements)
        target.draw(text);
}

void UI::drawSprite(sf::RenderTarget &target) const
{
    for (auto &[name, sprite] : m_spriteElements)
        target.draw(sprite);
}

void UI::drawRectangle(sf::RenderTarget &target) const
{
    for (auto &[name, rectangle] : m_rectangleElements)
        target.draw(rectangle);
}
