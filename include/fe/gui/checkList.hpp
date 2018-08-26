// checkList.hpp
// A button which allows multiple selections to be selected.
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/gui/control.hpp"
#include "fe/math/polygon2.hpp"
#include "fe/gui/text.hpp"
#include "fe/gui/checkBox.hpp"
#include <string>
#include <initializer_list>
#include <vector>

namespace fe
    {
        namespace gui
            {
                class guiGraph;
                class checkList : public fe::gui::control
                    {
                        protected:
                            std::vector<fe::gui::checkBox> m_checkBoxes;
                            std::vector<fe::gui::text> m_options;

                            float m_boxSize;
                            float m_gapBetweenBoxes;
                            int m_optionCount;
                            fe::lightVector2d m_boxPositionIncrement;

                            FLAT_ENGINE_API void onStateChange(dialogStates previous, dialogStates next) final;
                            FLAT_ENGINE_API void drawDialogElements(sf::RenderTarget &target, const fe::matrix3d &drawMatrix) final;

                            FLAT_ENGINE_API void handleWindowEvent(const sf::Event &event);

                        public:
                            // Creates a new checkButton list. Options correspond to text listed alongside check boxes. Left -> Right / Top -> Down
                            FLAT_ENGINE_API checkList(
                                float checkBoxSize, unsigned int count,
                                float gapBetweenBoxes = 5.f, float boxOutlineWidth = 5.f, float checkMarkWidthFromSide = 2.f,
                                const sf::Font *font = nullptr, std::initializer_list<std::string> options = {}, unsigned int charSize = 22, float gapBetweenTextAndOption = 5.f
                            );

                            FLAT_ENGINE_API void updateBoxes(float checkBoxSize, float gapBetweenBoxes, float boxOutlineWidth, float checkMarkWidthFromSide, unsigned int charSize, float gapBetweenTextAndBox);

                            FLAT_ENGINE_API void init(fe::gui::guiGraph &graph, int node);

                            FLAT_ENGINE_API fe::gui::checkBox &getCheckbox(unsigned int index);

                            // Returns all check box inputs from Left -> Right / Top -> Down
                            FLAT_ENGINE_API void getSelected(std::vector<bool> &selected) const;
                            FLAT_ENGINE_API std::vector<bool> getSelected() const;

                            FLAT_ENGINE_API bool anySelected() const;

                            FLAT_ENGINE_API fe::lightVector2d getSize() const;

                    };
            }
    }
