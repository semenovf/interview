#include <QStringList>
#include "VolumeListView.hpp"

VolumeListView::VolumeListView (QWidget * parent)
    : TableView(parent)
{
    QStringList headerLabels;
    headerLabels
            << tr("Volume")
            << tr("Layout")
            << tr("Type")
            << tr("File System")
            << tr("Status")
            << tr("Capacity")
            << tr("Free Space")
            << tr("% Free")
            << tr("Fault Tolerance")
            << tr("Overhead");

    setHorizontalHeaderLabels(headerLabels);
}
