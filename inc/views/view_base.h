#pragma once

#include "../models/model_base.h"

namespace SDL_GUI {
class ViewBase {
public:
    /**
     * Destructor
     */
    virtual ~ViewBase() = default;

    /**
     * initialise view. Has to be overridden by implementation
     * @returns true if initialisation succeeded. Otherwise false.
     */
    virtual bool init() = 0;
    /**
     * deinitialise view. Has to be overridden by implementation
     */
    virtual void deinit() = 0;
    /**
     * update view. This gets called every iteration of the Applications main loop. Has to be overridden by implementaiton.
     */
    virtual void update() = 0;
    /**
     * render view. This gets called every iteration of the Applications main loop. Has to be overridden by implementation.
     */
    virtual void render() = 0;
    /**
     * TODO: what is this?
     */
    virtual void frame_init() {};

protected:
    /**
     * Constructor
     */
    ViewBase() = default;

    /**
     * pointe rto the coupled model.
     */
    ModelBase *_model = nullptr;
};
}
