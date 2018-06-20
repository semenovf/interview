#pragma once
#include <QWidget>
#include <QList>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class Model;
class GraphicalViewItem;

class GraphicalView : public QWidget
{
    Q_OBJECT

    Model *                    _pmodel;
    QList<GraphicalViewItem *> _items;

public:
    GraphicalView (Model *, QWidget * parent = 0);
};


