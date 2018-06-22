#include <QApplication>
#include <QFile>
#include <QList>
#include <QDomDocument>
#include "Exception.hpp"
#include "Model.hpp"

#define ASSERT_EXPECT_DISK_ATTR(attrName)                                      \
        ASSERT_X(elem.hasAttribute(attrName)                                   \
                , QString("Expected attribute `%1` for %2")                    \
                    .arg(attrName).arg(diskIdStr(diskIndex)));

#define ASSERT_EXPECT_VOLUME_ATTR(attrName)                                    \
        ASSERT_X(elem.hasAttribute(attrName)                                   \
                , QString("Expected attribute `%1` for %2")                    \
                    .arg(attrName).arg(volumeIdStr(diskIndex(), volumeIndex)));

inline QString diskIdStr (int diskIndex)
{
    return QString("disk[") + QString::number(diskIndex) + "]";
}

inline QString volumeIdStr (int diskIndex, int volumeIndex)
{
    return QString("disk[") + QString::number(diskIndex) + "]"
            + ":volume[" + QString::number(volumeIndex) + "]";
}

struct TestVolumeModel : VolumeModel
{
    QDomElement elem;
    TestVolumeModel (int index, DiskModel * parent, QDomElement e)
        : VolumeModel(index, parent)
        , elem(e) {}

    virtual QString name () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("name");
        return elem.attribute("name");
    }

    virtual VolumeLayoutEnum layout () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("layout");
        return fromString<VolumeLayoutEnum>(elem.attribute("layout"));
    }

    virtual VolumTypeEnum type () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("type");
        return fromString<VolumTypeEnum>(elem.attribute("type"));
    }

    virtual FileSystemEnum fileSystem () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("file-system");
        return fromString<FileSystemEnum>(elem.attribute("file-system"));
    }

    virtual VolumeStatusEnum status () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("status");
        return fromString<VolumeStatusEnum>(elem.attribute("status"));
    }

    virtual Capacity capacity () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("capacity");

        QString capacityStr = elem.attribute("capacity");
        bool ok;
        qulonglong n = capacityStr.toULongLong(& ok);

        ASSERT_X(ok, QString("Bad `capacity` for ") + volumeIdStr(diskIndex(), volumeIndex));

        return Capacity(n);
    }

    virtual Capacity free () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("free");

        QString capacityStr = elem.attribute("free");
        bool ok;
        qulonglong n = capacityStr.toULongLong(& ok);

        ASSERT_X(ok, QString("Bad `free` capacity for ") + volumeIdStr(diskIndex(), volumeIndex));

        return Capacity(n);
    }

    virtual bool faultTolerance () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("fault-tolerance");
        return fromString<bool>(elem.attribute("fault-tolerance"));
    }

    virtual int overhead () const override
    {
        ASSERT_EXPECT_VOLUME_ATTR("overhead");

        QString overheadStr = elem.attribute("overhead");
        bool ok;
        int n = overheadStr.toInt(& ok);

        ASSERT_X(ok, QString("Bad `overhead` for ")
                + volumeIdStr(diskIndex(), volumeIndex));
        ASSERT_X(n >= 0 && n <= 100
                , QString("`overhead` is out of range [0,100] for ")
                        + volumeIdStr(diskIndex(), volumeIndex));

        return n;
    }
};

struct TestDiskModel : DiskModel
{
    QDomElement elem;
    QList<TestVolumeModel *> volumes;

    TestDiskModel (int index, Model * parent, QDomElement e)
        : DiskModel(index, parent)
        , elem(e)
    {
        QDomNodeList list = elem.elementsByTagName("volume");

        auto count = list.count();

        for (int i = 0; i < count; i++) {
            QDomNode node = list.at(i);
            volumes.append(new TestVolumeModel(i, this, node.toElement()));
        }
    }

    ~TestDiskModel ()
    {
        for (auto && volume: volumes)
            delete volume;
    }

    virtual int volumeCount () const override
    {
        return volumes.size();
    }

    virtual VolumeModel * volumeAt (int index) const override
    {
        ASSERT_X(index >= 0 && index < volumes.size()
                , QString("Out of range volume request for ") + diskIdStr(diskIndex));
        return volumes[index];
    }

    virtual QString name () const override
    {
        ASSERT_EXPECT_DISK_ATTR("name");
        return elem.attribute("name");
    }

    virtual DiskTypeEnum type () const override
    {
        ASSERT_EXPECT_DISK_ATTR("type");
        return fromString<DiskTypeEnum>(elem.attribute("type"));
    }

    virtual Capacity capacity () const override
    {
        ASSERT_EXPECT_DISK_ATTR("capacity");

        QString capacityStr = elem.attribute("capacity");
        bool ok;
        qulonglong n = capacityStr.toULongLong(& ok);

        ASSERT_X(ok, QString("Bad `capacity` for ") + diskIdStr(diskIndex));

        return Capacity(n);
    }

    virtual DiskStatusEnum status () const override
    {
        ASSERT_EXPECT_DISK_ATTR("status");
        return fromString<DiskStatusEnum>(elem.attribute("status"));
    }
};

struct TestModel : Model
{
    QDomDocument doc;
    QList<TestDiskModel *> disks;

    TestModel (QDomDocument d) : doc(d)
    {
        QDomNodeList list = d.elementsByTagName("disk");
        auto count = list.count();

        for (int i = 0; i < count; i++) {
            QDomNode node = list.at(i);
            disks.append(new TestDiskModel(i, this, node.toElement()));
        }
    }

    ~TestModel ()
    {
        for (auto && disk: disks)
            delete disk;
    }

    virtual int diskCount () const override
    {
        return disks.size();
    }

    virtual DiskModel * diskAt (int index) const override
    {
        ASSERT_X(index >= 0 && index < disks.size(), QString("Out of range disk request"));
        return disks[index];
    }
};

Model * requestModel (QString * errmsg)
{
    QString appName = qApp->applicationName();
    QString fileName = QString("./%1.xml").arg(appName);
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return nullptr;

    QDomDocument doc;

    QString errorMsg;
    int errorLine = 0;
    int errorColumn = 0;

    if (!doc.setContent(& file, & errorMsg, & errorLine, & errorColumn)) {
        if (errmsg) {
            *errmsg = "Failed to set content from XML: "
                + errorMsg
                + " at line " + QString::number(errorLine);
        }
        return nullptr;
    }

    file.close();

    return new TestModel(doc);
}

void releaseModel (Model * model)
{
    delete model;
}


