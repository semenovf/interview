#include <QApplication>
#include <QFile>
#include <QList>
#include <QDomDocument>
#include "Model.hpp"

#include <QDebug>

struct TestVolumeModel : VolumeModel
{
    QDomElement elem;
    TestVolumeModel (int index, DiskModel * parent, QDomElement e)
        : VolumeModel(index, parent)
        , elem(e) {}

    virtual QString name () const override
    {
        Q_ASSERT(elem.hasAttribute("name"));
        return elem.attribute("name");
    }

    virtual VolumeLayoutEnum layout () const override
    {
        Q_ASSERT(elem.hasAttribute("layout"));
        return fromString<VolumeLayoutEnum>(elem.attribute("layout"));
    }

    virtual VolumTypeEnum type () const override
    {
        Q_ASSERT(elem.hasAttribute("type"));
        return fromString<VolumTypeEnum>(elem.attribute("type"));
    }

    virtual FileSystemEnum fileSystem () const override
    {
        Q_ASSERT(elem.hasAttribute("file-system"));
        return fromString<FileSystemEnum>(elem.attribute("file-system"));
    }

    virtual VolumeStatusEnum status () const override
    {
        Q_ASSERT(elem.hasAttribute("status"));
        return fromString<VolumeStatusEnum>(elem.attribute("status"));
    }

    virtual Capacity capacity () const override
    {
        Q_ASSERT(elem.hasAttribute("capacity"));

        QString capacityStr = elem.attribute("capacity");
        bool ok;
        qulonglong n = capacityStr.toULongLong(& ok);

        Q_ASSERT(ok);

        return Capacity(n);
    }

    virtual Capacity free () const override
    {
        Q_ASSERT(elem.hasAttribute("free"));

        QString capacityStr = elem.attribute("free");
        bool ok;
        qulonglong n = capacityStr.toULongLong(& ok);

        Q_ASSERT(ok);

        return Capacity(n);
    }

    virtual bool faultTolerance () const override
    {
        Q_ASSERT(elem.hasAttribute("fault-tolerance"));
        return fromString<bool>(elem.attribute("fault-tolerance"));
    }

    virtual int overhead () const override
    {
        Q_ASSERT(elem.hasAttribute("overhead"));

        QString overheadStr = elem.attribute("overhead");
        bool ok;
        int n = overheadStr.toInt(& ok);

        Q_ASSERT(ok);
        Q_ASSERT(n >= 0 && n <= 100);

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
        Q_ASSERT(index >= 0 && index < volumes.size());
        return volumes[index];
    }

    virtual QString name () const override
    {
        Q_ASSERT(elem.hasAttribute("name"));
        return elem.attribute("name");
    }

    virtual DiskTypeEnum type () const override
    {
        Q_ASSERT(elem.hasAttribute("type"));
        return fromString<DiskTypeEnum>(elem.attribute("type"));
    }

    virtual Capacity capacity () const override
    {
        Q_ASSERT(elem.hasAttribute("capacity"));

        QString capacityStr = elem.attribute("capacity");
        bool ok;
        qulonglong n = capacityStr.toULongLong(& ok);

        Q_ASSERT(ok);

        return Capacity(n);
    }

    virtual DiskStatusEnum status () const override
    {
        Q_ASSERT(elem.hasAttribute("status"));
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
        Q_ASSERT(index >= 0 && index < disks.size());
        return disks[index];
    }
};

Model * requestModel ()
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
        qWarning() << "ERROR: Failed to set content from XML: "
                << errorMsg
                << " at line " << errorLine
                << " and column " << errorColumn;
        return nullptr;
    }

    file.close();

    return new TestModel(doc);
}

void releaseModel (Model * model)
{
    delete model;
}


