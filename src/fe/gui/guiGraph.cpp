#include "fe/gui/guiGraph.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/gui/dialog.hpp"
#include "fe/gui/control.hpp"

fe::transformable *fe::gui::guiGraph::getNodeTransform(fe::priv::node *node)
    {
        return static_cast<fe::gui::dialog*>(node->m_userData);
    }

fe::transformable *fe::gui::guiGraph::getNodeTempTransform(fe::priv::node *node)
    {
        return &static_cast<fe::gui::dialog*>(node->m_userData)->getDrawMatrix();
    }

const sf::Texture *fe::gui::guiGraph::getRenderTexture() const
    {
        return nullptr;
    }

bool fe::gui::guiGraph::renderNode(fe::priv::node *node) const
    {
        return true;
    }

void fe::gui::guiGraph::drawNode(fe::priv::node *node)
    {
        fe::gui::dialog *dialog = static_cast<fe::gui::dialog*>(node->m_userData);
        dialog->draw(m_batch);
    }

void fe::gui::guiGraph::drawToScreen(sf::RenderTarget &target, sf::RenderStates states)
    {
        m_batch.draw(target, states);
    }
