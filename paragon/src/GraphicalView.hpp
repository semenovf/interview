#pragma once
#include <QScrollArea>
#include <QList>

class Model;
class GraphicalViewItem;

class GraphicalView : public QScrollArea
{
    Q_OBJECT

    Model *                    _model;
    QList<GraphicalViewItem *> _items;

public:
    GraphicalView (Model *, QWidget * parent = 0);

    Q_SIGNAL void emitEntitySelected (int diskIndex, int volumeIndex);
    Q_SLOT void onEntitySelected (int diskIndex, int volumeIndex);
};


