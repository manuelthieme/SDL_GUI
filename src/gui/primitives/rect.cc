#include <gui/primitives/rect.h>

#include <SDL2/SDL2_gfxPrimitives.h>

using namespace SDL_GUI;

Rect::Rect(std::string type, Position position, unsigned width, unsigned height)
    : Drawable(type, position) {
    this->_width = width;
    this->_height = height;
}

Drawable *Rect::clone() const {
    return new Rect(*this);
}

void Rect::draw(SDL_Renderer *renderer, Position position) const {

    if (this->_style._has_background) {
        const RGB &c = this->_style._color;
        this->_style._color.activate(renderer);
        boxRGBA(renderer, position._x, position._y,
                position._x + this->_width, position._y + this->_height,
                c._r, c._g, c._b, c._a);
    }
}
