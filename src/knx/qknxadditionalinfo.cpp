/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxadditionalinfo.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxAdditionalInfo

    \inmodule QtKnx
    \brief The QKnxAdditionalInfo class represents the additional information
    that can be placed inside a KNX cEMI frame.
*/

/*!
    \enum QKnxAdditionalInfo::Type

    \omitvalue Reserved
    \value PlMediumInformation          Domain Address used by PL medium.
    \value RfMediumInformation          RF-Info byte (formerly named
                                        RF-Ctrl) and KNX Serial Number/DoA
                                        and Data Link Layer Frame Number (LFN).
    \value BusmonitorStatusInfo         Busmonitor Error Flags.
    \value TimestampRelative            Relative timestamp; e.g. for L_Raw.ind.
    \value TimeDelayUntilSending        Time delay; e.g. for L_Raw.req.
    \value ExtendedRelativeTimestamp    Device independent time stamp, e.g.
                                        for L_Raw.ind or L_Busmon.ind.
    \value BiBatInformation             Contains b7-b4 of the RF KNX-Ctrl
                                        field and BiBat Block-number.
    \value RfMultiInformation           RF Multi frequency, call channel and
                                        Fast Ack number.
    \value PreambleAndPostamble         Preamble and postamble length.
    \value RfFastAckInformation         Status and information about each
                                        expected number of Fast Ack (N)
    \value ManufactorSpecificData       Manufacturer specific data, including
                                        manufacturer ID (2 byte) and
                                        Subfunction ID (1 byte).
    \omitvalue EscCode
*/

/*!
    Returns the additional info \l Type.
*/
QKnxAdditionalInfo::Type QKnxAdditionalInfo::type() const
{
    return QKnxAdditionalInfo::Type(byte(0));
}

/*!
    \fn QKnxAdditionalInfo::QKnxAdditionalInfo()

    Constructs an new, empty, invalid additional info object.
*/

/*!
    Constructs an new additional info object and sets its \l Type to \a type and
    data to \a data.
*/
QKnxAdditionalInfo::QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QByteArray &data)
{
    setByte(0, quint8(type));
    setByte(1, quint8(data.size()));
    appendBytes(data);
    if (!isValid()) resize(0);
}

/*!
    Constructs an new additional info object and sets its \l Type to \a type and
    data to \a data.
*/
QKnxAdditionalInfo::QKnxAdditionalInfo(QKnxAdditionalInfo::Type type, const QVector<quint8> &data)
{
    setByte(0, quint8(type));
    setByte(1, quint8(data.size()));
    appendBytes(data);
    if (!isValid()) resize(0);
}

/*!
    \overload

    Returns \c true if this is a valid additional info object; \c false
    otherwise.
*/
bool QKnxAdditionalInfo::isValid() const
{
    if (size() > 254)
        return false;

    const auto type = QKnxAdditionalInfo::Type(byte(0));
    switch (type) {
    case QKnxAdditionalInfo::Type::PlMediumInformation:
    case QKnxAdditionalInfo::Type::RfMediumInformation:
    case QKnxAdditionalInfo::Type::BusmonitorStatusInfo:
    case QKnxAdditionalInfo::Type::TimestampRelative:
    case QKnxAdditionalInfo::Type::TimeDelayUntilSending:
    case QKnxAdditionalInfo::Type::ExtendedRelativeTimestamp:
    case QKnxAdditionalInfo::Type::BiBatInformation:
    case QKnxAdditionalInfo::Type::RfMultiInformation:
    case QKnxAdditionalInfo::Type::PreambleAndPostamble:
        return qint16(dataSize()) == expectedDataSize(type);
    case QKnxAdditionalInfo::Type::RfFastAckInformation:
        return (qint16(dataSize()) >= expectedDataSize(type)) && ((qint16(dataSize()) % 2) == 0);
    case QKnxAdditionalInfo::Type::ManufactorSpecificData:
        return qint16(dataSize()) >= expectedDataSize(type);
    default:
        break;
    }
    return false;
}

/*!
    Returns the number of bytes representing the additional info, excluding the
    byte for \l Type id and the byte for length information.
*/
quint8 QKnxAdditionalInfo::dataSize() const
{
    if (size() >= 2)
        return size() - 2;
    return 0;
}

/*!
    Returns the number of expected bytes for \l Type id \a type. The additional
    \c bool parameter \a isFixedSize is set to \c true if the \a type expects
    a fixed size of bytes; otherwise to false. If the \a type is unknown, the
    function will return a negative value.

    Know types of variable size are:
    \list
        \li \l QKnxAdditionalInfo::RfFastAckInformation - a multiple of two
               bytes, two bytes minimum.
        \li \l QKnxAdditionalInfo::ManufactorSpecificData - three bytes minimum.
    \endlist
*/
qint32 QKnxAdditionalInfo::expectedDataSize(QKnxAdditionalInfo::Type type, bool *isFixedSize)
{
    static const std::array<int, 0x100> table = []() {
        std::array<int, 0x100> table; table.fill(-1);
        table[int(QKnxAdditionalInfo::Type::PlMediumInformation)] = 2;
        table[int(QKnxAdditionalInfo::Type::RfMediumInformation)] = 8;
        table[int(QKnxAdditionalInfo::Type::BusmonitorStatusInfo)] = 1;
        table[int(QKnxAdditionalInfo::Type::TimestampRelative)] = 2;
        table[int(QKnxAdditionalInfo::Type::TimeDelayUntilSending)] = 4;
        table[int(QKnxAdditionalInfo::Type::ExtendedRelativeTimestamp)] = 4;
        table[int(QKnxAdditionalInfo::Type::BiBatInformation)] = 2;
        table[int(QKnxAdditionalInfo::Type::RfMultiInformation)] = 4;
        table[int(QKnxAdditionalInfo::Type::PreambleAndPostamble)] = 3;
        table[int(QKnxAdditionalInfo::Type::RfFastAckInformation)] = 2;
        table[int(QKnxAdditionalInfo::Type::ManufactorSpecificData)] = 3;
        return table;
    }();
    if (isFixedSize)
        *isFixedSize = (type < QKnxAdditionalInfo::Type::RfFastAckInformation);
    return table[int(type)];
}

/*!
    Returns the additional info's type, length and the data as string. Type,
    length and data are formatted in hexadecimal notation. If the additional
    info is invalid, an empty string is returned.
*/
QString QKnxAdditionalInfo::toString() const
{
    if (!isValid())
        return QString();

    QString data;
    for (quint8 byte : ref(2))
        data += QStringLiteral("0x%1, ").arg(byte, 2, 16, QLatin1Char('0'));
    data.chop(2);

    return QStringLiteral("Type { 0x%1 }, Size { 0x%2 }, Data { %3 }")
        .arg(static_cast<quint8> (byte(0)), 2, 16, QLatin1Char('0'))
        .arg(byte(1), 2, 16, QLatin1Char('0')).arg(data);
}

/*!
    \fn auto QKnxAdditionalInfo::bytes() const

    Returns the additional info as range of bytes if the information is valid;
    otherwise an empty vector. The vector includes the type id, the size of the
    actual data and the data itself.

    \note Only QByteArray and QVector<quint8> are supported as return type.
*/

/*!
    \relates QKnxAdditionalInfo

    Writes the KNX cEMI frame's additional \a info to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxAdditionalInfo &info)
{
    QDebugStateSaver _(debug);
    if (info.isValid()) {
        QDebug &dbg = debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2)
            << qSetPadChar(QLatin1Char('0'));
        const auto rawData = info.bytes();
        for (quint8 byte : qAsConst(rawData))
            dbg << byte;
    } else {
         debug.nospace().noquote() << "0x1nv4l1d";
    }
    return debug;
}

/*!
    \relates QKnxAdditionalInfo

    Reads the KNX cEMI frame's additional \a info from the \a stream and returns
    a reference to the \a stream.
*/
QDataStream &operator>>(QDataStream &stream, QKnxAdditionalInfo &info)
{
    quint8 type, size;
    stream >> type >> size;

    QByteArray ba(size, Qt::Uninitialized);
    stream.readRawData(ba.data(), size);
    info = QKnxAdditionalInfo(QKnxAdditionalInfo::Type(type), ba);
    return stream;
}

/*!
    \relates QKnxAdditionalInfo

    Writes the KNX cEMI frame's additional \a info to the \a stream and returns
    a reference to the \a stream.
*/
QDataStream &operator<<(QDataStream &stream, const QKnxAdditionalInfo &info)
{
    if (!info.isValid())
        return stream;
    for (quint8 byte : info.bytes())
        stream << static_cast<quint8> (byte);
    return stream;
}

QT_END_NAMESPACE
