#pragma once

#include <SDL2/SDL.h>

#include <gui/attributable.h>
#include <gui/position.h>
#include <gui/positionable.h>
#include <gui/style.h>
#include <util/tree.h>

class Drawable : public Positionable, public Attributable, public Tree<Drawable> {
protected:
    Style *_current_style = &this->_default_style;

    /**
     * default Contructor
     */
    Drawable() = default;

    /**
     * Constructor
     *
     * @param position
     *   local position inside parent drawable
     */
    Drawable(Position position) : Positionable(position) {}

    /**
     * draw this Object. gets called by render()
     *
     * @param renderer
     *   renderer to draw on
     * @param position
     *   global offset in window
     */
    virtual void draw(SDL_Renderer *renderer, Position position) const = 0;

    virtual void hook_set_current_style(Style *style);
public:
    Style _default_style;
    Style _hover_style;

    /**
     * change the style to use for rendering
     *
     * @param style
     *   style to use
     */
    void set_current_style(Style *style);

    /**
     * draw this and all of its childs
     *
     * @param renderer
     *   renderer to draw on
     * @param parent_position
     *   global offset of parent
     */
    void render(SDL_Renderer *renderer, Position parent_position = {0,0}) const;

    /**
     * Find first Child that holds a given attribute
     *
     * @param attribute
     *   attribute to search for
     * @returns
     *   first found Drawable that holds the given attribute. nullptr if no such object exists.
     */
    Drawable *find_first_with_attribute(std::string attribute);

    /**
     * Find all Childs that hold a given attribute
     *
     * @param attribute
     *   attribute to search for
     * @returns
     *   List of Objects that hold the given attribute.
     */
    std::vector<Drawable *> find_all_with_attribute(std::string attribute);

    /**
     * check if Position is inside this
     *
     * @param position
     *   Position to check for
     * @returns
     *   True if position is inside. False otherwise.
     */
    virtual bool is_inside(Position position) const;

    /**
     * add a child
     *
     * @param child
     *   child to add
     */
    void add_child(Drawable *child);

};
