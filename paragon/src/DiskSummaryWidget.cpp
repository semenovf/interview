#include <QLabel>
#include <QVBoxLayout>
#include <QFontMetrics>
#include "Model.hpp"
#include "DiskSummaryWidget.hpp"

DiskSummaryWidget::DiskSummaryWidget (DiskModel * pmodel, QWidget * parent)
    : QFrame(parent)
    , _active(false)
    , _pmodel(pmodel)
{
//     auto nameLabel = new QLabel(_model.name());
//     auto typeLabel = new QLabel(toString(_model.type()));
//     auto capacityLabel = new QLabel(toString(_model.capacity()));

    // Only for test
    auto name     = QString("Disk 0");
    auto type     = fromString<DiskTypeEnum>(tr("Basic"));
    auto capacity = Capacity(100000);
    auto status   = fromString<DiskStatusEnum>(tr("Online"));

    auto nameLabel     = new QLabel(name);
    auto typeLabel     = new QLabel(toString(type));
    auto capacityLabel = new QLabel(toString(capacity));
    auto statusLabel   = new QLabel(toString(status));

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

void DiskSummaryWidget::toggle ()
{
    _active = !_active;
    updateFrame();
}
