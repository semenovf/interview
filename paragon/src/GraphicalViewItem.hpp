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

    Q_SIGNAL void emitDiskSelected (int diskIndex);
    Q_SIGNAL void emitVolumeSelected (int diskIndex, int volumeIndex);
    Q_SLOT void onDiskSelected (int diskIndex);
    Q_SLOT void onVolumeSelected (int diskIndex, int volumeIndex);
};

