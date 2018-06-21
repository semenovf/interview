#include <QApplication>
#include <QFile>
#include <QList>
#include <QDomDocument>
#include <QDebug>
#include "Model.hpp"

template <int CapacityValue, int FreeValue>
struct TestVolumeModel : VolumeModel
{
    QString volumeName;

    TestVolumeModel (int index, QString const & name, DiskModel * parent)
            : VolumeModel(index, parent)
            , volumeName(name)
    {}

    virtual QString name () const override { return volumeName; }
    virtual VolumeLayoutEnum layout () const override { return VolumeLayoutEnum::Unknown; }
    virtual VolumTypeEnum type () const override { return VolumTypeEnum::Unknown; }
    virtual FileSystemEnum fileSystem () const override { return FileSystemEnum::NTFS; }
    virtual VolumeStatusEnum status () const override { return VolumeStatusEnum::Healthy; }
    virtual Capacity capacity () const override { return Capacity(CapacityValue); }
    virtual Capacity free () const override { return Capacity(FreeValue); }
    virtual bool faultTolerance () const override { return false; }
    virtual int overhead () const override { return 0; }
};

template <int VolumeCount, int CapacityValue>
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

Model * requestModel ()
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


// struct TestVolumeModel : VolumeModel
// {
//     QDomElement elem;
//
//     TestVolumeModel (QDomElement e) : elem(e) {}
//
//     virtual QString name () const override { return QString("Volume-Name"); }
//     virtual VolumeLayoutEnum layout () const override { return VolumeLayoutEnum::Unknown; }
//     virtual VolumTypeEnum type () const override { return VolumTypeEnum::Unknown; }
//     virtual FileSystemEnum fileSystem () const override { return FileSystemEnum::Unknown; }
//     virtual VolumeStatusEnum status () const override { return VolumeStatusEnum::Unknown; }
//     virtual Capacity capacity () const override { return Capacity(0); }
//     virtual Capacity free () const override { return Capacity(0); }
//     virtual bool faultTolerance () const override { return false; }
//     virtual int overhead () const override { return 0; }
// };
//
// struct TestDiskModel : DiskModel
// {
//     QDomElement elem;
//
//     TestDiskModel (QDomElement e) : elem(e) {}
//
//     virtual int volumeCount () const override
//     {
//         QDomNodeList list = elem.elementsByTagName("volume");
//         return list.count();
//     }
//
//     virtual VolumeModel::SharedType volumeAt (int index) const override
//     {
//         QDomNodeList list = elem.elementsByTagName("volume");
//
//         if (index < 0 || index >= list.count())
//             return VolumeModel::SharedType();
//
//         QDomNode node = list.at(index);
//         auto result = QSharedPointer<TestVolumeModel>(new TestVolumeModel(node.toElement()));
//
//         return result.staticCast<VolumeModel>();
//     }
//
//     virtual QString name () const override { return QString("Disk-Name"); }
//     virtual DiskTypeEnum type () const override { return DiskTypeEnum::Unknown; }
//     virtual Capacity capacity () const override { return Capacity(0); }
//     virtual DiskStatusEnum status () const override { return DiskStatusEnum::Unknown; }
// };
//
// struct TestModel : Model
// {
//     QDomDocument doc;
//
//     virtual int diskCount () const override
//     {
//         QDomNodeList list = doc.elementsByTagName("disk");
//         return list.count();
//     }
//
//     virtual DiskModel::SharedType diskAt (int index) const override
//     {
//         QDomNodeList list = doc.elementsByTagName("disk");
//
//         if (index < 0 || index >= list.count())
//             return DiskModel::SharedType();
//
//         QDomNode node = list.at(index);
//         auto result = QSharedPointer<TestDiskModel>(new TestDiskModel(node.toElement()));
//
//         return result.staticCast<DiskModel>();
//     }
// };
//
// Model * requestModel ()
// {
//     QString appName = qApp->applicationName();
//     QString fileName = QString("./%1.xml").arg(appName);
//     QFile file(fileName);
//
//     if (!file.open(QIODevice::ReadOnly))
//         return nullptr;
//
//     auto pmodel = new TestModel;
//
//     QString errorMsg;
//     int errorLine = 0;
//     int errorColumn = 0;
//
//     if (!pmodel->doc.setContent(& file, & errorMsg, & errorLine, & errorColumn)) {
//         qWarning() << "ERROR: Failed to set content from XML: "
//                 << errorMsg
//                 << " at line " << errorLine
//                 << " and column " << errorColumn;
//         delete pmodel;
//         pmodel = nullptr;
//     }
//
//     file.close();
//
//     return pmodel;
// }
//
// void releaseModel (Model * model)
// {
//     delete model;
// }

// struct TestVolumeData
// {
//     QString name;
//     VolumeLayoutEnum layout;
//     VolumTypeEnum type;
//     FileSystemEnum fileSystem;
//     VolumeStatusEnum status;
//     Capacity capacity;
//     Capacity free;
//     bool faultTolerance;
//     int overhead;
//
//     using SharedType = QSharedPointer<TestVolumeData>;
//
//     static SharedType make ()
//     {
//         return SharedType(new TestVolumeData);
//     }
// };
//
// struct TestDiskData
// {
//     QString name;
//     DiskTypeEnum type;
//     Capacity capacity;
//     DiskStatusEnum status;
//
//     QList<TestVolumeData::SharedType> volumes;
//
//     using SharedType = QSharedPointer<TestDiskData>;
//
//     static SharedType make ()
//     {
//         return SharedType(new TestDiskData);
//     }
// };
//
// struct TestData
// {
//     QList<TestDiskData::SharedType> disks;
// }
//
// struct TestVolumeModel : VolumeModel
// {
//     TestVolumeData data;
//
//     virtual QString name () const override { return pdata->name; }
//     virtual VolumeLayoutEnum layout () const override { return pdata->layout; }
//     virtual VolumTypeEnum type () const override { return pdata->type; }
//     virtual FileSystemEnum fileSystem () const override { return pdata->fileSystem; }
//     virtual VolumeStatusEnum status () const override { return pdata->status; }
//     virtual Capacity capacity () const override { return pdata->capacity; }
//     virtual Capacity free () const override { return pdata->free; }
//     virtual bool faultTolerance () const override { return pdata->faultTolerance; }
//     virtual int overhead () const override { return pdata->overhead; }
// };
//
// struct TestDiskModel : DiskModel
// {
//     TestDiskData pdata;
//
//     virtual int volumeCount () const override;
//     virtual VolumeModel * volumeAt (int index) const override;
//     virtual QString name () const override { return pdata->name; }
//     virtual DiskTypeEnum type () const override { return pdata->type; }
//     virtual Capacity capacity () const override { return pdata->capacity; }
//     virtual DiskStatusEnum status () const override { return pdata->status; }
//
// };
//
// struct TestModel : Model
// {
//     TestData data;
//     virtual int diskCount () const override;
//     virtual DiskModel * diskAt (int index) const override;
// };
//
// template <>
// Model * requestModel<RequestInternalTestModel> ()
// {
//     ///////////////////////////////////////////////////////////////////////////
//     // Disk 0                                                                //
//     ///////////////////////////////////////////////////////////////////////////
//     auto pdisk0 = TestDiskData::make();
//     pdisk0->name     = "Disk 0";
//     pdisk0->type     = DiskTypeEnum::Basic;
//     pdisk0->capacity = Capacity(200000000ULL);
//     pdisk0->status   = DiskStatusEnum::Online;
//
//     ///////////////////////////////////////////////////////////////////////////
//     // Disk 0, Volume 0                                                      //
//     ///////////////////////////////////////////////////////////////////////////
//     auto pvol0_0 = TestVolumeData::make();
//     pvol0_0->name           = "(C:)";
//     pvol0_0->layout         = VolumeLayoutEnum::Simple;
//     pvol0_0->type           = VolumTypeEnum::Basic;
//     pvol0_0->fileSystem     = FileSystemEnum::NTFS;
//     pvol0_0->status         = VolumeStatusEnum::Healthy;
//     pvol0_0->capacity       = Capacity(100000000ULL);
//     pvol0_0->free           = Capacity( 50000000ULL);
//     pvol0_0->faultTolerance = true;
//     pvol0_0->overhead       = 10;
//
//     ///////////////////////////////////////////////////////////////////////////
//     // Disk 0, Volume 1                                                      //
//     ///////////////////////////////////////////////////////////////////////////
//     auto pvol0_1 = TestVolumeData::make();
//     pvol0_1->name           = "(D:)";
//     pvol0_1->layout         = VolumeLayoutEnum::Simple;
//     pvol0_1->type           = VolumTypeEnum::Basic;
//     pvol0_1->fileSystem     = FileSystemEnum::NTFS;
//     pvol0_1->status         = VolumeStatusEnum::Healthy;
//     pvol0_1->capacity       = Capacity(100000000ULL);
//     pvol0_1->free           = Capacity( 50000000ULL);
//     pvol0_1->faultTolerance = true;
//     pvol0_1->overhead       = 10;
//
//     ///////////////////////////////////////////////////////////////////////////
//     // Disk 0, Volume 2                                                      //
//     ///////////////////////////////////////////////////////////////////////////
//     auto pvol0_2 = TestVolumeData::make();
//     pvol0_2->name           = "(E:)";
//     pvol0_2->layout         = VolumeLayoutEnum::Simple;
//     pvol0_2->type           = VolumTypeEnum::Basic;
//     pvol0_2->fileSystem     = FileSystemEnum::NTFS;
//     pvol0_2->status         = VolumeStatusEnum::Healthy;
//     pvol0_2->capacity       = Capacity(100000000ULL);
//     pvol0_2->free           = Capacity( 50000000ULL);
//     pvol0_2->faultTolerance = true;
//     pvol0_2->overhead       = 10;
//
//     ///////////////////////////////////////////////////////////////////////////
//     // Disk 1                                                                //
//     ///////////////////////////////////////////////////////////////////////////
//     //auto pdisk1 = TestDiskData::SharedType(new TestDiskData);
//
//     ///////////////////////////////////////////////////////////////////////////
//     // Disk 2                                                                //
//     ///////////////////////////////////////////////////////////////////////////
//     //auto pdisk2 = TestDiskData::SharedType(new TestDiskData);
//
//     auto model = new TestModel;
//
//     return model;
// }
//
// template <>
// void releaseModel<RequestInternalTestModel> (Model * model)
// {
//     delete model;
// }
