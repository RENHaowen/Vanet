#ifndef LAYERCHECKBOX_H
#define LAYERCHECKBOX_H

#include <QCheckBox>
#include <Scene/layer.h>

class LayerCheckBox: public QCheckBox {
    Q_OBJECT

    Layer *managed_layer;

public:
    LayerCheckBox() = delete;
    LayerCheckBox(Layer *l, const QString &label, QWidget *parent);
    ~LayerCheckBox() {}

public slots:
    void setState(int state);
};



#endif // LAYERCHECKBOX_H
