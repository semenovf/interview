#pragma once
#include <QFrame>
#include "Model.hpp"

QT_BEGIN_NAMESPACE
class QMouseEvent;
QT_END_NAMESPACE

class VolumeSummaryWidget : public QFrame
{
    Q_OBJECT

    static int const InactiveFrame = QFrame::Panel | QFrame::Raised;
    static int const ActiveFrame = QFrame::Panel | QFrame::Sunken;

    VolumeModel * _volumeModel;
    bool          _active;

public:
    VolumeSummaryWidget (VolumeModel * volumeModel, QWidget * parent = 0);

public:
    Q_SLOT void setActive (bool);
    Q_SLOT void toggle ();

protected:
    virtual void mousePressEvent (QMouseEvent * event) override;

private:
    void updateFrame ();
    void updateBackground ();
};
