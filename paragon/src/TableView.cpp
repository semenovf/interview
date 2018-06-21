#include <QStandardItemModel>
#include <QStringList>
#include "TableView.hpp"

TableView::TableView (QWidget * parent)
    : QTableView(parent)
{
    auto model = new ModelType;
    setModel(model);
    setSelectionMode(SingleSelection);
}

void TableView::setHorizontalHeaderLabels (QStringList const & labels)
{
    auto model = static_cast<ModelType *>(this->model());
    model->setHorizontalHeaderLabels(labels);
}