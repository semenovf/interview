#pragma once
#include "TableView.hpp"

class Model;

class VolumeListView : public TableView
{
    Q_OBJECT

    Model * _model;

public:
    VolumeListView (Model *, QWidget * parent = 0);

    Q_SLOT void onEntitySelected (int diskIndex, int volumeIndex);
    Q_SIGNAL void emitEntitySelected (int diskIndex, int volumeIndex);

private:
    Q_SLOT void onSelected (QModelIndex const &);

    QModelIndex getItemIndex (int diskIndex, int volumeIndex);
};
