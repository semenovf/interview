#pragma once
#include <QWidget>
#include "Model.hpp"

class DiskSummaryWidget;
class VolumeSummaryWidget;

class GraphicalViewItem : public QWidget
{
    Q_OBJECT

    DiskModel *                  _pdiskModel;
    Capacity                     _maxCapacity;
    DiskSummaryWidget *          _pdisk;
    QList<VolumeSummaryWidget *> _volumes;

public:
    GraphicalViewItem (DiskModel *, Capacity const & maxCapacity, QWidget * parent = 0);

public:
    Q_SLOT void setActive (bool);
};

