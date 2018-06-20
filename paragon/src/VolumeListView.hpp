#pragma once
#include "TableView.hpp"

class Model;

class VolumeListView : public TableView
{
    Q_OBJECT

    Model * _model;

public:
    VolumeListView (Model *, QWidget * parent = 0);

public:
    Q_SLOT void onVolumeSelected (QString const & diskName, QString const & volumeName);
    Q_SIGNAL void emitSelected (QString const & diskName, QString const & volumeName);

private:
    Q_SLOT void onSelected (QModelIndex const &);

};
