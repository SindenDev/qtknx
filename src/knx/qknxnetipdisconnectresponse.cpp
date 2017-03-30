/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include "qknxnetipdisconnectresponse.h"

QT_BEGIN_NAMESPACE

QKnxNetIpDisconnectResponse::QKnxNetIpDisconnectResponse(quint8 channelId,
        QKnxNetIp::Error status)
    : QKnxNetIpFrame(quint16(QKnxNetIp::ServiceType::ConnectionStateResponse))
{
    QKnxNetIpPayload payload;
    payload.setByte(0, channelId);
    payload.setByte(1, quint8(status));
    setPayload(payload);
}

quint8 QKnxNetIpDisconnectResponse::channelId() const
{
    return payload().byte(0);
}

QKnxNetIp::Error QKnxNetIpDisconnectResponse::status() const
{
    return QKnxNetIp::Error(payload().byte(1));
}

bool QKnxNetIpDisconnectResponse::isValid() const
{
    return QKnxNetIpFrame::isValid() && size() == 8;
}

QT_END_NAMESPACE