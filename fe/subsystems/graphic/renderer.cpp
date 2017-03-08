#include "renderer.hpp"
#include "../memory/feNew.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

void fe::renderer::startUp()
    {
        m_renderWindow = new sf::RenderWindow;
    }

void fe::renderer::shutDown()
    {
    }

void fe::renderer::load()
    {
        m_renderWindow->create(sf::VideoMode(1280, 720), "Flat Engine", sf::Style::Close);
        m_renderWindow->setFramerateLimit(2000);
    }

void fe::renderer::draw(const sf::Drawable &draw)
    {
    }

sf::RenderWindow &fe::renderer::getRenderWindow()
    {
        return *m_renderWindow;
    }
