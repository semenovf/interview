#pragma once
#include <QScrollArea>
#include <QList>

///////////////////////////////////////////////////////////////////////////////
// GraphicalView
///////////////////////////////////////////////////////////////////////////////
// Графическое представление информации о дисках и томах в системе.
// Представление состоит из элементов, каждый из которых отображает
// схематически диск и тома, принадлежащие данному диску в пропорциях
// соответствующих размеру каждого тома относительно емкости диска с
// максимальной емкостью.
///////////////////////////////////////////////////////////////////////////////

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
