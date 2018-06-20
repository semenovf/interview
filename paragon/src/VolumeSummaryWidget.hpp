#pragma once
#include <QFrame>
#include "Model.hpp"

class VolumeSummaryWidget : public QFrame
{
    Q_OBJECT

    static int const InactiveFrame = QFrame::Panel | QFrame::Raised;
    static int const ActiveFrame = QFrame::Panel | QFrame::Sunken;

    VolumeModel * _pmodel;
    bool _active;

public:
    VolumeSummaryWidget (VolumeModel *, QWidget * parent = 0);

public:
    Q_SLOT void setActive (bool);
    Q_SLOT void toggle ();

private:
    void updateFrame ();
    void updateBackground ();
};
