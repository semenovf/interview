#include "Model.hpp"

static QString FileSystemEnumStrings[] = {
      QT_TR_NOOP("Unknown")
    , QT_TR_NOOP("Ext2")
    , QT_TR_NOOP("Ext3")
    , QT_TR_NOOP("Ext4")
    , QT_TR_NOOP("FAT16")
    , QT_TR_NOOP("FAT32")
    , QT_TR_NOOP("NTFS")
    , QT_TR_NOOP("CDFS")
};

static QString VolumTypeEnumStrings[] = {
      QT_TR_NOOP("Unknown")
    , QT_TR_NOOP("Basic")
};

static QString VolumeLayoutEnumStrings[] = {
      QT_TR_NOOP("Unknown")
    , QT_TR_NOOP("Simple")
};

static QString VolumeStatusEnumStrings[] = {
      QT_TR_NOOP("Unknown")
    , QT_TR_NOOP("Healthy")
    , QT_TR_NOOP("Failed")
};

static QString DiskTypeEnumStrings[] = {
      QT_TR_NOOP("Unknown")
    , QT_TR_NOOP("Basic")
    , QT_TR_NOOP("CD_ROM")
    , QT_TR_NOOP("DVD_ROM")
};

static QString DiskStatusEnumStrings[] = {
      QT_TR_NOOP("Unknown")
    , QT_TR_NOOP("Online")
    , QT_TR_NOOP("Offline")
};

template <typename EnumT>
inline QString __toString (EnumT const & value, QString strings[])
{
    int index = static_cast<int>(value);
    return strings[index];
}

QString toString (FileSystemEnum const & value)
{
    return __toString<>(value, FileSystemEnumStrings);
}

QString toString (VolumTypeEnum const & value)
{
    return __toString<>(value, VolumTypeEnumStrings);
}

QString toString (VolumeLayoutEnum const & value)
{
    return __toString<>(value, VolumeLayoutEnumStrings);
}

QString toString (VolumeStatusEnum const & value)
{
    return __toString<>(value, VolumeStatusEnumStrings);
}

QString toString (DiskTypeEnum const & value)
{
    return __toString<>(value, DiskTypeEnumStrings);
}

QString toString (DiskStatusEnum const & value)
{
    return __toString<>(value, DiskStatusEnumStrings);
}

static QString CapacityUnitAbbrStrings[] =
{
      QT_TR_NOOP("b")
    , QT_TR_NOOP("Kb")
    , QT_TR_NOOP("Mb")
    , QT_TR_NOOP("Gb")
    , QT_TR_NOOP("Tb")
};

QString toString (Capacity const & cap)
{
    Capacity::type n = cap.value;
    int abbrCount = sizeof(CapacityUnitAbbrStrings) / sizeof(CapacityUnitAbbrStrings[0]);
    int abbrIndex = 0;

    for (; abbrIndex < abbrCount - 1 && n >= 1000; abbrIndex++)
        n /= 1000;

    return QString("%1 %2").arg(n).arg(CapacityUnitAbbrStrings[abbrIndex]);
}

QString toString (bool value)
{
    return value ? QT_TR_NOOP("Yes") : QT_TR_NOOP("No");
}

template <>
FileSystemEnum fromString<FileSystemEnum> (QString const & s)
{
    QString ls = s.toLower();
    if (ls == QT_TR_NOOP("ext2"))  return FileSystemEnum::Ext2;
    if (ls == QT_TR_NOOP("ext3"))  return FileSystemEnum::Ext3;
    if (ls == QT_TR_NOOP("ext4"))  return FileSystemEnum::Ext4;
    if (ls == QT_TR_NOOP("fat16")) return FileSystemEnum::FAT16;
    if (ls == QT_TR_NOOP("fat32")) return FileSystemEnum::FAT32;
    if (ls == QT_TR_NOOP("ntfs"))  return FileSystemEnum::NTFS;
    if (ls == QT_TR_NOOP("cdfs"))  return FileSystemEnum::CDFS;
    return FileSystemEnum::Unknown;
}

template <>
VolumTypeEnum fromString<VolumTypeEnum> (QString const & s)
{
    QString ls = s.toLower();
    if (ls == QT_TR_NOOP("basic"))  return VolumTypeEnum::Basic;
    return VolumTypeEnum::Unknown;
}

template <>
VolumeLayoutEnum fromString<VolumeLayoutEnum> (QString const & s)
{
    QString ls = s.toLower();
    if (ls == QT_TR_NOOP("simple"))  return VolumeLayoutEnum::Simple;
    return VolumeLayoutEnum::Unknown;
}

template <>
VolumeStatusEnum fromString<VolumeStatusEnum> (QString const & s)
{
    QString ls = s.toLower();
    if (ls == QT_TR_NOOP("healthy")) return VolumeStatusEnum::Healthy;
    if (ls == QT_TR_NOOP("failed"))  return VolumeStatusEnum::Failed;
    return VolumeStatusEnum::Unknown;
}

template <>
DiskTypeEnum fromString<DiskTypeEnum> (QString const & s)
{
    QString ls = s.toLower();
    if (ls == QT_TR_NOOP("basic"))   return DiskTypeEnum::Basic;
    if (ls == QT_TR_NOOP("cd_rom"))  return DiskTypeEnum::CD_ROM;
    if (ls == QT_TR_NOOP("cd-rom"))  return DiskTypeEnum::CD_ROM;
    if (ls == QT_TR_NOOP("dvd_rom")) return DiskTypeEnum::DVD_ROM;
    if (ls == QT_TR_NOOP("dvd-rom")) return DiskTypeEnum::DVD_ROM;
    return DiskTypeEnum::Unknown;
}

template <>
DiskStatusEnum fromString<DiskStatusEnum> (QString const & s)
{
    QString ls = s.toLower();
    if (ls == QT_TR_NOOP("online"))  return DiskStatusEnum::Online;
    if (ls == QT_TR_NOOP("offline")) return DiskStatusEnum::Offline;
    return DiskStatusEnum::Unknown;
}

template <>
bool fromString<bool> (QString const & s)
{
    QString ls = s.toLower();
    if (ls == QT_TR_NOOP("yes") || ls == QT_TR_NOOP("true")) return true;
    return false;
}

Capacity DiskModel::free () const
{
    int count = volumeCount();
    Capacity result(0);

    for (int i = 0; i < count; i++) {
        auto volumeModel = volumeAt(i);
        result.value += volumeModel->free().value;
    }
    return result;
}

Capacity Model::maxCapacity () const
{
    int count = diskCount();
    Capacity result(0);

    for (int i = 0; i < count; i++) {
        auto diskModel = diskAt(i);
        result = Capacity(qMax(result.value, diskModel->capacity().value));
    }
    return result;
}
