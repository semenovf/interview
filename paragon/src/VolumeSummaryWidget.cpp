#include <QLabel>
#include <QVBoxLayout>
#include <QFontMetrics>
#include <QPalette>
#include <QMouseEvent>
#include "Model.hpp"
#include "VolumeSummaryWidget.hpp"

VolumeSummaryWidget::VolumeSummaryWidget (VolumeModel * volumeModel, QWidget * parent)
    : QFrame(parent)
    , _volumeModel(volumeModel)
    , _active(false)
{
    QFontMetrics fm(font());

    setContentsMargins(0,0,0,0);

    // Only for test
    QString name              = _volumeModel->name();
    Capacity capacity         = _volumeModel->capacity();
    FileSystemEnum fileSystem = _volumeModel->fileSystem();
    VolumeStatusEnum status   = _volumeModel->status();

    auto captionLabel  = new QLabel();
    auto nameLabel     = new QLabel(name);
    auto compaundLabel = new QLabel(toString(capacity) + " " + toString(fileSystem));
    auto statusLabel   = new QLabel(toString(status));

    QPalette thisPalette = this->palette();

    QPalette captionPalette;
    captionPalette.setColor(QPalette::Window, Qt::blue);
    captionLabel->setAutoFillBackground(true);
    captionLabel->setPalette(captionPalette);
    captionLabel->setFixedHeight(fm.height());

    // Set backround color for labels as for parent widget
    QPalette labelPalette;
    labelPalette.setColor(QPalette::Window, thisPalette.color(QPalette::Window));
    nameLabel->setPalette(labelPalette);
    nameLabel->setAutoFillBackground(true);
    compaundLabel->setPalette(labelPalette);
    compaundLabel->setAutoFillBackground(true);
    statusLabel->setPalette(labelPalette);
    statusLabel->setAutoFillBackground(true);

    // Set labels fix-sized
    nameLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    compaundLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    statusLabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

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
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    QBrush brush = palette.brush(QPalette::Window);
    QColor color;

    if (_active) {
        color = Qt::white;
        brush.setColor(Qt::black);
        brush.setStyle(Qt::BDiagPattern);
    } else {
        color = Qt::white;
        brush.setStyle(Qt::NoBrush);
    }

    palette.setColor(QPalette::Window, color);
    palette.setBrush(QPalette::Window, brush);
    setAutoFillBackground(true);
    setPalette(palette);
}

void VolumeSummaryWidget::setActive (bool value)
{
    _active = value;
    updateFrame();
    updateBackground();
}

void VolumeSummaryWidget::mousePressEvent (QMouseEvent * event)
{
    setActive(true);
    emitEntitySelected(_volumeModel->diskIndex(), _volumeModel->index());
    event->accept();
}

void VolumeSummaryWidget::onEntitySelected (int diskIndex, int volumeIndex)
{
    if (diskIndex == _volumeModel->diskIndex()
            && volumeIndex == _volumeModel->index())
        setActive(true);
    else
        setActive(false);
}
