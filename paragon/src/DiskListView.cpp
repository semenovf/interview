#include "Model.hpp"
#include "DiskListView.hpp"

static int const DiskIndexRole = Qt::UserRole + 1;

DiskListView::DiskListView (Model * model, QWidget * parent)
    : TableView(parent)
    , _model(model)
{
    QStringList headerLabels;
    headerLabels
            << tr("Disk")
            << tr("Type")
            << tr("Capacity")
            << tr("Free Space")
            << tr("Status");

    setHorizontalHeaderLabels(headerLabels);

    auto viewModel = static_cast<TableView::ModelType *>(this->model());
    int count = _model->diskCount();

    for (int i = 0; i < count; i++) {
        auto diskModel = _model->diskAt(i);
        auto nameItem     = new ItemType(diskModel->name());
        auto typeItem     = new ItemType(toString(diskModel->type()));
        auto capacityItem = new ItemType(toString(diskModel->capacity()));
        auto freeItem     = new ItemType(toString(diskModel->free()));
        auto statusItem   = new ItemType(toString(diskModel->status()));

        Qt::ItemFlags defaultFlags = Qt::NoItemFlags; //Qt::ItemIsEnabled;

        nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        nameItem->setData(i, DiskIndexRole);
        typeItem->setFlags(defaultFlags);
        capacityItem->setFlags(defaultFlags);
        freeItem->setFlags(defaultFlags);
        statusItem->setFlags(defaultFlags);

        QList<QStandardItem *> items;
        items.append(nameItem);
        items.append(typeItem);
        items.append(capacityItem);
        items.append(freeItem);
        items.append(statusItem);

        viewModel->appendRow(items);
    }

    connect(this, SIGNAL(activated(QModelIndex const &)), SLOT(onSelected(QModelIndex const &)));
    connect(this, SIGNAL(clicked(QModelIndex const &)), SLOT(onSelected(QModelIndex const &)));
}

QModelIndex DiskListView::getItemIndex (int diskIndex)
{
    auto model = static_cast<ModelType *>(this->model());
    int rowCount = model->rowCount();

    // Check first column in all rows to find specified disk
    for (int i = 0; i < rowCount; i++) {
        auto item = model->item(i, 0);
        int di = item->data(DiskIndexRole).toInt();

        if (di == diskIndex)
            return model->index(i, 0);
    }

    return QModelIndex();
}

void DiskListView::onEntitySelected (int diskIndex, int volumeIndex)
{
    if (volumeIndex < 0) {
        auto index = getItemIndex(diskIndex);

        if (index.isValid()) {
            setCurrentIndex(index);
        }
    } else {
        auto selectionModel = this->selectionModel();
        selectionModel->clear();
    }
}

void DiskListView::onSelected (QModelIndex const & index)
{
    if (index.isValid()) {
        auto model = static_cast<ModelType *>(this->model());
        auto item = model->itemFromIndex(index);
        int diskIndex = item->data(DiskIndexRole).toInt();
        emitEntitySelected(diskIndex, -1);
    }
}

