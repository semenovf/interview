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

    Q_SIGNAL void emitDiskSelected (int diskIndex);
    Q_SIGNAL void emitVolumeSelected (int diskIndex, int volumeIndex);
    Q_SLOT void onDiskSelected (int diskIndex);
    Q_SLOT void onVolumeSelected (int diskIndex, int volumeIndex);

private:
    //Q_SLOT onDiskSelected ();


};


