#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include "Model.hpp"
#include "GraphicalViewItem.hpp"
#include "GraphicalView.hpp"

#include <QDebug>

GraphicalView::GraphicalView (Model * model, QWidget * parent)
    : QScrollArea(parent)
    , _model(model)
{
    auto scrollAreaWidget = new QWidget;
    this->setWidgetResizable(true);
    this->setWidget(scrollAreaWidget);
    //this->setBackgroundRole(QPalette::Dark);

    auto scrollAreaLayout = new QVBoxLayout;
    scrollAreaLayout->setContentsMargins(0,0,0,0);
    scrollAreaWidget->setLayout(scrollAreaLayout);

    auto count = _model->diskCount();

    for (int i = 0; i < count; i++) {
        auto item = new GraphicalViewItem(_model->diskAt(i), scrollAreaWidget);
        _items.append(item);
        scrollAreaLayout->addWidget(item);

        connect(item, SIGNAL(emitEntitySelected(int, int)), this, SIGNAL(emitEntitySelected(int, int)));
    }

    scrollAreaLayout->addStretch(100);
}

void GraphicalView::onEntitySelected (int diskIndex, int volumeIndex)
{
    for (auto item: _items)
        item->onEntitySelected(diskIndex, volumeIndex);
}

