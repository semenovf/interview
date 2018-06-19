#include "DiskListView.hpp"

DiskListView::DiskListView (QWidget * parent)
    : TableView(parent)
{
    QStringList headerLabels;
    headerLabels
            << tr("Disk")
            << tr("Type")
            << tr("Capacity")
            << tr("Free Space")
            << tr("Status")
            << tr("Device Type")
            << tr("Partition?");

    setHorizontalHeaderLabels(headerLabels);
}
