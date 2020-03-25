#include <gui/primitives/texture.h>

#include <SDL2/SDL_image.h>

using namespace SDL_GUI;
std::map<std::string, SDL_Texture *> Texture::_textures;

Texture::Texture(std::string path, SDL_Renderer *renderer) : Drawable("Texture"), _path(path) {
    if (!Texture::_textures.contains(path)) {
        Texture::_textures.insert({path, IMG_LoadTexture(renderer, this->_path.c_str())});
    }
    this->_texture = Texture::_textures[path];
}

void Texture::draw(SDL_Renderer *renderer, Position position) const {
    SDL_Rect rect = {
        position._x,
        position._y,
        static_cast<int>(this->_width),
        static_cast<int>(this->_height),
    };
    SDL_RenderCopy(renderer, this->_texture, NULL, &rect);
}
