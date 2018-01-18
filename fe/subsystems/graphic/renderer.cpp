#include "renderer.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <fstream>

#include "../memory/memoryManager.hpp"

void fe::renderer::startUp()
    {
		m_renderWindow = new(fe::memoryManager::get().getStackAllocater().alloc(sizeof(sf::RenderWindow))) sf::RenderWindow;
        m_windowSettings = new(fe::memoryManager::get().getStackAllocater().alloc(sizeof(fe::serializerID))) fe::serializerID;

        m_windowSize.x = 1280;
        m_windowSize.y = 720;

        m_borderless = false;
        m_fullscreen = false;

        m_fps = 0;
    }

void fe::renderer::shutDown()
    {
        if (m_renderWindow)
            {
                delete m_renderWindow;
                m_renderWindow = nullptr;
            }

        if (m_windowSettings)
            {
                delete m_windowSettings;
                m_windowSettings = nullptr;
            }
    }

void fe::renderer::save() const
    {
        serialize(*m_windowSettings);

        std::ofstream out("window.cfg");
        m_windowSettings->outData(out);
        out.close();
    }

void fe::renderer::load()
    {
        std::ifstream in("window.cfg");
        m_windowSettings->readData(in);
        in.close();

        deserialize(*m_windowSettings);
        sf::Uint32 settingFlags = sf::Style::Close;
        sf::VideoMode mode(m_windowSize.x, m_windowSize.y);

        if (m_borderless && m_fullscreen) 
            {
                settingFlags = sf::Style::None | sf::Style::Fullscreen;
                mode = sf::VideoMode::getDesktopMode();
            }
        else if (m_borderless)
            {
                settingFlags = sf::Style::None;
            }
        else if (m_fullscreen)
            {
                settingFlags = sf::Style::Fullscreen;
                mode = sf::VideoMode::getDesktopMode();
            }

        m_renderWindow->create(mode, "Flat Engine", settingFlags);
        m_renderWindow->setFramerateLimit(m_fps);
    }

void fe::renderer::draw(const sf::Drawable &draw)
    {
        m_renderWindow->draw(draw);
    }

sf::RenderWindow &fe::renderer::getRenderWindow() const
    {
        return *m_renderWindow;
    }
	
fe::Vector2<unsigned int> fe::renderer::getWindowSize()
	{
		return fe::Vector2<unsigned int>(m_renderWindow->getSize());
	}

fe::renderer::~renderer()
    {}
