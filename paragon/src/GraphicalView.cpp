#include <QLabel>
#include <QVBoxLayout>
#include "Model.hpp"
#include "GraphicalViewItem.hpp"
#include "GraphicalView.hpp"

#include <QDebug>

GraphicalView::GraphicalView (Model * model, QWidget * parent)
    : QWidget(parent)
    , _pmodel(model)
{
    auto layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);

    auto count = _pmodel->diskCount();

    for (int i = 0; i < count; i++) {
        auto item = new GraphicalViewItem(_pmodel->diskAt(i), this);
        _items.append(item);
        layout->addWidget(item);

        connect(item, SIGNAL(emitDiskSelected(int)), this, SIGNAL(emitDiskSelected(int)));
        connect(item, SIGNAL(emitVolumeSelected(int, int)), this, SIGNAL(emitVolumeSelected(int, int)));
    }

    layout->addStretch(100);
// void
// addSpacerItem ( QSpacerItem * spacerItem )

    setLayout(layout);
}

void GraphicalView::onDiskSelected (int diskIndex)
{
    for (auto item: _items) {
        item->onDiskSelected(diskIndex);
    }
}

void GraphicalView::onVolumeSelected (int diskIndex, int volumeIndex)
{
    for (auto item: _items) {
        item->onVolumeSelected(diskIndex, volumeIndex);
    }
}

