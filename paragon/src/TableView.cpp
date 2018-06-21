#include <QStandardItemModel>
#include <QStringList>
#include <QPalette>
#include <QHeaderView>
#include "TableView.hpp"

class TableViewModel : public QStandardItemModel
{
    QColor _color;

public:
    TableViewModel (QColor color, QObject * parent = 0)
        : QStandardItemModel(parent)
        , _color(color)
    {}

    virtual QVariant data (QModelIndex const & index, int role = Qt::DisplayRole ) const override;
};

TableView::TableView (QWidget * parent)
    : QTableView(parent)
{
    QColor color = palette().brush(QPalette::Normal, QPalette::Foreground).color();

    auto model = new TableViewModel(color);
    verticalHeader()->hide();
    setModel(model);
    setSelectionMode(SingleSelection);
}

void TableView::setHorizontalHeaderLabels (QStringList const & labels)
{
    auto model = static_cast<ModelType *>(this->model());
    model->setHorizontalHeaderLabels(labels);
}

QVariant TableViewModel::data (QModelIndex const & index, int role) const
{
    QVariant result = QStandardItemModel::data(index, role);

    if (role == Qt::ForegroundRole) {
        return _color;
    }

    return result;
}
