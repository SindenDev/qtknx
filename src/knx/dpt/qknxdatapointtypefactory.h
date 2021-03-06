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

#ifndef QKNXDATAPOINTTYPEFACTORY_H
#define QKNXDATAPOINTTYPEFACTORY_H

#include <QtKnx/qknxdatapointtype.h>
#include <QtKnx/qknxglobal.h>

QT_BEGIN_NAMESPACE

class Q_KNX_EXPORT QKnxDatapointTypeFactory
{
public:
    using FactoryFunction = QKnxDatapointType *(*)();

    ~QKnxDatapointTypeFactory() = default;
    static QKnxDatapointTypeFactory &instance()
    {
        static QKnxDatapointTypeFactory _instance;
        return _instance;
    }

    template <typename Class> void registerType(int mainType, int subType, int size)
    {
        static_assert(std::is_convertible<Class *, QKnxDatapointType *>::value, "Cannot register "
            "class because it is not derived from QKnxDatapointType.");

        QKnxDatapointTypeFactory::setTypeSize(mainType, size);
        factoryTable()[mainType].insert(subType, &QKnxDatapointTypeFactory::create<Class>);
    }

    QKnxDatapointType *createType(int mainType, int subType) const;
    QKnxDatapointType *createType(QKnxDatapointType::Type type) const;

    static int typeSize(int mainType);

    QList<int> mainTypes() const;
    bool containsMainType(int mainType) const;

    QList<int> subTypes(int mainType) const;
    bool containsSubType(int mainType, int subType) const;

private:
    QKnxDatapointTypeFactory();

    template <typename Class> static QKnxDatapointType *create()
    {
        return new Class();
    }

    static QHash<int, QHash<int, FactoryFunction>> &factoryTable()
    {
        static QHash<int, QHash<int, FactoryFunction>> _instance;
        return _instance;
    }

    template <typename Class> void registerType()
    {
        registerType<Class>(Class::MainType, Class::SubType, Class::TypeSize);
    }

    static QHash<int, int> &sizeTable();
    static void setTypeSize(int mainType, int size);

    QKnxDatapointTypeFactory(const QKnxDatapointTypeFactory &) = delete;
    QKnxDatapointTypeFactory &operator=(const QKnxDatapointTypeFactory &) = delete;
};

QT_END_NAMESPACE

#endif
