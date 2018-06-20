#include <QStringList>
#include <QBrush>
#include "Model.hpp"
#include "VolumeListView.hpp"

static int const DiskNameRole = Qt::UserRole + 1;
static int const VolumeNameRole = Qt::UserRole + 2;

VolumeListView::VolumeListView (Model * model, QWidget * parent)
    : TableView(parent)
    , _model(model)
{
    QStringList headerLabels;
    headerLabels
            << tr("Volume")
            << tr("Layout")
            << tr("Type")
            << tr("File System")
            << tr("Status")
            << tr("Capacity")
            << tr("Free Space")
            << tr("% Free")
            << tr("Fault Tolerance")
            << tr("Overhead");

    setHorizontalHeaderLabels(headerLabels);

    auto viewModel = static_cast<TableView::ModelType *>(this->model());
    int diskCount = _model->diskCount();

    for (int i = 0; i < diskCount; i++) {
        auto diskModel = _model->diskAt(i);
        int volumeCount = diskModel->volumeCount();

        for (int j = 0; j < volumeCount; j++) {
            auto volumeModel = diskModel->volumeAt(j);
            double ratio = 100.f * double(volumeModel->free().value) / double(volumeModel->capacity().value);

            auto nameItem       = new ItemType(volumeModel->name());
            auto layoutItem     = new ItemType(toString(volumeModel->layout()));
            auto typeItem       = new ItemType(toString(volumeModel->type()));
            auto fileSystemItem = new ItemType(toString(volumeModel->fileSystem()));
            auto statusItem     = new ItemType(toString(volumeModel->status()));
            auto capacityItem   = new ItemType(toString(volumeModel->capacity()));
            auto freeItem       = new ItemType(toString(volumeModel->free()));
            auto freeRatioItem  = new ItemType(QString::number(int(ratio)) + " %");
            auto faultItem      = new ItemType(toString(volumeModel->faultTolerance()));
            auto overheadItem   = new ItemType(QString::number(volumeModel->overhead())+ " %");

            Qt::ItemFlags defaultFlags = Qt::NoItemFlags; //Qt::ItemIsEnabled;

            nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            nameItem->setData(diskModel->name(), DiskNameRole);
            nameItem->setData(volumeModel->name(), VolumeNameRole);

            layoutItem->setFlags(defaultFlags);
            typeItem->setFlags(defaultFlags);
            fileSystemItem->setFlags(defaultFlags);
            statusItem->setFlags(defaultFlags);
            capacityItem->setFlags(defaultFlags);
            freeItem->setFlags(defaultFlags);
            freeRatioItem->setFlags(defaultFlags);
            faultItem->setFlags(defaultFlags);
            overheadItem->setFlags(defaultFlags);

//             QBrush brush = nameItem->foreground();
//             layoutItem->setForeground(brush);

            QList<QStandardItem *> items;
            items.append(nameItem);
            items.append(layoutItem);
            items.append(typeItem);
            items.append(fileSystemItem);
            items.append(statusItem);
            items.append(capacityItem);
            items.append(freeItem);
            items.append(freeRatioItem);
            items.append(faultItem);
            items.append(overheadItem);

            viewModel->appendRow(items);
        }
    }

    connect(this, SIGNAL(pressed(QModelIndex const &)), SLOT(onSelected(QModelIndex const &)));
}

void VolumeListView::onSelected (QModelIndex const & index)
{
    if (index.isValid()) {
        auto model = static_cast<ModelType *>(this->model());
        auto item = model->itemFromIndex(index);
        QString diskName = item->data(DiskNameRole).toString();
        QString volumeName = item->data(VolumeNameRole).toString();
        emitSelected(diskName, volumeName);
    }
}
