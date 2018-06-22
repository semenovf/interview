#include "Model.hpp"

template <Capacity::type CapacityValue, Capacity::type FreeValue>
struct TestVolumeModel : VolumeModel
{
    QString volumeName;

    TestVolumeModel (int index, QString const & name, DiskModel * parent)
            : VolumeModel(index, parent)
            , volumeName(name)
    {}

    virtual QString name () const override { return volumeName; }
    virtual VolumeLayoutEnum layout () const override { return VolumeLayoutEnum::Simple; }
    virtual VolumTypeEnum type () const override { return VolumTypeEnum::Basic; }
    virtual FileSystemEnum fileSystem () const override { return FileSystemEnum::NTFS; }
    virtual VolumeStatusEnum status () const override { return VolumeStatusEnum::Healthy; }
    virtual Capacity capacity () const override { return Capacity(CapacityValue); }
    virtual Capacity free () const override { return Capacity(FreeValue); }
    virtual bool faultTolerance () const override { return false; }
    virtual int overhead () const override { return 0; }
};

template <int VolumeCount, Capacity::type CapacityValue>
struct TestDiskModel : DiskModel
{
    VolumeModel * volumes[VolumeCount];
    QString diskName;

    TestDiskModel (int index, QString const & name, Model * parent)
        : DiskModel(index, parent)
        , diskName(name)
    {}

    virtual int volumeCount () const override { return VolumeCount; }
    virtual VolumeModel * volumeAt (int index) const override { return volumes[index]; }
    virtual QString name () const override { return diskName; }
    virtual DiskTypeEnum type () const override { return DiskTypeEnum::Basic; }
    virtual Capacity capacity () const override { return Capacity(CapacityValue); }
    virtual DiskStatusEnum status () const override { return DiskStatusEnum::Online; }
};

template <int DiskCount>
struct TestModel : Model
{
    DiskModel * disks[DiskCount];

    virtual int diskCount () const override { return DiskCount; }
    virtual DiskModel * diskAt (int index) const override { return disks[index]; }
};

static TestModel<3> model;
static TestDiskModel<3, 1000> diskModel0(0, "Disk 0", & model);
static TestVolumeModel<400, 150> volumeModel0_0(0, "C:", & diskModel0);
static TestVolumeModel<200, 100> volumeModel0_1(1, "D:", & diskModel0);
static TestVolumeModel<400, 400> volumeModel0_2(2, "E:", & diskModel0);

static TestDiskModel<1, 650> diskModel1(1, "CD-ROM 0", & model);
static TestVolumeModel<650, 650> volumeModel1_0(0, "O:", & diskModel1);

static TestDiskModel<2, 500> diskModel2(2, "Disk 1", & model);
static TestVolumeModel<100, 100> volumeModel2_0(0, "F:", & diskModel2);
static TestVolumeModel<400, 250> volumeModel2_1(1, "G:", & diskModel2);

Model * requestModel (QString *)
{
    diskModel0.volumes[0] = & volumeModel0_0;
    diskModel0.volumes[1] = & volumeModel0_1;
    diskModel0.volumes[2] = & volumeModel0_2;

    diskModel1.volumes[0] = & volumeModel1_0;

    diskModel2.volumes[0] = & volumeModel2_0;
    diskModel2.volumes[1] = & volumeModel2_1;

    model.disks[0] = & diskModel0;
    model.disks[1] = & diskModel1;
    model.disks[2] = & diskModel2;

    return & model;
}

void releaseModel (Model *)
{}
