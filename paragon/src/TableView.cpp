#include <QStandardItemModel>
#include <QStringList>
#include "TableView.hpp"

using ModelType = QStandardItemModel;

TableView::TableView (QWidget * parent)
    : QTableView(parent)
{
    auto model = new ModelType;
    this->setModel(model);
}

void TableView::setHorizontalHeaderLabels (QStringList const & labels)
{
    auto model = static_cast<ModelType *>(this->model());
    model->setHorizontalHeaderLabels(labels);
}
