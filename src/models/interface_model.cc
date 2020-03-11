#include <models/interface_model.h>

#include <iostream>
#include <string>

using namespace SDL_GUI;

InterfaceModel::InterfaceModel() {
    this->init();
}

InterfaceModel::~InterfaceModel() {
    delete this->_null_drawable;
    delete this->_null_drawable_node;
}

void InterfaceModel::init() {
    this->_null_drawable = new NullDrawable();
    this->_null_drawable_node = new TreeNode<Drawable>(this->_null_drawable);
    /* init font */
    this->_font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf", 12);
    if (!this->_font) {
        std::cerr << "TTF_Error: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}


TTF_Font *InterfaceModel::font() const {
    return this->_font;
}

Tree<Drawable> *InterfaceModel::drawable_tree() {
    return this->_drawable_tree;
}

const Tree<Drawable> *InterfaceModel::drawable_tree() const {
    return this->_drawable_tree;
}

void InterfaceModel::set_drawable_tree(Tree<Drawable> *tree) {
    this->_drawable_tree = tree;
}

std::vector<Drawable *> InterfaceModel::find_drawables(std::string attribute) {
    std::vector<TreeNode<Drawable> *> treenodes = this->_drawable_tree->filter([attribute](Drawable *d){return d->has_attribute(attribute);});
    std::vector<Drawable *> drawables;
    for (TreeNode<Drawable> *node: treenodes) {
        drawables.push_back(node->node());
    }
    return drawables;
}

std::vector<const Drawable *> InterfaceModel::find_drawables(std::string attribute) const {
    std::vector<TreeNode<Drawable> *> treenodes = this->_drawable_tree->filter([attribute](Drawable *d){return d->has_attribute(attribute);});
    std::vector<const Drawable *> drawables;
    for (TreeNode<Drawable> *node: treenodes) {
        drawables.push_back(node->node());
    }
    return drawables;
}

Drawable * InterfaceModel::find_first_drawable(std::string attribute) {
    std::vector<TreeNode<Drawable> *> treenodes = this->_drawable_tree->filter([attribute](Drawable *d){return d->has_attribute(attribute);});
    if (treenodes.size() < 1) {
        return nullptr;
    }
    return treenodes[0]->node();
}

const Drawable * InterfaceModel::find_first_drawable(std::string attribute) const {
    std::vector<TreeNode<Drawable> *> treenodes = this->_drawable_tree->filter([attribute](Drawable *d){return d->has_attribute(attribute);});
    if (treenodes.size() < 1) {
        return nullptr;
    }
    return treenodes[0]->node();
}

std::vector<TreeNode<Drawable> *> InterfaceModel::find_tree_nodes(std::string attribute) {
    return this->_drawable_tree->filter([attribute](Drawable *d){return d->has_attribute(attribute);});
}

std::vector<const TreeNode<Drawable> *> InterfaceModel::find_tree_nodes(std::string attribute) const {
    std::vector<TreeNode<Drawable> *> tree_nodes = this->_drawable_tree->filter([attribute](Drawable *d){return d->has_attribute(attribute);});
    return std::vector<const TreeNode<Drawable> *>(tree_nodes.begin(), tree_nodes.end());
}

TreeNode<Drawable> * InterfaceModel::find_first_tree_node(std::string attribute) {
    std::vector<TreeNode<Drawable> *> tree_nodes = this->_drawable_tree->filter([attribute](Drawable *d){return d->has_attribute(attribute);});
    if (tree_nodes.size() < 1) {
        return this->_null_drawable_node;
    }
    return tree_nodes[0];
}

const TreeNode<Drawable> * InterfaceModel::find_first_tree_node(std::string attribute) const {
    std::vector<TreeNode<Drawable> *> tree_nodes = this->_drawable_tree->filter([attribute](Drawable *d){return d->has_attribute(attribute);});
    if (tree_nodes.size() < 1) {
        return this->_null_drawable_node;
    }
    return tree_nodes[0];

}
