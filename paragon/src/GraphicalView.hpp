#pragma once
#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QFrame>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class DiskModel;
class VolumeModel;
class GraphicalView;

class DiskSummaryWidget : public QPushButton
{
    Q_OBJECT

    DiskModel & _model;

public:
    DiskSummaryWidget (DiskModel & model, QWidget * parent = 0);
};

class PartitionSummaryWidget : public QFrame
{
    Q_OBJECT

public:
    PartitionSummaryWidget (QWidget * parent = 0);
};

class GraphicalViewItem : public QWidget
{
    Q_OBJECT

    DiskSummaryWidget *             _disk;
    QList<PartitionSummaryWidget *> _partitions;

public:
    GraphicalViewItem (GraphicalView * parent = 0);
};

class GraphicalView : public QWidget
{
    Q_OBJECT

public:
    GraphicalView (QWidget * parent = 0);
};


