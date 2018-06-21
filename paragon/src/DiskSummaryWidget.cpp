#include <QLabel>
#include <QVBoxLayout>
#include <QFontMetrics>
#include <QMouseEvent>
#include "Model.hpp"
#include "DiskSummaryWidget.hpp"

DiskSummaryWidget::DiskSummaryWidget (DiskModel * diskModel, QWidget * parent)
    : QFrame(parent)
    , _diskModel(diskModel)
    , _active(false)
{
    auto nameLabel     = new QLabel(_diskModel->name());
    auto typeLabel     = new QLabel(toString(_diskModel->type()));
    auto capacityLabel = new QLabel(toString(_diskModel->capacity()));
    auto statusLabel   = new QLabel(toString(_diskModel->status()));

    // Set Bold font for disk name
    QFont nameFont = nameLabel->font();
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);

    auto layout = new QVBoxLayout;
    layout->addWidget(nameLabel);
    layout->addWidget(typeLabel);
    layout->addWidget(capacityLabel);
    layout->addWidget(statusLabel);
    setLayout(layout);

    QFontMetrics fm(font());
    int width  = fm.width('W') * 10;
    int height = fm.height() * 6;

    setFrameStyle(InactiveFrame);
    setFixedHeight(height);
    setFixedWidth(width);
}

void DiskSummaryWidget::updateFrame ()
{
    if (_active)
        setFrameStyle(ActiveFrame);
    else
        setFrameStyle(InactiveFrame);
}

void DiskSummaryWidget::setActive (bool value)
{
    _active = value;
    updateFrame();
}

void DiskSummaryWidget::onDiskSelected (int diskIndex)
{
    if (diskIndex == _diskModel->index())
        setActive(true);
    else
        setActive(false);
}

// void DiskSummaryWidget::toggle ()
// {
//     _active = !_active;
//     updateFrame();
// }

void DiskSummaryWidget::mousePressEvent (QMouseEvent * event)
{
    setActive(true);
    emitDiskSelected(_diskModel->index());
    event->accept();
}
