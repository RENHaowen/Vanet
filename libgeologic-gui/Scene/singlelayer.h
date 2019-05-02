#ifndef SINGLELAYER_H
#define SINGLELAYER_H

#include "layer.h"

class CompositeLayer;

class SingleLayer : public Layer {
public:
    SingleLayer(Data *d, CompositeLayer *parent = nullptr): Layer(d, parent) {}
    virtual ~SingleLayer();
    virtual void draw();
    virtual void draw(unsigned long long t);
    virtual void drawCheckbox(QWidget *tab, QVBoxLayout *layout);
};

#endif // SINGLELAYER_H
