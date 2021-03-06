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

#include "qknxnetipcrd.h"

QT_BEGIN_NAMESPACE

QKnxNetIpCrd::QKnxNetIpCrd(const QKnxNetIpConnectionTypeStruct &other)
    : QKnxNetIpConnectionTypeStruct(other)
{}

QKnxNetIpCrd::QKnxNetIpCrd(QKnxNetIp::ConnectionType connectionType)
    : QKnxNetIpConnectionTypeStruct(connectionType)
{}

QKnxNetIpCrd::QKnxNetIpCrd(const QKnxAddress &individualAddress)
    : QKnxNetIpConnectionTypeStruct(QKnxNetIp::ConnectionType::Tunnel)
{
    setIndividualAddress(individualAddress);
}

QKnxNetIp::ConnectionType QKnxNetIpCrd::connectionType() const
{
    return code();
}

void QKnxNetIpCrd::setConnectionType(QKnxNetIp::ConnectionType connectionType)
{
    setCode(connectionType);
}

bool QKnxNetIpCrd::isValid() const
{
    switch (connectionType()) {
        case QKnxNetIp::ConnectionType::DeviceManagement:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        case QKnxNetIp::ConnectionType::Tunnel: {
            auto address = individualAddress();
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 4
                && address.isValid() && address.type() == QKnxAddress::Type::Individual;
        } break;
        case QKnxNetIp::ConnectionType::RemoteLogging:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        case QKnxNetIp::ConnectionType::RemoteConfiguration:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        case QKnxNetIp::ConnectionType::ObjectServer:
            return QKnxNetIpConnectionTypeStruct::isValid() && size() == 2;
        default:
            break;
    }
    return false;
}

QKnxAddress QKnxNetIpCrd::individualAddress() const
{
    return QKnxAddress(QKnxAddress::Type::Individual, QKnxUtils::QUint16::fromBytes(payloadRef()));
}

bool QKnxNetIpCrd::setIndividualAddress(const QKnxAddress &address)
{
    if (connectionType() != QKnxNetIp::ConnectionType::Tunnel
        || (!address.isValid()) || (address.type() != QKnxAddress::Type::Individual))
        return false;

    setPayload(QKnxNetIpPayload::fromBytes(address.bytes(), 0, address.size()));
    return true;
}

QT_END_NAMESPACE
