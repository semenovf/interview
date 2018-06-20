#pragma once
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

class TableView : public QTableView
{
    Q_OBJECT

protected:
    using ModelType = QStandardItemModel;
    using ItemType  = QStandardItem;

public:
    TableView (QWidget * parent = 0);

protected:
    void setHorizontalHeaderLabels (QStringList const & horizontalHeaderLabels);
};

