#include "singlelayer.h"
#include <geologic-core/utils/customToolbox.h>
#include <data/data.h>
#include <layercheckbox.h>

SingleLayer::~SingleLayer() {
}

/*!
 * \brief SingleLayer::draw
 * \see Layer::draw()
 */
void SingleLayer::draw() {
    if (visible && _data != nullptr) {
        _data->draw();
    }
}

/*!
 * \brief SingleLayer::draw
 * \param t
 * \see Layer::draw(t)
 */
void SingleLayer::draw(unsigned long long t) {
    customToolbox::start("Layer::draw(t)");
    if (visible && _data != nullptr)
        _data->draw(t);
    customToolbox::end("Layer::draw(t)");
}

/*!
 * \brief SingleLayer::drawCheckbox draw the layer checkbox
 * \param tab the tab widget in which to display the checkboxes
 * \param layout the layout in which to add the checkboc (layout is in tab)
 */
void SingleLayer::drawCheckbox(QWidget *tab, QVBoxLayout *layout) {
    if (tab != nullptr && layout != nullptr) {
        _my_checkbox = new LayerCheckBox(this, QObject::tr(_data->getName().c_str()), tab);
        layout->addWidget(_my_checkbox);
    }
}

