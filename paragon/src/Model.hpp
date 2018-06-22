#pragma once
#include <QString>
#include <QList>
#include <QSharedPointer>

///////////////////////////////////////////////////////////////////////////////
// Файл содержит описание модели (интерфейс) дисковой подсистемы (далее М).
// Также здесь дано описание типов (в том числе словарных) и обьявлены
// утилиты для их обработки.
//
// В данном проекты имеются две реализации М: TestModel и TestXmlModel.
// TestModel - встроенная статическая реализация М.
// TestXmlModel - реализация М, основанная хранении параметров дисков и томов
// во внешнем файле в формате XML.
//
// Пример файла конфигурации:
//-----------------------------------------------------------------------------
// <?xml version="1.0" encoding="utf-8"?>
// <disks>
//    <disk name="Disk 0" capacity="1000" type="Basic" status="Online">
//         <volume name="C:" capacity="400" free="150" layout="Simple" type="Basic" file-system="NTFS" status="Healthy" fault-tolerance="yes" overhead="0" />
//         <volume name="D:" capacity="200" free="100" layout="Simple" type="Basic" file-system="NTFS" status="Healthy" fault-tolerance="yes" overhead="0" />
//         <volume name="E:" capacity="400" free="400" layout="Simple" type="Basic" file-system="NTFS" status="Healthy" fault-tolerance="yes" overhead="0" />
//     </disk>
//
//     <disk name="CD-ROM 0" capacity="600" type="Basic" status="Online">
//         <volume name="O:" capacity="650" free="650" layout="Simple" type="Basic" file-system="CDFS" status="Healthy" fault-tolerance="yes" overhead="10" />
//     </disk>
//
//     <disk name="Disk 1" capacity="500" type="Basic" status="Online">
//         <volume name="F:" capacity="100" free="100" layout="Simple" type="Basic" file-system="FAT32" status="Healthy" fault-tolerance="yes" overhead="0" />
//         <volume name="G:" capacity="400" free="250" layout="Simple" type="Basic" file-system="FAT16" status="Healthy" fault-tolerance="yes" overhead="0" />
//     </disk>
// </disks>
//-----------------------------------------------------------------------------
//
// Тип и допустимые значения атрибутов элемента 'disk':
//=============================================================================
// имя аттрибута     тип          диапазон значений   примечание
//-----------------------------------------------------------------------------
// name              строка
//-----------------------------------------------------------------------------
// capacity          целое        >= 0
//-----------------------------------------------------------------------------
// type              строка       basic               регистронезависимая строка
//                                | cd_rom | cd-rom
//                                | dvd_rom | dvd-rom
//-----------------------------------------------------------------------------
// status            строка       online | offline    регистронезависимая строка
//=============================================================================
//
// Тип и допустимые значения атрибутов элемента 'volume':
//=============================================================================
// имя аттрибута     тип          диапазон значений   примечание
//-----------------------------------------------------------------------------
// name              строка
//-----------------------------------------------------------------------------
// capacity          целое        >= 0
//-----------------------------------------------------------------------------
// free capacity     целое        >= 0
//-----------------------------------------------------------------------------
// layout            строка       simple              регистронезависимая строка
//-----------------------------------------------------------------------------
// type              строка       basic               регистронезависимая строка
//-----------------------------------------------------------------------------
// file-system       строка       ext2 | ext3 | ext4  регистронезависимая строка
//                                | fat16 | fat32
//                                | ntfs | cdfs
//-----------------------------------------------------------------------------
// status            строка       healthy | failed    регистронезависимая строка
//-----------------------------------------------------------------------------
// fault-tolerance   логический   yes | true          регистронезависимая строка
//                                | no | false
//-----------------------------------------------------------------------------
// overhead          целое        [0, 100]
//=============================================================================
//
// Примечение: в текущей реализации нет проверки на соответствие размеров томов и диска
///////////////////////////////////////////////////////////////////////////////


enum struct FileSystemEnum
{
      Unknown
    , Ext2
    , Ext3
    , Ext4
    , FAT16
    , FAT32
    , NTFS
    , CDFS
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
    , Failed
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

    /**
     * @return Индекс тома на диске.
     */
    int index () const { return volumeIndex; }

    /**
     * @return Индекс диска , которому принадлежит данный том.
     */
    int diskIndex () const;

    /**
     * @return Имя тома.
     */
    virtual QString name () const = 0;

    /**
     * @return Расположение тома.
     */
    virtual VolumeLayoutEnum layout () const = 0;

    /**
     * @return Тип тома.
     */
    virtual VolumTypeEnum type () const = 0;

    /**
     * @return Файлова система тома.
     */
    virtual FileSystemEnum fileSystem () const = 0;

    /**
     * @return Состояние тома.
     */
    virtual VolumeStatusEnum status () const = 0;

    /**
     * @return Емкость тома в байтах.
     */
    virtual Capacity capacity () const = 0;

    /**
     * @return Свободное пространство тома в байтах.
     */
    virtual Capacity free () const = 0;

    /**
     * @return Отказоустойчивость тома.
     */
    virtual bool faultTolerance () const = 0;

    /**
     * @return Накладные расходы тома.
     */
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

    /**
     * @return Индекс диска.
     */
    int index () const { return diskIndex; }

    /**
     * @return Количество томаов на диске.
     */
    virtual int volumeCount () const = 0;

    /**
     * @return Том по индексу @a index.
     */
    virtual VolumeModel * volumeAt (int index) const = 0;

    /**
     * @return Имя диска.
     */
    virtual QString name () const = 0;

    /**
     * @return Тип диска.
     */
    virtual DiskTypeEnum type () const = 0;

    /**
     * @return Емкость диска в байтах.
     */
    virtual Capacity capacity () const = 0;

    /**
     * @return Состояние диска.
     */
    virtual DiskStatusEnum status () const = 0;

    /**
     * @return Свободное пространство на диске.
     * @note Определяется путем сложения размеров
     *       свободного пространства всех томов диска.
     */
    Capacity free () const;
};

struct Model
{
    /**
     * @return Количество дисков.
     */
    virtual int diskCount () const = 0;

    /**
     * @return Диск по индексу @arg index.
     */
    virtual DiskModel * diskAt (int index) const = 0;

    /**
     * @return Максимальная емкость среди всех дисков.
     */
    Capacity maxCapacity () const;
};

inline int VolumeModel::diskIndex () const
{
    return parentDiskModel->index();
}

/**
 * @brief Запрос реализации модели дисковой подсистемы.
 * @param errmsg Указатель на строку для записи сообщения об ошибке.
 * @return Указатель на модель в случае успешного запроса или
 *         @c nullptr в случае неудачи. В последнем случае
 *         @a *errmsg (если errmsg != nullptr) будет содержать
 *         сообщение об ошибке.
 */
Model * requestModel (QString * errmsg);

/**
 * @brief Освобождает ресурсы, выделенные для модели.
 * @param model Указатель на модель, возвращенный ранее
 *        функцией requestModel().
 */
void releaseModel (Model * model);
