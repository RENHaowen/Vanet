#ifndef COMPOSITELAYER_H
#define COMPOSITELAYER_H

#include "layer.h"
#include <vector>
#include <QString>
#include <QCheckBox>

class CompositeLayer : public Layer {
    std::vector<Layer *> _children;
    QString _title;

public:
    CompositeLayer(Data *d = nullptr, CompositeLayer *parent = nullptr);
    virtual ~CompositeLayer();
    void addChild(Layer *child);
    void removeChild(Layer *child);
    virtual void draw();
    virtual void draw(unsigned long long t);
    void setTitle(const char *title);
    const std::vector<Layer *> &getChildren()const;
    virtual void drawCheckbox(QWidget *tab, QVBoxLayout *layout);
    void childNotification();
    virtual void setVisible();
    virtual void setHidden();
};

#endif // COMPOSITELAYER_H
