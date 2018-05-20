#include "fe/gui/guiGraph.hpp"
#include "fe/debug/profiler.hpp"
#include "fe/gui/dialog.hpp"
#include "fe/gui/control.hpp"
#include "fe/gui/dialogProxy.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

int fe::gui::guiGraph::getNodeFromObject(void *object)
    {
        return -1;
    }

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

void fe::gui::guiGraph::drawNode(fe::priv::node *node, sf::RenderTarget &target)
    {
        fe::gui::dialog *dialog = static_cast<fe::gui::dialog*>(node->m_userData);
        dialog->draw(target);
        dialog->draw(target);
    }

void *fe::gui::guiGraph::createZOrderProxy()
    {
        return new imp::dialogProxy();
    }

void fe::gui::guiGraph::drawToScreen(sf::RenderTarget &target, sf::RenderStates states)
    {
    }

void fe::gui::guiGraph::onObjectAdd(void *object, int node)
    {
        static_cast<fe::gui::dialog*>(object)->init(*this, node);
    }

void fe::gui::guiGraph::onObjectRemove(void *object, int node)
    {
        static_cast<fe::gui::dialog*>(object)->deinit(*this, node);
    }

fe::gui::guiGraph::guiGraph() :
    renderGraph(2500)
    {
    }

void fe::gui::guiGraph::preDraw()
    {
    }

