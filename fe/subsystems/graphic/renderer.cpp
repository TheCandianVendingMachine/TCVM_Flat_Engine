#include "renderer.hpp"
#include "../memory/feNew.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

fe::renderer *fe::renderer::m_instance = nullptr;

void fe::renderer::startUp()
    {
        if (!m_instance) 
            {
                m_renderWindow = new sf::RenderWindow;
                m_instance = this;
            }
    }

void fe::renderer::shutDown()
    {
    }

fe::renderer &fe::renderer::get()
    {
        return *m_instance;
    }

void fe::renderer::load()
    {
        m_renderWindow->create(sf::VideoMode(1280, 720), "Flat Engine", sf::Style::Close);
        m_renderWindow->setFramerateLimit(2000);
    }

void fe::renderer::draw(const sf::Drawable &draw)
    {
        m_renderWindow->draw(draw);
    }

sf::RenderWindow &fe::renderer::getRenderWindow()
    {
        return *m_renderWindow;
    }
