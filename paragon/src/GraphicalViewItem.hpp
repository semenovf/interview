#pragma once
#include <QWidget>
#include "Model.hpp"

class DiskSummaryWidget;
class VolumeSummaryWidget;

class GraphicalViewItem : public QWidget
{
    Q_OBJECT

    DiskModel *                  _diskModel;
    Capacity                     _maxCapacity;
    DiskSummaryWidget *          _disk;
    QList<VolumeSummaryWidget *> _volumes;

public:
    GraphicalViewItem (DiskModel * diskModel
            , Capacity const & maxCapacity
            , QWidget * parent = 0);

public:
    Q_SLOT void setActiveDisk (bool);
    Q_SLOT void setActiveVolume (int volumeIndex, bool);
};

