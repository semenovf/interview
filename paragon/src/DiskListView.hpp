#pragma once
#include "TableView.hpp"

class Model;

class DiskListView : public TableView
{
    Q_OBJECT

    Model * _model;

public:
    DiskListView (Model *, QWidget * parent = 0);

    Q_SIGNAL void emitEntitySelected (int diskIndex, int volumeIndex);
    Q_SLOT void onEntitySelected (int diskIndex, int volumeIndex);

private:
    Q_SLOT void onSelected (QModelIndex const &);

    QModelIndex getItemIndex (int diskIndex);
};
