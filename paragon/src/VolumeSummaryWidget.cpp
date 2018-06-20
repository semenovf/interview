#include <QLabel>
#include <QVBoxLayout>
#include <QFontMetrics>
#include <QPalette>
#include "Model.hpp"
#include "VolumeSummaryWidget.hpp"

VolumeSummaryWidget::VolumeSummaryWidget (VolumeModel * pmodel, QWidget * parent)
    : QFrame(parent)
    , _pmodel(pmodel)
    , _active(false)
{
    QFontMetrics fm(font());

    setContentsMargins(0,0,0,0);

    // Only for test
    QString name              = _pmodel->name();
    Capacity capacity         = _pmodel->capacity();
    FileSystemEnum fileSystem = _pmodel->fileSystem();
    VolumeStatusEnum status   = _pmodel->status();

    auto captionLabel  = new QLabel();
    auto nameLabel     = new QLabel(name);
    auto compaundLabel = new QLabel(toString(capacity) + " " + toString(fileSystem));
    auto statusLabel   = new QLabel(toString(status));

    QPalette captionPalette;
    captionPalette.setColor(QPalette::Background, Qt::blue);
    captionLabel->setAutoFillBackground(true);
    captionLabel->setPalette(captionPalette);
    captionLabel->setFixedHeight(fm.height());

    // Set Bold font for disk name
    QFont nameFont = nameLabel->font();
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);

    auto innerLayout = new QVBoxLayout;
    innerLayout->setContentsMargins(10,10,10,10);
    innerLayout->addWidget(nameLabel);
    innerLayout->addWidget(compaundLabel);
    innerLayout->addWidget(statusLabel);

    auto layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(captionLabel);
    layout->addLayout(innerLayout);

    setLayout(layout);

//    int width  = fm.width('W') * 10;
    int height = fm.height() * 6;

    setFrameStyle(InactiveFrame);
    setFixedHeight(height);
//     setFixedWidth(width);
}

void VolumeSummaryWidget::updateFrame ()
{
    if (_active)
        setFrameStyle(ActiveFrame);
    else
        setFrameStyle(InactiveFrame);
}

void VolumeSummaryWidget::updateBackground ()
{
}

void VolumeSummaryWidget::setActive (bool value)
{
    _active = value;
    updateFrame();
    updateBackground();
}

void VolumeSummaryWidget::toggle ()
{
    _active = !_active;
    updateFrame();
    updateBackground();
}
