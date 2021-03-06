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

#ifndef QKNXNETIPCONFIGDIB_H
#define QKNXNETIPCONFIGDIB_H

#include <QtCore/qbytearray.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>
#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxglobal.h>
#include <QtKnx/qknxnetipstruct.h>
#include <QtKnx/qknxtraits.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qnetworkinterface.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpConfigDib final : public QKnxNetIpDescriptionTypeStruct
{
public:
    // 03_08_03 Management v01.06.02 AS, 2.5.7 PID_IP_CAPABILITIES (PID = 56)
    enum class Capability : quint8
    {
        BootP = 0x00,
        Dhcp = 0x01,
        AutoIp = 0x02
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)

    // 03_08_03 Management v01.06.02 AS, 2.5.6 PID_IP_ASSIGNMENT_METHOD (PID = 55)
    enum class AssignmentMethod : quint8
    {
        Manual = 0x01,
        BootP = 0x02,
        Dhcp = 0x04,
        AutoIp = 0x08
    };
    Q_DECLARE_FLAGS(AssignmentMethods, AssignmentMethod)

    QKnxNetIpConfigDib() = default;
    ~QKnxNetIpConfigDib() override = default;

    QKnxNetIpConfigDib(const QHostAddress &ipAddress,
                       const QHostAddress &subnetMask,
                       const QHostAddress &gateway,
                       Capabilities capabilitys,
                       AssignmentMethods methods);

    QKnxNetIpConfigDib(const QNetworkAddressEntry &addressEntry,
                       const QHostAddress &gateway,
                       Capabilities capabilitys,
                       AssignmentMethods methods);

    template <typename T> static QKnxNetIpConfigDib fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpStructHelper::fromBytes(bytes, index,
            QKnxNetIp::DescriptionType::IpConfiguration);
    }

    QKnxNetIp::DescriptionType descriptionType() const;
    QHostAddress ipAddress() const;
    QHostAddress subnetMask() const;
    QHostAddress defaultGateway() const;
    Capabilities capabilities() const;
    AssignmentMethods assignmentMethods() const;

    bool isValid() const override;

private:
    QKnxNetIpConfigDib(const QKnxNetIpDescriptionTypeStruct &other);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIpConfigDib::Capabilities)
Q_DECLARE_OPERATORS_FOR_FLAGS(QKnxNetIpConfigDib::AssignmentMethods)

QT_END_NAMESPACE

#endif
