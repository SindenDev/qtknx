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

#include "qknxnetipservicefamiliesdib.h"

#include <algorithm>

QT_BEGIN_NAMESPACE

// 7.5.4.3 Supported service families DIB
// The service family IDs shall be the high octet of the Service Type ID

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib()
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(const QKnxNetIpDescriptionTypeStruct &other)
    : QKnxNetIpDescriptionTypeStruct(other)
{}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(ServiceFamilieId id, quint8 version)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(id, version);
}

QKnxNetIpServiceFamiliesDib::QKnxNetIpServiceFamiliesDib(const ServiceFamilyIdVersions &families)
    : QKnxNetIpDescriptionTypeStruct(QKnxNetIp::DescriptionType::SupportedServiceFamilies)
{
    add(families);
}

QKnxNetIp::DescriptionType QKnxNetIpServiceFamiliesDib::descriptionType() const
{
    return code();
}

void QKnxNetIpServiceFamiliesDib::add(ServiceFamilieId id, quint8 version)
{
    auto load = payload();
    load.appendBytes<std::array<quint8, 2>, 2>(std::array<quint8, 2>{ { quint8(id), version } });
    setPayload(load);
}

void QKnxNetIpServiceFamiliesDib::add(const ServiceFamilyIdVersions &families)
{
    QByteArray additionalData;

    int i = 0;
    auto keys = families.uniqueKeys();
    for (auto key : qAsConst(keys)) {
        auto values = families.values(key);
        std::sort(std::begin(values), std::end(values));
        for (auto value : values)
            additionalData[i++] = quint8(key), additionalData[i++] = value;
    }

    auto load = payload();
    load.appendBytes(additionalData);
    setPayload(load);
}

QKnxNetIpServiceFamiliesDib::ServiceFamilyIdVersions
    QKnxNetIpServiceFamiliesDib::serviceFamilyIdVersions() const
{
    ServiceFamilyIdVersions serviceTypesAndVersions;

    const auto &ref = payloadRef();
    for (int i = 0 ; i < ref.size() ; i += 2)
        serviceTypesAndVersions.insertMulti(ServiceFamilieId(ref.byte(i)), ref.byte(i+1));

    return serviceTypesAndVersions;
}

bool QKnxNetIpServiceFamiliesDib::isValid() const
{
    return QKnxNetIpDescriptionTypeStruct::isValid() && (size() % 2 == 0) // must be even sized
        && descriptionType() == QKnxNetIp::DescriptionType::SupportedServiceFamilies;
}

QT_END_NAMESPACE
