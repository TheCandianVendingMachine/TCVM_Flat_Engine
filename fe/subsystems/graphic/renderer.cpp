#include "renderer.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <fstream>

#include "../memory/memoryManager.hpp"

void fe::renderer::startUp()
    {
        m_renderWindowMarker = fe::memoryManager::get().getStackAllocater().getMarker();
		m_renderWindow = new(fe::memoryManager::get().getStackAllocater().alloc(sizeof(sf::RenderWindow))) sf::RenderWindow;
        m_windowSettings = new(fe::memoryManager::get().getStackAllocater().alloc(sizeof(fe::serializerID))) fe::serializerID;

        m_settings.m_windowSize.x = 1280;
        m_settings.m_windowSize.y = 720;

        m_settings.m_borderless = false;
        m_settings.m_fullscreen = false;

        m_settings.m_fps = 0;
    }

void fe::renderer::shutDown()
    {
        if (m_renderWindow)
            {
                m_renderWindow->~RenderWindow();
                m_renderWindow = nullptr;
            }

        if (m_windowSettings)
            {
                m_windowSettings->~serializerID();
                m_windowSettings = nullptr;
            }

        FE_FREE_STACK("RendererDealloc", m_renderWindowMarker);
    }

void fe::renderer::save() const
    {
        m_windowSettings->writeObject("settings", m_settings);

        std::ofstream out("window.cfg");
        m_windowSettings->outData(out);
        out.close();
    }

void fe::renderer::load()
    {
        std::ifstream in("window.cfg");
        m_windowSettings->readData(in);
        in.close();

        m_windowSettings->readObject("settings", m_settings);
        sf::Uint32 settingFlags = sf::Style::Close;
        sf::VideoMode mode(m_settings.m_windowSize.x, m_settings.m_windowSize.y);

        if (m_settings.m_borderless && m_settings.m_fullscreen)
            {
                settingFlags = sf::Style::None | sf::Style::Fullscreen;
                mode = sf::VideoMode::getDesktopMode();
            }
        else if (m_settings.m_borderless)
            {
                settingFlags = sf::Style::None;
            }
        else if (m_settings.m_fullscreen)
            {
                settingFlags = sf::Style::Fullscreen;
                mode = sf::VideoMode::getDesktopMode();
            }

        m_renderWindow->create(mode, "Flat Engine", settingFlags);
        m_renderWindow->setFramerateLimit(m_settings.m_fps);
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

void fe::renderer::settings::serialize(fe::serializerID &serializer) const
    {
        serializer.write("windowSizeX", m_windowSize.x);
        serializer.write("windowSizeY", m_windowSize.y);
        serializer.write("fpsCap", m_fps);
        serializer.write("fullscreen", m_fullscreen);
        serializer.write("borderless", m_borderless);
    }

void fe::renderer::settings::deserialize(fe::serializerID &serializer)
    {
        m_windowSize.x =    serializer.read<unsigned int>("windowSizeX");
        m_windowSize.y =    serializer.read<unsigned int>("windowSizeY");
        m_fps =             serializer.read<unsigned int>("fpsCap");
        m_fullscreen =      serializer.read<bool>("fullscreen");
        m_borderless =      serializer.read<bool>("borderless");
    }
