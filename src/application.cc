#include <application.h>

#include <iostream>
#include <chrono>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <controllers/input_controller.h>


using namespace SDL_GUI;


inline std::chrono::high_resolution_clock::duration t_sys() {
    return std::chrono::high_resolution_clock::now().time_since_epoch();
}

void ApplicationBase::init_SDL() {
    if (0 != SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if (0 != TTF_Init()) {
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

SDL_Window *ApplicationBase::init_window(std::string title, unsigned width, unsigned height) {
    /* init window at {0,0} */
    SDL_Window *window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "unable to create window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    return window;
}

SDL_Renderer *ApplicationBase::init_renderer(SDL_Window *window) {
    /* init renderer  for first found rendering device (-1) */
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                                            SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "unable to create renderer: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return renderer;
}

ApplicationBase::ApplicationBase(std::string application_title, unsigned window_width,
                                 unsigned window_height)
    : _application_title(application_title), _window_width(window_width),
      _window_height(window_height) {
    ApplicationBase::init_SDL();
    this->_window = ApplicationBase::init_window(application_title, window_width, window_height);
    this->_renderer = ApplicationBase::init_renderer(this->_window);
}

void ApplicationBase::run() {
    using duration_t = std::chrono::high_resolution_clock::duration;
    unsigned logic_count = 0;
    unsigned render_count = 0;

    const duration_t dt = std::chrono::microseconds(1000000 / this->_fps);
    duration_t time_app = t_sys();
    duration_t time_logic = time_app - dt;
    while (this->_is_running) {
        while(time_app - time_logic >= dt) {
            logic_count++;
            time_logic += dt;
            read_sdl_events();
            this->update_controllers();
            this->update_views();
            clear_sdl_events();
        }

        time_app = t_sys();
        /* render */
        render_count ++;
        this->render_views();
    }
    this->deinit();
}

void ApplicationBase::update_controllers() {
    for (ControllerBase *controller: this->_controller_list) {
        controller->update();
    }
}

void ApplicationBase::update_views() {
    for (ViewBase *view: this->_view_list) {
        view->update();
    }
}

void ApplicationBase::render_views() {
    for (ViewBase *view: this->_view_list) {
        view->render();
    }
}
