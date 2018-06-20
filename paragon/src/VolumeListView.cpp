#include <QStringList>
#include <QBrush>
#include "Model.hpp"
#include "VolumeListView.hpp"

static int const DiskIndexRole = Qt::UserRole + 1;
static int const VolumeIndexRole = Qt::UserRole + 2;

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

            nameItem->setData(i, DiskIndexRole);
            nameItem->setData(j, VolumeIndexRole);

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

    connect(this, SIGNAL(activated(QModelIndex const &)), SLOT(onSelected(QModelIndex const &)));
    connect(this, SIGNAL(clicked(QModelIndex const &)), SLOT(onSelected(QModelIndex const &)));
}

QModelIndex VolumeListView::getItemIndex (int diskIndex, int volumeIndex)
{
    auto model = static_cast<ModelType *>(this->model());
    int rowCount = model->rowCount();

    // Check first column in all rows to find specified volume
    for (int i = 0; i < rowCount; i++) {
        auto item = model->item(i, 0);
        int di = item->data(DiskIndexRole).toInt();
        int vi = item->data(VolumeIndexRole).toInt();

        if (di == diskIndex && vi == volumeIndex)
            return model->index(i, 0);
    }

    return QModelIndex();
}

void VolumeListView::onVolumeSelected (int diskIndex, int volumeIndex)
{
    auto index = getItemIndex(diskIndex, volumeIndex);

    if (index.isValid()) {
        setCurrentIndex(index);
    }
}

void VolumeListView::onSelected (QModelIndex const & index)
{
    if (index.isValid()) {
        auto model = static_cast<ModelType *>(this->model());
        auto item = model->itemFromIndex(index);
        int diskIndex = item->data(DiskIndexRole).toInt();
        int volumeIndex = item->data(VolumeIndexRole).toInt();
        emitVolumeSelected(diskIndex, volumeIndex);
    }
}
