#pragma once
#include <QWidget>
#include "Model.hpp"

class DiskSummaryWidget;
class VolumeSummaryWidget;

class GraphicalViewItem : public QWidget
{
    Q_OBJECT

    DiskModel *                  _diskModel;
    DiskSummaryWidget *          _disk;
    QList<VolumeSummaryWidget *> _volumes;

public:
    GraphicalViewItem (DiskModel * diskModel, QWidget * parent = 0);

    Q_SIGNAL void emitEntitySelected (int diskIndex, int volumeIndex);
    Q_SLOT void onEntitySelected (int diskIndex, int volumeIndex);
};

