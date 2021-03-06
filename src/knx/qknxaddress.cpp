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

#include "qknxaddress.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnxAddress

    \inmodule QtKnx
    \brief The QKnxAddress class represents a 16 bit KNX address value.

    KNX is a fully distributed network, which accommodates up to 65,536 devices
    in a 16-bit individual address space. Concrete address types are \l Group
    and \l Individual.
*/

/*!
    \enum QKnxAddress::Type
    This enum describes the supported KNX address types.

    \value Group        The Group address shall be a 2-byte value and does not
                        need to be unique. A device may have more than one Group
                        address.
    \value Individual   The Individual address shall be a 2-byte value and
                        needs to be unique within a KNX installation.
*/

/*!
    \enum QKnxAddress::Notation
    This enum describes the supported KNX address human-readable representation.

    \value TwoLevel     The visual representation will be main group and sub
                        group, separated by a forward slash.
    \value ThreeLevel   The visual representation for \l Group addresses will
                        be main, middle, and sub group separated by a forward
                        slash, while \l Individual addresses will be formatted
                        as area, line, and sequential number, separated by a
                        dot.

    \note Only \l Group addresses support 2-level notation as of now.
*/

/*!
    Returns the address type.
*/
QKnxAddress::Type QKnxAddress::type() const
{
    return m_type;
}

/*!
    \fn QKnxAddress::QKnxAddress()

    Constructs an new, invalid KNX address object. The formatting is set to
    3-level notation.
*/

/*!
    Creates a KNX address from a 16-bit \a address value. The type of the
    address is specified by \a type. The value of \a address should be in the
    range \c 0 to \c 65535. Hexadecimal, octal, and decimal notation are
    supported. The formatting is set to 3-level notation.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, quint16 address)
{
    if (type != QKnxAddress::Type::Group && type != QKnxAddress::Type::Individual)
        return;

    m_type = type;
    m_address = address;
}

/*!
    Creates a KNX address from a string \a address representation. The type of
    the address is specified by \a type. Depending on parsed sections, the
    formatting is set to 2 or 3-level notation. If the address or parts begin
    with "0x", hexadecimal notation is assumed; if the address begins with "0",
    octal notation is assumed; otherwise decimal notation.

    QKnxAddress can be in one of the following formats:
    \list
        \li 2-level notation, like 1/2000, for \l Group addresses.
        \li 3-level notation, like 1/1/1 for \l Group or 1.1.1 for \l Individual
            addresses.
        \li Number in the range 0 to 65535 without separator for \l Group and
            \l Individual addresses.
    \endlist

    \note For \l Group address 2-level notation, the value of the main group
    should be in the range \c 0 to \c 31 and the sub group value in the range
    \c 0 to \c 2047.

    \note For \l Group address 3-level notation, the value of the main group
    should be in the range \c 0 to \c 31, the value of middle group in the range
    \c 0 to \c 7, and the sub group value in the range \c 0 to \c 255.

    \note For \l Individual address 3-level notation, the value of area should
    be in the range \c 0 to \c 15, the value of line in the range \c 0 to \c 15,
    and the sequential number value in the range \c 0 to \c 255.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, const QString &address)
{
    const bool dots = address.contains(QLatin1Char('.'));
    const bool slashes = address.contains(QLatin1Char('/'));
    if (dots && slashes)
        return;

    QVector<quint16> sections;
    const auto convert = [&sections](const QVector<QStringRef> &in) -> bool {
        for (const QStringRef &section : qAsConst(in)) {
            bool ok = false;
            const quint16 value = section.toUShort(&ok, 0);
            if (!ok)
                return false;
            sections.append(value);
        }
        return in.size() == sections.size();
    };

    if (slashes && type == QKnxAddress::Type::Group) {
        if (!convert(address.splitRef(QLatin1Char('/'))))
            return;
        if (sections.count() == 2)
            *this = QKnxAddress(type, sections[0], nullptr, sections[1]);
        if (sections.count() == 3)
            *this = QKnxAddress(type, sections[0], &sections[1], sections[2]);
    }
    if (dots && type == QKnxAddress::Type::Individual) {
        if (convert(address.splitRef(QLatin1Char('.'))) && sections.count() == 3)
            *this = QKnxAddress(type, sections[0], &sections[1], sections[2]);
    }
    if (type == QKnxAddress::Type::Group || type == QKnxAddress::Type::Individual) {
        if (convert({ address.midRef(0) }) && sections.count() == 1) {
            m_type = type;
            m_address = sections[0];
        }
    }
}

/*!
    Creates a KNX address from the first two bytes of the \a address byte
    array. The type of the address is specified by \a type. Hexadecimal, octal,
    and decimal notation are supported.

    \note The byte array must contain at least two elements.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, const QByteArray &address)
{
    if (address.size() < 2)
        return;
    if (type != QKnxAddress::Type::Group && type != QKnxAddress::Type::Individual)
        return;

    m_type = type;
    m_address = QKnxUtils::QUint16::fromBytes(address);
}

/*!
    Creates a KNX address from the first two bytes of the \a address vector.
    The type of the address is specified by \a type. Hexadecimal, octal, and
    decimal notation are supported.

    \note The vector must contain at least two elements.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, const QVector<quint8> &address)
{
    if (address.size() < 2)
        return;
    if (type != QKnxAddress::Type::Group && type != QKnxAddress::Type::Individual)
        return;

    m_type = type;
    m_address = QKnxUtils::QUint16::fromBytes(address);
}

/*!
    Creates a KNX \l Group address from the 2-level notation. The value of
    \a mainGroup should be in the range \c 0 to \c 31 and the \a subGroup
    value in the range \c 0 to \c 2047. Hexadecimal, octal, and decimal
    notation are supported.
*/
QKnxAddress QKnxAddress::createGroup(quint8 mainGroup, quint16 subGroup)
{
    return QKnxAddress(QKnxAddress::Type::Group, mainGroup, nullptr, subGroup);
}

/*!
    Creates a KNX \l Group address from the 3-level notation. The value of
    \a mainGroup should be in the range \c 0 to \c 31, the value of
    \a middleGroup in the range \c 0 to \c 7, and the \a subGroup value in the
    range \c 0 to \c 255. Hexadecimal, octal, and decimal notation are supported.
*/
QKnxAddress QKnxAddress::createGroup(quint8 mainGroup, quint16 middleGroup, quint8 subGroup)
{
    return QKnxAddress(QKnxAddress::Type::Group, mainGroup, &middleGroup, subGroup);
}

/*!
    Creates a KNX \l Individual address from the 3-level notation. The value of
    \a area should be in the range \c 0 to \c 15, the value of \a line in the
    range \c 0 to \c 15, and the \a sequentialNumber value in the range \c 0 to
    \c 255. Hexadecimal, octal, and decimal notation are supported.
*/
QKnxAddress QKnxAddress::createIndividual(quint8 area, quint16 line, quint8 sequentialNumber)
{
    return QKnxAddress(QKnxAddress::Type::Individual, area, &line, sequentialNumber);
}

/*!
    Returns \c true if this is a valid \l Group KNX address object and the
    address is set to \c 0x0000; \c false otherwise.
*/
bool QKnxAddress::isBroadcast() const
{
    return (m_type == QKnxAddress::Type::Group) && (m_address == 0x0000);
}

/*!
    \relates QKnxAddress
    \variable QKnxAddress::Group::Broadcast

    Represents a single KNX \l Group address with the value \c 0x0000 that is
    reserved for the broadcast address that shall be exclusively used in
    broadcast communication mode.
*/
QKnxAddress QKnxAddress::Group::Broadcast = { QKnxAddress::Type::Group, 0x0000 };

/*!
    \relates QKnxAddress
    \variable QKnxAddress::Individual::Unregistered

    Represents a single KNX \l Individual address with the value \c 0xffff.
*/
QKnxAddress QKnxAddress::Individual::Unregistered = { QKnxAddress::Type::Individual, 0xffff };

/*!
    Returns \c true if this is a valid \l Individual KNX address object and the
    device's sequential number is set to \c 0x00; \c false otherwise.

    Area and line couplers, as well as KNXnet/IP routers (routing) are given
    the sequential number \c 0x00.

    For example, the individual address \c 1.5.0 identifies a KNXnet/IP router
    acting as a line coupler that couples the fifth line with the main line in
    the first area.
*/
bool QKnxAddress::isCouplerOrRouter() const
{
    return (m_type == QKnxAddress::Type::Individual) && (quint8(m_address) == 0x00);
}

/*!
    Returns \c true if this is a valid \l Individual KNX address object and the
    device's sequential number is set to \c 0xff; \c false otherwise.
*/
bool QKnxAddress::isUnregistered() const
{
    return (m_type == QKnxAddress::Type::Individual) && (quint8(m_address) == 0xff);
}

/*!
    Returns \c true if this is a valid KNX address object; \c false otherwise.
*/
bool QKnxAddress::isValid() const
{
    return (m_type == QKnxAddress::Type::Group || m_type == QKnxAddress::Type::Individual)
        && m_address >= 0;
}

/*!
    Returns the KNX address as a string formatted using the internal stored
    notation. An address of the type \l Group is formatted using a forward slash
    as a separator, while an address of the type \l Individual is formatted
    using a dot as separator.

    \note \l Individual addresses support only 3-level notation, whereas
    \l Group addresses support 2-level or 3-level notation. If the address is
    invalid or 2-level notation is requested for individual addresses, an empty
    string is returned.
*/
QString QKnxAddress::toString(Notation notation) const
{
    if (notation == QKnxAddress::Notation::ThreeLevel) {
        if (m_type == QKnxAddress::Type::Group) {
            return QStringLiteral("%1/%2/%3").arg((m_address >> 11) & 0x1f)
                .arg((m_address >> 8) & 0x07).arg(m_address & 0xff);
        }
        if (m_type == QKnxAddress::Type::Individual) {
            return QStringLiteral("%1.%2.%3").arg(m_address >> 12)
                .arg((m_address >> 8) & 0x0f).arg(m_address & 0xff);
        }
    }
    if (notation == QKnxAddress::Notation::TwoLevel && m_type == QKnxAddress::Type::Group)
        return QStringLiteral("%1/%2").arg((m_address >> 11) & 0x1f).arg(m_address & 0x07ff);
    return QString();
}

bool QKnxAddress::operator==(const QKnxAddress &other) const
{
    return m_address == other.m_address && m_type == other.m_type;
}

bool QKnxAddress::operator!=(const QKnxAddress &other) const
{
    return !operator==(other);
}

/*!
    \fn auto QKnxAddress::bytes() const

    Returns a KNX address as a range of bytes if the address is valid;
    otherwise the return value is empty. Only the first two bytes of the return
    value are part of the address.

    \note Only QByteArray and QVector<quint8> are supported as return types.
*/

/*!
    \relates QKnxAddress

    Writes the KNX address \a address to the \a debug stream.
*/
QDebug operator<<(QDebug debug, const QKnxAddress &address)
{
    QDebugStateSaver _(debug);
    if (address.isValid()) {
        const auto rawData = address.bytes<QVector<quint8>>();
        debug.nospace().noquote() << "0x" << hex << qSetFieldWidth(2) << qSetPadChar(QLatin1Char('0'))
            << rawData[0] << hex << rawData[1];
    } else {
         debug.nospace().noquote() << "0x1nv4l1d";
    }
    return debug;
}

/*!
    \relates QKnxAddress

    Writes the KNX address \a address to the stream \a out and returns a
    reference to the stream.
*/
QDataStream &operator<<(QDataStream &out, const QKnxAddress &address)
{
    if (!address.isValid())
        return out;
    const auto rawData = address.bytes();
    return out << quint8(rawData[0]) << quint8(rawData[1]);
}


// -- private

/*!
    \internal

    Creates a KNX address from 2 or 3-level notation. The type of the address is
    specified by \a type. Hexadecimal, octal, and decimal notation are
    supported.
*/
QKnxAddress::QKnxAddress(QKnxAddress::Type type, quint16 sec1, quint16 *sec2, quint16 sec3)
{
    const auto checkRange = [type, sec1, sec2, sec3](QKnxAddress::Notation notation) {
        if (type == QKnxAddress::Type::Group) {
            if (sec1 > 31)
                return false;
            if (notation == QKnxAddress::Notation::TwoLevel)
                return (sec3 <= 2047);
            if (sec2 && (notation == QKnxAddress::Notation::ThreeLevel))
                return (*sec2 <= 7) && (sec3 <= 255);
        }
        if (sec2 && (type == QKnxAddress::Type::Individual))
            return (sec1 <= 15) && ( *sec2 <= 15) && (sec3 <= 255);
        return false;
    };

    if (type == QKnxAddress::Type::Group) {
        if (!sec2) {
            if (checkRange(QKnxAddress::Notation::TwoLevel)) {
                m_type = type;
                m_address = quint16(sec1 << 11 | sec3);
            }
        } else {
            if (checkRange(QKnxAddress::Notation::ThreeLevel)) {
                m_type = type;
                m_address = quint16(sec1 << 11 | (*sec2) << 8 | sec3);
            }
        }
    }
    if (type == QKnxAddress::Type::Individual) {
    if (checkRange(QKnxAddress::Notation::ThreeLevel)) {
            m_type = type;
            m_address = quint16(sec1 << 12 | (*sec2) << 8 | sec3);
        }
    }
}

uint qHash(const QKnxAddress &key, uint seed) Q_DECL_NOTHROW
{
    return qHash(key.bytes(), seed);
}

QT_END_NAMESPACE
