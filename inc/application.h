#pragma once

#include <string>
#include <tuple>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "controllers/controller_base.h"
#include "models/model_base.h"
#include "plugins/plugin_base.h"
#include "views/view_base.h"

#include "gui/primitives/texture.h"

namespace SDL_GUI {

class ApplicationBase;

/**
 * This function is used to recursively instantiate all the Plugins. This special case has an empty
 * template list and results in an empty tuple. This is the last recursion.
 * @tparam Ts Plugins to instantiate
 * @param application The application
 * @param argc The programs argc
 * @param argv[] The programs argv
 * @return tuple with all instantiated plugins
 */
template <typename ... Ts>
typename std::enable_if<(sizeof...(Ts) == 0), std::tuple<Ts...>>::type
create_plugins(ApplicationBase *application, int argc, char *argv[]) {
    (void)application;
    (void)argc;
    (void)argv;
    return std::make_tuple();
}

/**
 * This function is used to recursively instantiate all the Plugins. The first given template
 * parameter gets instantiated with access to all the template parameters after it.
 * @tparam T type of first Plugin in plugin List
 * @tparam Ts remaining plugins to instantiate
 * @param application The application
 * @param argc The programs argc
 * @param argv[] The programs argv
 * @return tuple with all instantiated plugins
 */
template <typename T, typename ... Ts>
std::tuple<T, Ts...> create_plugins(ApplicationBase *application, int argc, char *argv[]) {
    std::tuple<Ts...> previous = create_plugins<Ts...>(application, argc, argv);
    T current = T();
    current.init(application, previous, argc, argv);
    return std::tuple_cat(std::make_tuple(current), previous);
}

/** Abstract class for Application Objects */
class ApplicationBase {
protected:
    std::string _application_title = "";            /**< title string of the Application */
    std::vector<ModelBase *> _model_list;           /**< list of Models */
    std::vector<ViewBase *> _view_list;             /**< list of views */
    std::vector<ControllerBase *> _controller_list; /**< list of controllers */
    SDL_Window *_window = nullptr;                  /**< window to render in */
    SDL_Renderer *_renderer = nullptr;              /**< renderer to render on */
    unsigned _window_width;                         /**< width of window */
    unsigned _window_height;                        /**< height of window */
    unsigned _target_fps = 60;                      /**< number of targeted frames per second */
    unsigned _current_fps = 0;                      /**< number of frames in the last second */
    unsigned _target_tps = 60;                      /**< number of targeted ticks per second */
    unsigned _current_tps = 0;                      /**< number of ticks in the last second */


    /**
     * initialise everything concerning SDL
     */
    static void init_SDL();

    /**
     * initialise the window
     * @param title window title
     * @param width window width
     * @param height window height
     * @return pointer to the initialised window
     */
    static SDL_Window *init_window(std::string title, unsigned width, unsigned height);

    /**
     * initialise the renderer
     * @param window window to initialise renderer for
     * @return pointer to initialised renderer
     */
    static SDL_Renderer *init_renderer(SDL_Window *window);

    /**
     * do further initialisation in derived classes
     */
    virtual void init_this() {}

    /**
     * deinitialise application
     * Currently this function does nothing
     */
    void deinit() {}

    /**
     * trigger update on all the existing controllers
     */
    void update_controllers();

    /**
     * trigger update on all the existing views that have a related model
     */
    void update_views();

    /**
     * trigger rendering on all the existing views that have a related model
     */
    void render_views();

    /**
     * Constructor
     * @param application_title title string for the application
     * @param window_width initial width of window
     * @param window_height initial height of window
     */
    ApplicationBase(std::string application_title, unsigned window_width = 1920,
                    unsigned window_height = 1080);

    /** Destructor */
    ~ApplicationBase();

public:

    /** flag that determines whether this application should continue running */
    bool _is_running = true;

    /**
     * run application
     * This starts the main loop that updates all the controllers and views and renders.
     */
    void run();

    /**
     * Getter for _window
     * @return this->_window
     */
    SDL_Window *window();

    /**
     * Getter for _renderer
     * @return this->_renderer
     */
    SDL_Renderer *renderer();

    /**
     * Getter for _window_width
     * @return this->_window_width
     */
    unsigned window_width() const;

    /**
     * Getter for _window_height
     * @return this->_window_height
     */
    unsigned window_height() const;

    unsigned target_fps() const;
    unsigned current_fps() const;
    unsigned target_tps() const;
    unsigned current_tps() const;

    /**
     * Add Model to applications model list
     * @param model model to add
     */
    void add_model(ModelBase *model);

    /**
     * Add controller to applications model list
     * @param controller controller to add
     */
    void add_controller(ControllerBase *controller);

    /**
     * Add view to applications model list
     * @param view view to add
     */
    void add_view(ViewBase *view);
};

/**
 * Actual Application class
 * @tparam Ts list of Plugins the application uses
 */
template<typename ... Ts>
class Application : public ApplicationBase {
protected:
    std::tuple<Ts...> _plugins;     /**< list of Plugins */
public:
    /**
     * Constructor
     * @param application_title itle string for the application
     * @param argc the programs argc
     * @param argv[] the programs argv
     * @param window_width initial window width
     * @param window_height initial window height
     */
    Application(std::string application_title, int argc, char *argv[], unsigned window_width = 1920,
                unsigned window_height = 1080)
        : ApplicationBase(application_title, window_width, window_height) {
            this->_plugins = create_plugins<Ts...>(this, argc, argv);
        }
};
}
