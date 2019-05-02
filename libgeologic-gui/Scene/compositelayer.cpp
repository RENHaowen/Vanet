#include "compositelayer.h"
#include <data/data.h>
#include <algorithm>
#include <layercheckbox.h>

/*!
 * \brief CompositeLayer::CompositeLayer CompositeLayer constructor
 * \param d a pointer to the layer data, defaults to nullptr
 * \param parent a pointer to its parent, defaults to nullptr (top level layer)
 */
CompositeLayer::CompositeLayer(Data *d, CompositeLayer *parent): Layer(d, parent) {}

CompositeLayer::~CompositeLayer() {}

/*!
 * \brief CompositeLayer::addChild adds a child to the vector of children layers
 * \param child the child to add (non owning pointer)
 */
void CompositeLayer::addChild(Layer *child) {
    _children.push_back(child);
    child->setParent(this);
}

/*!
 * \brief CompositeLayer::removeChild removes a child from children vector
 * \param child defined by its pointer
 */
void CompositeLayer::removeChild(Layer *child) {
    auto it = find(_children.begin(), _children.end(), child);
    if (it != _children.end())
        _children.erase(it);
}

/*!
 * \brief CompositeLayer::draw
 * \see Layer::draw()
 */
void CompositeLayer::draw() {
    if (visible) {
        if (_data.get() != nullptr)
            _data->draw();
        for (Layer *elt: _children)
            elt->draw();
    }
}

/*!
 * \brief CompositeLayer::draw
 * \param t
 * \see Layer::draw(t)
 */
void CompositeLayer::draw(unsigned long long t) {
    if (visible) {
        if (_data.get() != nullptr)
            _data->draw(t);
        for (Layer *elt: _children)
            elt->draw(t);
    }
}

/*!
 * \brief CompositeLayer::setTitle sets the display title for this composite layer
 * \param title the title as a C string
 * This method sets the layer name displayed along its checkbox
 */
void CompositeLayer::setTitle(const char *title) {
    _title = QObject::tr(title);
}

const std::vector<Layer *> &CompositeLayer::getChildren() const {
    return _children;
}

/*!
 * \brief CompositeLayer::drawCheckbox draws the layer and its children checkboxes
 * \param tab the tab widget in which to display the checkboxes
 * \param layout the layout in which to add the checkboc (layout is in tab)
 */
void CompositeLayer::drawCheckbox(QWidget *tab, QVBoxLayout *layout) {
    if (tab != nullptr && layout != nullptr) {
        _my_checkbox = new LayerCheckBox(this, _title, tab);
        _my_checkbox->setTristate(true);
        layout->addWidget(_my_checkbox);

        QVBoxLayout *next_layout = new QVBoxLayout();
        next_layout->setContentsMargins(10, 0, 0, 0);
        layout->addLayout(next_layout);
        for (Layer *elt: _children) {
            if (elt != nullptr)
                elt->drawCheckbox(tab, next_layout);
        }
    }
}

/*!
 * \brief CompositeLayer::childNotification is used to update the composite layer checkbox.
 * if no child is enabled, its unchecked, if all children are enabled, it is checked, else it is partially checked.
 */
void CompositeLayer::childNotification() {
    int checked_count = 0;
    for (Layer *elt: _children) {
        if (elt->isVisible())
            ++checked_count;
    }
    if (checked_count == _children.size()) {
        _my_checkbox->setCheckState(Qt::Checked);
    } else if (checked_count == 0) {
        _my_checkbox->setCheckState(Qt::Unchecked);
    } else {
        _my_checkbox->setCheckState(Qt::PartiallyChecked);
    }
}

/*!
 * \brief CompositeLayer::setVisible sets the visibility of the layer and updates its children
 */
void CompositeLayer::setVisible() {
    Layer::setVisible();
    for (Layer *elt: _children) {
        elt->setParent(nullptr);
        elt->getCheckBox()->setChecked(true);
        elt->setParent(this);
    }
}
/*!
 * \brief CompositeLayer::setHidden hides the layer and its children
 */
void CompositeLayer::setHidden() {
    Layer::setHidden();
    for (Layer *elt: _children) {
        elt->setParent(nullptr);
        elt->getCheckBox()->setChecked(false);
        elt->setParent(this);
    }
}
