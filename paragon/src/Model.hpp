#pragma once
#include <QString>
#include <QList>
#include <QSharedPointer>

enum struct FileSystemEnum
{
      Unknown
    , Ext2
    , Ext3
    , Ext4
    , FAT16
    , FAT32
    , NTFS
};

enum struct VolumTypeEnum
{
      Unknown
    , Basic
};

enum struct VolumeLayoutEnum
{
      Unknown
    , Simple
};

enum struct VolumeStatusEnum
{
      Unknown
    , Healthy
};

enum struct DiskTypeEnum
{
      Unknown
    , Basic
    , CD_ROM
    , DVD_ROM
};

enum struct DiskStatusEnum
{
        Unknown
      , Online
      , Offline
};

struct Capacity
{
    using type = quint64;
    quint64 value;
    Capacity (quint64 v) : value(v) {}
};

QString toString (FileSystemEnum const & value);
QString toString (VolumTypeEnum const & value);
QString toString (VolumeLayoutEnum const & value);
QString toString (VolumeStatusEnum const & value);
QString toString (DiskTypeEnum const & value);
QString toString (DiskStatusEnum const & value);
QString toString (Capacity const & value);
QString toString (bool value);

template <typename EnumT>
EnumT fromString (QString const & s);

struct Model;
struct DiskModel;

struct VolumeModel
{
    DiskModel * parentDiskModel;
    int volumeIndex;

    VolumeModel (int index, DiskModel * parent)
        : parentDiskModel(parent)
        , volumeIndex(index) {}

    int index () const { return volumeIndex; }
    int diskIndex () const;

    virtual QString name () const = 0;
    virtual VolumeLayoutEnum layout () const = 0;
    virtual VolumTypeEnum type () const = 0;
    virtual FileSystemEnum fileSystem () const = 0;
    virtual VolumeStatusEnum status () const = 0;
    virtual Capacity capacity () const = 0;
    virtual Capacity free () const = 0;
    virtual bool faultTolerance () const = 0;
    virtual int overhead () const = 0;
};

struct DiskModel
{
    Model * parentModel;
    int diskIndex;

    DiskModel (int index, Model * parent)
            : parentModel(parent)
            , diskIndex(index)
    {}

    int index () const { return diskIndex; }

    virtual int volumeCount () const = 0;
    virtual VolumeModel * volumeAt (int index) const = 0;
    virtual QString name () const = 0;
    virtual DiskTypeEnum type () const = 0;
    virtual Capacity capacity () const = 0;
    virtual DiskStatusEnum status () const = 0;

    /**
     * @return Free capacity
     */
    Capacity free () const;
};

struct Model
{
    virtual int diskCount () const = 0;
    virtual DiskModel * diskAt (int index) const = 0;

    /**
     * @return Capacity of the lagest disk
     */
    Capacity maxCapacity () const;
};

inline int VolumeModel::diskIndex () const
{
    return parentDiskModel->index();
}

Model * requestModel ();
void releaseModel (Model * model);
