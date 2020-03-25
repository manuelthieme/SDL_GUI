#pragma once

#include "rgb.h"

namespace SDL_GUI {
class Style {
public:
    RGB _color = RGB(); /**< background color */
    RGB _border_color = RGB("black"); /**< border color */
    unsigned _border_width = 1; /**< border width in px*/
    bool _has_background = false; /**< background flag */
    bool _has_border = false; /**< border flag */
    bool _overflow = false; /**< overflow flag */
    bool _hidden = false; /**< hidden flag */

    Style() = default;
};
}
