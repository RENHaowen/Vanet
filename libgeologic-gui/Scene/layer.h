#ifndef LAYER_H
#define LAYER_H

#include <memory>
#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>

class Data;
class CompositeLayer;

/*!
 * \class Layer
 * \brief Manages the graphical layers to display.
 * This class manages a Data graphical components and
 * decides whether to display it or not, based on its
 * visibility parameter.
 * \see Data
 */
class Layer {
protected:
    std::unique_ptr<Data> _data; ///< The Data attached to the Layer
    bool visible; ///< The visibility: if true, Data is displayed, else it is not.
    CompositeLayer *_parent; ///< Used for child layer to forward its changes to parent.
    QCheckBox *_my_checkbox = nullptr;

public:
    /*!
     * \brief Layer constructor.
     * Takes its Data as a parameter
     * \param d the Data to attach to current Layer. Current layer takes ownership of the data.
     */
    Layer(Data *d, CompositeLayer *parent = nullptr);

    /*!
     * \brief Layer destructor.
     * Does not do anything currently.
     */
    virtual ~Layer() = 0;

    /*!
     * \brief Sets visible to true.
     * This is used to direct current Layer to display its Data.
     */
    virtual void setVisible();

    /*!
     * \brief Sets visible to false.
     * This is used to direct current Layer to hide its Data.
     */
    virtual void setHidden();

    /*!
     * \brief Requests Layer visibility.
     * \return The visibility attribute value.
     */
    virtual bool isVisible();

    /*!
     * \brief Draws its Data.
     * Calls Data::draw() if visibility is set to true.
     * Does nothing else.
     */
    virtual void draw() = 0;

    /*!
     * \brief Draws its Data.
     * Calls Data::draw(t) if visibility is set to true.
     * Does nothing else.
     */
    virtual void draw(unsigned long long t) = 0;

    /*!
     * \brief Gets the Data attached to current Layer.
     * \return Layer Data as a pointer.
     */
    virtual Data *getData();

    /*!
     * \brief setParent sets the layer parent
     * \param parent a pointer to the layer parent
     */
    virtual void setParent(CompositeLayer *parent);

    /*!
     * \brief drawCheckbox draws a checkbox into current container
     */
    virtual void drawCheckbox(QWidget *tab, QVBoxLayout *layout) = 0;

    virtual void notifyParent();
    QCheckBox *getCheckBox() {return _my_checkbox;}
};

#endif // LAYER_H
