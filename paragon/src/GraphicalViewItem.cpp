#include <QHBoxLayout>
#include "DiskSummaryWidget.hpp"
#include "VolumeSummaryWidget.hpp"
#include "GraphicalViewItem.hpp"

GraphicalViewItem::GraphicalViewItem (DiskModel * diskModel
            , Capacity const & maxCapacity
            , QWidget * parent)
    : QWidget(parent)
    , _diskModel(diskModel)
    , _maxCapacity(maxCapacity)
{
    auto layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);

    _disk = new DiskSummaryWidget(_diskModel, this);

    layout->addWidget(_disk);

    auto count = _diskModel->volumeCount();

    for (int i = 0; i < count; i++) {
        auto volume = new VolumeSummaryWidget(_diskModel->volumeAt(i), this);
        _volumes.append(volume);
        layout->addWidget(volume);
    }

    setLayout(layout);
}

void GraphicalViewItem::setActiveDisk (bool value)
{
    _disk->setActive(value);
}

void GraphicalViewItem::setActiveVolume (int volumeIndex, bool value)
{
}

// void GraphicalViewItem::setActive (bool value)
// {
//     _pdisk->setActive(value);
//
//     for (auto volume: _volumes)
//         volume->setActive(value);
// }
