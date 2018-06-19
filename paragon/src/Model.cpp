#include "Model.hpp"

static QString FileSystemEnumStrings[] = {
      QT_TR_NOOP("Unknown")
    , QT_TR_NOOP("Ext2")
    , QT_TR_NOOP("Ext3")
    , QT_TR_NOOP("Ext4")
    , QT_TR_NOOP("FAT16")
    , QT_TR_NOOP("FAT32")
    , QT_TR_NOOP("NTFS")
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
    // TODO
    int abbrCount = sizeof(CapacityUnitAbbrStrings) / sizeof(CapacityUnitAbbrStrings[0]);
    int abbrIndex = 0;

    quint64 cur = cap.value;
    quint64 prev = cur;

    while (cur > 0 && --abbrCount) {
        cur /= 1024;
        abbrIndex++;
    }

    //return QString("%1 %2").arg(prev).arg(CapacityUnitAbbrStrings[abbrIndex]);
    return QString("%1 %2").arg(cap.value).arg(CapacityUnitAbbrStrings[0]);

}
