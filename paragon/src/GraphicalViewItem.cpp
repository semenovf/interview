#include <QHBoxLayout>
#include "DiskSummaryWidget.hpp"
#include "VolumeSummaryWidget.hpp"
#include "GraphicalViewItem.hpp"

GraphicalViewItem::GraphicalViewItem (DiskModel * diskModel
            , QWidget * parent)
    : QWidget(parent)
    , _diskModel(diskModel)
{
    auto layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);

    _disk = new DiskSummaryWidget(_diskModel, this);
    connect(_disk, SIGNAL(emitEntitySelected(int, int)), this, SIGNAL(emitEntitySelected(int, int)));

    layout->addWidget(_disk);

    auto count = _diskModel->volumeCount();

    auto maxCapacity = _diskModel->parentModel->maxCapacity();
    auto diskCapacity = _diskModel->capacity();
    int tailStretch = 100;

    for (int i = 0; i < count; i++) {
        auto volumeModel = _diskModel->volumeAt(i);
        auto volume = new VolumeSummaryWidget(volumeModel, this);
        _volumes.append(volume);

        int stretch = (100 * volumeModel->capacity().value) / maxCapacity.value;
        layout->addWidget(volume, stretch);
        tailStretch -= stretch;

        connect(volume, SIGNAL(emitEntitySelected(int, int)), this, SIGNAL(emitEntitySelected(int, int)));
    }

    if (tailStretch > 0)
        layout->addStretch(tailStretch);

    setLayout(layout);
}

void GraphicalViewItem::onEntitySelected (int diskIndex, int volumeIndex)
{
    _disk->onEntitySelected(diskIndex, volumeIndex);

    for (auto volume: _volumes)
        volume->onEntitySelected(diskIndex, volumeIndex);
}

