// radioList.hpp
// A button which allows a single selection to be selected.
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/gui/control.hpp"
#include "fe/math/polygon2.hpp"
#include "fe/gui/text.hpp"
#include "fe/gui/radioButton.hpp"
#include <string>
#include <initializer_list>
#include <vector>

namespace fe
    {
        namespace gui
            {
                class guiGraph;
                class radioList : public fe::gui::control
                    {
                        protected:
                            std::vector<fe::gui::radioButton> m_selections;
                            std::vector<fe::gui::text> m_options;

                            float m_selectionSize;
                            float m_gapBetweenSelections;
                            int m_optionCount;
                            fe::lightVector2d m_selectionPositionIncrement;

                            FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next) final;
                            FLAT_ENGINE_API void drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix) final;

                            FLAT_ENGINE_API void handleWindowEvent(const sf::Event &event);

                        public:
                            // Creates a new checkButton list. Options correspond to text listed alongside check boxes. Left -> Right / Top -> Down
                            FLAT_ENGINE_API radioList(
                                float selectionSize, unsigned int count,
                                float gapBetweenSelections = 5.f, float selectionOutlineWidth = 5.f, float markWidthFromSide = 2.f,
                                const sf::Font *font = nullptr, std::initializer_list<std::string> options = {}, unsigned int charSize = 22, float gapBetweenTextAndOption = 5.f
                            );

                            FLAT_ENGINE_API void updateSelections(float selectionSize, float gapBetweenSelections, float selectionOutlineWidth, float markWidthFromSide, unsigned int charSize, float gapBetweenTextAndBox);

                            FLAT_ENGINE_API void init(fe::gui::guiGraph &graph, int node);

                            FLAT_ENGINE_API fe::gui::radioButton &getSelection(unsigned int index);

                            // Returns all check box inputs from Left -> Right / Top -> Down
                            FLAT_ENGINE_API void getSelected(std::vector<bool> &selected);
                            FLAT_ENGINE_API std::vector<bool> getSelected();

                            using fe::gui::control::handleEvent;
                            FLAT_ENGINE_API void handleEvent(const fe::gameEvent &event) override;

							FLAT_ENGINE_API fe::lightVector2d getSize() const;
                    };
            }
    }
