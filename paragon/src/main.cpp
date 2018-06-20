#include <QApplication>
#include <QDebug>
#include "Model.hpp"
#include "MainWindow.hpp"

struct TestVolumeModel : VolumeModel
{
    QString volumeName;

    TestVolumeModel (DiskModel * parent) : VolumeModel(parent) {}

    virtual QString name () const override { return volumeName; }
    virtual VolumeLayoutEnum layout () const override { return VolumeLayoutEnum::Unknown; }
    virtual VolumTypeEnum type () const override { return VolumTypeEnum::Unknown; }
    virtual FileSystemEnum fileSystem () const override { return FileSystemEnum::NTFS; }
    virtual VolumeStatusEnum status () const override { return VolumeStatusEnum::Healthy; }
    virtual Capacity capacity () const override { return Capacity(1000); }
    virtual Capacity free () const override { return Capacity(500); }
    virtual bool faultTolerance () const override { return false; }
    virtual int overhead () const override { return 0; }
};

struct TestDiskModel : DiskModel
{
    VolumeModel * volumes[2];
    QString diskName;

    TestDiskModel (Model * parent) : DiskModel(parent) {}

    virtual int volumeCount () const override { return 2; }
    virtual VolumeModel * volumeAt (int index) const override { return volumes[index]; }
    virtual QString name () const override { return diskName; }
    virtual DiskTypeEnum type () const override { return DiskTypeEnum::Basic; }
    virtual Capacity capacity () const override { return Capacity(2000); }
    virtual DiskStatusEnum status () const override { return DiskStatusEnum::Online; }
};

struct TestModel : Model
{
    DiskModel * disks[2];

    virtual int diskCount () const override { return 2; }
    virtual DiskModel * diskAt (int index) const override { return disks[index]; }
};

int main (int argc, char * argv[])
{
    TestModel model;

    TestDiskModel diskModel0(& model);
    TestVolumeModel volumeModel0_0(& diskModel0);
    TestVolumeModel volumeModel0_1(& diskModel0);
    diskModel0.diskName = "Disk 0";
    volumeModel0_0.volumeName = "C:";
    volumeModel0_1.volumeName = "D:";
    diskModel0.volumes[0] = & volumeModel0_0;
    diskModel0.volumes[1] = & volumeModel0_1;

    TestDiskModel diskModel1(& model);
    TestVolumeModel volumeModel1_0(& diskModel1);
    TestVolumeModel volumeModel1_1(& diskModel1);
    diskModel1.diskName = "CD-ROM 0";
    volumeModel1_0.volumeName = "E:";
    volumeModel1_1.volumeName = "F:";
    diskModel1.volumes[0] = & volumeModel1_0;
    diskModel1.volumes[1] = & volumeModel1_1;

    model.disks[0] = & diskModel0;
    model.disks[1] = & diskModel1;

    QApplication app(argc, argv);
    app.setOrganizationName("paragon-exam");
    app.setApplicationName("partman");

//     Model * model = requestModel();
//
//     if (!model) {
//         qWarning() << "ERROR: Failed to request model";
//         return -1;
//     }

    MainWindow mainWin(& model);
    mainWin.show();

//     auto graphicalView = new GraphicalView(& model);
//     //graphicalView->setActive("Disk 0");
//
//     mainWin.setTopWidget(graphicalView);

    return app.exec();
}
