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
        auto pitem = new GraphicalViewItem(_pmodel->diskAt(i), _pmodel->maxCapacity(), this);
        _items.append(pitem);
        layout->addWidget(pitem);
    }

    layout->addStretch(100);
// void
// addSpacerItem ( QSpacerItem * spacerItem )

    setLayout(layout);
}

void GraphicalView::onDiskSelected (int diskIndex)
{
    qDebug() << "Disk selected: " << diskIndex;
}

void GraphicalView::onVolumeSelected (int diskIndex, int volumeIndex)
{
    qDebug() << "Volume selected: " << diskIndex << ":" << volumeIndex;
}

