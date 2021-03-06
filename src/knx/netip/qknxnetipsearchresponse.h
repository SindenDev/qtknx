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

#ifndef QKNXNETIPSEARCHRESPONSE_H
#define QKNXNETIPSEARCHRESPONSE_H

#include <QtKnx/qknxnetipdevicedib.h>
#include <QtKnx/qknxnetipframe.h>
#include <QtKnx/qknxnetiphpai.h>
#include <QtKnx/qknxnetipservicefamiliesdib.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxNetIpSearchResponse final : public QKnxNetIpFrame
{
public:
    QKnxNetIpSearchResponse() = default;
    ~QKnxNetIpSearchResponse() override = default;

    QKnxNetIpSearchResponse(const QKnxNetIpHpai &controlEndpoint,
                            const QKnxNetIpDeviceDib &deviceHardware,
                            const QKnxNetIpServiceFamiliesDib &supportedFamilies);

    template <typename T> static QKnxNetIpSearchResponse fromBytes(const T &bytes, quint16 index)
    {
        return QKnxNetIpFrameHelper::fromBytes(bytes, index, QKnxNetIp::ServiceType::SearchResponse);
    }

    QKnxNetIpHpai controlEndpoint() const;
    QKnxNetIpDeviceDib deviceHardware() const;
    QKnxNetIpServiceFamiliesDib supportedFamilies() const;

    bool isValid() const override;

private:
    QKnxNetIpSearchResponse(const QKnxNetIpFrame &other);
};

QT_END_NAMESPACE

#endif
