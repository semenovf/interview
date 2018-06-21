#pragma once
#include <QFrame>
#include "Model.hpp"

QT_BEGIN_NAMESPACE
class QMouseEvent;
QT_END_NAMESPACE

class DiskSummaryWidget : public QFrame
{
    Q_OBJECT

    static int const InactiveFrame = QFrame::Panel | QFrame::Raised;
    static int const ActiveFrame = QFrame::Panel | QFrame::Sunken;

    DiskModel * _diskModel;
    bool        _active;

public:
    DiskSummaryWidget (DiskModel * diskModel, QWidget * parent = 0);

    Q_SIGNAL void emitDiskSelected (int diskIndex);
    Q_SLOT void onDiskSelected (int diskIndex);
    //Q_SLOT void toggle ();

protected:
    virtual void mousePressEvent (QMouseEvent * event) override;

private:
    void setActive (bool);
    void updateFrame ();
};

