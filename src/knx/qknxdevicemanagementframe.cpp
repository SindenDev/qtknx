/******************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtKnx module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/

#include "qknxcemi.h"
#include "qknxdevicemanagementframe.h"

QT_BEGIN_NAMESPACE

QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(QKnxDeviceManagementFrame::MessageCode code)
    : QKnxCemiFrame(code)
{
    if (code != QKnxCemiFrame::MessageCode::ResetRequest
        && code != QKnxCemiFrame::MessageCode::ResetIndication) {
        static const  constexpr quint8 data[6] = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
        setServiceInformation({ data, 6 });
    }
}

QKnxDeviceManagementFrame::Error QKnxDeviceManagementFrame::errorCode() const
{
    if (!isNegativeConfirmation())
        return QKnxDeviceManagementFrame::Error::None;

    auto err = data();
    if (err.size() < 1)
        return QKnxDeviceManagementFrame::Error::Data;
    return QKnxDeviceManagementFrame::Error(quint8(err[0]));
}

bool QKnxDeviceManagementFrame::isValid() const
{
    switch (messageCode()) {
    case QKnxCemiFrame::MessageCode::PropertyReadRequest:
        if (size() > 7) // 4.1.7.3.2 The request shall not contain any further data
        return false;
    case QKnxCemiFrame::MessageCode::PropertyReadConfirmation:
    case QKnxCemiFrame::MessageCode::PropertyWriteRequest:
    case QKnxCemiFrame::MessageCode::PropertyWriteConfirmation:
    case QKnxCemiFrame::MessageCode::PropertyInfoIndication:
    case QKnxCemiFrame::MessageCode::FunctionPropertyCommandRequest:
    case QKnxCemiFrame::MessageCode::FunctionPropertyStateReadRequest:
    case QKnxCemiFrame::MessageCode::FunctionPropertyCommandConfirmation:
    // same as the one above
    //case QKnxCemiFrame::MessageCode::FunctionPropertyStateReadConfirmation:
        if (size() < 7)
            return false;
        break;
    case QKnxCemiFrame::MessageCode::ResetRequest:
    case QKnxCemiFrame::MessageCode::ResetIndication:
        return size() == 1;
    default:
        return false;
    }

    auto type = objectType();
    if (!QKnxInterfaceObject::Type::isObjectType(type))
        return false;

    if (objectInstance() < 1)
        return false;

    if (!QKnxInterfaceObject::Type::isMatch(type, property()))
        return false;
    return true; // TODO: Find other definitions of validity.
}

bool QKnxDeviceManagementFrame::isNegativeConfirmation() const
{
    // 4.1.7.3.3 Property read confirmation
    // 4.1.7.3.5 Property write confirmation
    // The response indicates an error with number of elements == 0.
    return (messageCode() == QKnxCemiFrame::MessageCode::PropertyReadConfirmation
        || messageCode() == QKnxCemiFrame::MessageCode::PropertyWriteConfirmation)
        && numberOfElements() == 0;
}

QKnxInterfaceObject::Type QKnxDeviceManagementFrame::objectType() const
{
    return QKnxInterfaceObject::Type(QKnxUtils::QUint16::fromBytes(serviceInformationRef()));
}

void QKnxDeviceManagementFrame::setObjectType(QKnxInterfaceObject::Type type)
{
    if (!QKnxInterfaceObject::Type::isObjectType(type))
        return;

    auto si = serviceInformation();
    si.replaceBytes(0, QKnxUtils::QUint16::bytes(quint16(type)));
    setServiceInformation(si);
}

quint8 QKnxDeviceManagementFrame::objectInstance() const
{
    return serviceInformationRef().byte(2);
}

void QKnxDeviceManagementFrame::setObjectInstance(quint8 instance)
{
    auto si = serviceInformation();
    si.replaceBytes<std::array<quint8, 1>, 1>(2, { instance });
    setServiceInformation(si);
}

QKnxInterfaceObject::Property QKnxDeviceManagementFrame::property() const
{
    return serviceInformationRef().byte(3);
}

void QKnxDeviceManagementFrame::setProperty(QKnxInterfaceObject::Property pid)
{
    auto si = serviceInformation();
    si.replaceBytes<std::array<quint8, 1>, 1>(3, { pid });
    setServiceInformation(si);
}

quint8 QKnxDeviceManagementFrame::numberOfElements() const
{
    return quint8(serviceInformationRef().byte(4) & 0xf0) >> 4;
}

void QKnxDeviceManagementFrame::setNumberOfElements(quint8 numOfElements)
{
    if (numOfElements > 0xf)
        return;

    auto si = serviceInformation();
    si.setByte(4, (si.byte(4) & 0xf) | (numOfElements << 4));
    setServiceInformation(si);
}

quint16 QKnxDeviceManagementFrame::startIndex() const
{
    return QKnxUtils::QUint16::fromBytes(serviceInformationRef(4)) & 0x0fff;
}

void QKnxDeviceManagementFrame::setStartIndex(quint16 index)
{
    if (index > 0x0fff)
        return;

    auto si = serviceInformation();
    auto startIndex = QKnxUtils::QUint16::fromBytes(serviceInformationRef(4));
    si.replaceBytes(4, QKnxUtils::QUint16::bytes((startIndex & 0xf000) | index));
    setServiceInformation(si);
}

QKnxDeviceManagementFrame::QKnxDeviceManagementFrame(const QKnxCemiFrame &other)
    : QKnxCemiFrame(other)
{}

QT_END_NAMESPACE