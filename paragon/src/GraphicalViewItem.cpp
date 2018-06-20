#include <QHBoxLayout>
#include "DiskSummaryWidget.hpp"
#include "VolumeSummaryWidget.hpp"
#include "GraphicalViewItem.hpp"

GraphicalViewItem::GraphicalViewItem (DiskModel * pmodel
            , Capacity const & maxCapacity
            , QWidget * parent)
    : QWidget(parent)
    , _pdiskModel(pmodel)
    , _maxCapacity(maxCapacity)
{
    auto layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);

    _pdisk = new DiskSummaryWidget(pmodel, this);

    layout->addWidget(_pdisk);

    auto count = _pdiskModel->volumeCount();

    for (int i = 0; i < count; i++) {
        auto pvolume = new VolumeSummaryWidget(_pdiskModel->volumeAt(i), this);
        _volumes.append(pvolume);
        layout->addWidget(pvolume);
    }

    setLayout(layout);
}

void GraphicalViewItem::setActive (bool value)
{
    _pdisk->setActive(value);

    for (auto volume: _volumes)
        volume->setActive(value);
}
