#pragma once
#include <QTableView>

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

class TableView : public QTableView
{
    Q_OBJECT

public:
    TableView (QWidget * parent = 0);

protected:
    void setHorizontalHeaderLabels (QStringList const & horizontalHeaderLabels);
};

