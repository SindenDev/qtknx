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

#include "qknxnetiproutingindication.h"

QT_BEGIN_NAMESPACE

QKnxNetIpRoutingIndication::QKnxNetIpRoutingIndication(const QKnxLinkLayerFrame &frame)
    : QKnxNetIpFrame(QKnxNetIp::ServiceType::RoutingIndication)
{
    setLinkLayerFrame(frame);
}

QKnxNetIpRoutingIndication::QKnxNetIpRoutingIndication(const QKnxNetIpFrame &other)
    : QKnxNetIpFrame(other)
{}

QKnxLinkLayerFrame QKnxNetIpRoutingIndication::linkLayerFrame() const
{
    return QKnxLinkLayerFrame::fromBytes(payloadRef(), 0, payloadRef().size());
}

void QKnxNetIpRoutingIndication::setLinkLayerFrame(const QKnxLinkLayerFrame &frame)
{
    setPayload(QKnxNetIpPayload::fromBytes(frame.bytes(), 0, frame.size()));
}

bool QKnxNetIpRoutingIndication::isValid() const
{
    // TODO: fix size check once the minimum CEMI frame size is known
    return QKnxNetIpFrame::isValid() && size() >= 8
        && code() == QKnxNetIp::ServiceType::RoutingIndication;
}

QT_END_NAMESPACE
