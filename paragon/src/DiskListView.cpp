#include "Model.hpp"
#include "DiskListView.hpp"

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

        QList<QStandardItem *> items;
        items.append(nameItem);
        items.append(typeItem);
        items.append(capacityItem);
        items.append(freeItem);
        items.append(statusItem);

        viewModel->appendRow(items);
    }
}
