// checkList.hpp
// A button which allows multiple selections to be selected.
#pragma once
#define FLAT_ENGINE_EXPORT
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

                        public:
                            enum class mode
                                {
                                    VERTICAL,
                                    HORIZONTAL
                                };

                            // Creates a new checkButton list. Options correspond to text listed alongside check boxes. Left -> Right / Top -> Down
                            FLAT_ENGINE_API checkList(float checkBoxSize, unsigned int count, mode boxMode, float gapBetweenBoxes = 5.f, sf::Font *font = nullptr, std::initializer_list<std::string> options = {});

                            FLAT_ENGINE_API void init(fe::gui::guiGraph &graph, int node);

                    };
            }
    }
