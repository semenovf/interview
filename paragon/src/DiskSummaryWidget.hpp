#pragma once
#include <QFrame>
#include "Model.hpp"

class DiskSummaryWidget : public QFrame
{
    Q_OBJECT

    static int const InactiveFrame = QFrame::Panel | QFrame::Raised;
    static int const ActiveFrame = QFrame::Panel | QFrame::Sunken;

    DiskModel * _pmodel;
    bool _active;
public:
    //DiskSummaryWidget (DiskModel & model, QWidget * parent = 0);
    DiskSummaryWidget (DiskModel *, QWidget * parent = 0);

public:
    Q_SLOT void setActive (bool);
    Q_SLOT void toggle ();

private:
    void updateFrame ();
};

