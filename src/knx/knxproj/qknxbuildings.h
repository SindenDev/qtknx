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

#ifndef QKNXBUILDINGS_H
#define QKNXBUILDINGS_H

#include <QtCore/qstring.h>
#include <QtCore/qvector.h>
#include <QtKnx/qknxprojectutils.h>

QT_BEGIN_NAMESPACE

struct Q_KNX_EXPORT QKnxGroupAddressRef
{
    Q_DECLARE_TR_FUNCTIONS(QKnxGroupAddressRef)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    QString RefId; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Role; // optional, 255 character max.
    qint32 Puid;

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxFunction
{
    Q_DECLARE_TR_FUNCTIONS(QKnxFunction)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    QString Type; // optional, Building, BuildingPart, Floor, Room, DistributionBoard, Stairway, Corridor
    QString Number; // optional, 255 character max.
    QString Comment; // optional
    QString Description; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    QString DefaultGroupRange; // optional, non-colonized name, pattern [\i-[:]][\c-[:]]*
    qint32 Puid;

    QVector<QKnxGroupAddressRef> GroupAddressRef; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxBuildingPart
{
    Q_DECLARE_TR_FUNCTIONS(QKnxBuildingPart)

public:
    QString Id; // non-colonized name, pattern [\i-[:]][\c-[:]]*
    QString Name; // 255 character max.
    QString Type; // Building, BuildingPart, Floor, Room, DistributionBoard, Stairway, Corridor
    QString Number; // optional, 255 character max.
    QString Comment; // optional
    QString Description; // optional

    // Undefined, Editing, FinishedDesign, FinishedCommissioning, Tested, Accepted, Locked
    QString CompletionStatus { QLatin1String("Undefined") }; // optional

    QString DefaultLine; // optional
    qint32 Puid;

    QVector<QKnxBuildingPart> BuildingPart; // 0..n
    QVector<QString> DeviceInstanceRef; // 0..n, non-colonized name, pattern [\i-[:]][\c-[:]]*
    QVector<QKnxFunction> Function; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

struct Q_KNX_EXPORT QKnxBuildings
{
    Q_DECLARE_TR_FUNCTIONS(QKnxBuildings)

public:
    QVector<QKnxBuildingPart> BuildingPart; // 0..n

    bool parseElement(QXmlStreamReader *reader, bool pedantic);
};

QT_END_NAMESPACE

#endif
