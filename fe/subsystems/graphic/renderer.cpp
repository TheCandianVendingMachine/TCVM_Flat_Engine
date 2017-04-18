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
        m_renderWindow->setFramerateLimit(120);
    }

void fe::renderer::draw(const sf::Drawable &draw)
    {
        m_renderWindow->draw(draw);
    }

sf::RenderWindow &fe::renderer::getRenderWindow()
    {
        return *m_renderWindow;
    }
	
fe::Vector2d fe::renderer::getWindowSize()
	{
		return fe::Vector2d(m_renderWindow->getSize().x, m_renderWindow->getSize().y);
	}
