#include "Scene/layer.h"

#include <data/data.h>
#include <iostream>
#include <geologic-core/utils/customToolbox.h>
#include <Scene/compositelayer.h>

using namespace std;

Layer::Layer(Data *d, CompositeLayer *parent): _data(unique_ptr<Data>(d)), visible(true), _parent(parent) {
    customToolbox::start("Layer::Layer");
    customToolbox::end("Layer::Layer");
}

Layer::~Layer() {
}

void Layer::setVisible() {
    customToolbox::start("Layer::setVisible");
    visible = true;
    notifyParent();
    customToolbox::end("Layer::setVisible");
}

void Layer::setHidden() {
    customToolbox::start("Layer::setHidden");
    visible = false;
    notifyParent();
    customToolbox::end("Layer::setHidden");
}

bool Layer::isVisible() {
    return visible;
}

Data* Layer::getData() {
    return _data.get();
}

void Layer::setParent(CompositeLayer *parent) {
    _parent = parent;
}

/*!
 * \brief Layer::notifyParent notifies parent (if there is one) of the layer changed state.
 */
void Layer::notifyParent() {
    if (_parent != nullptr) {
        _parent->childNotification();
    }
}
