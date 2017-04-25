/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
****************************************************************************/

#include <QtCore/qdebug.h>
#include <QtKnx/qknxnetipknxaddressesdib.h>
#include <QtTest/qtest.h>

class tst_QKnxNetIpKnxAddressesDIB : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testConstructorWithOneArgument();
    void testConstructorWithTwoArguments();
    void testIndividualAddresses();
    void testDebugStream()
    {
        // TODO: Implement.
    }
    void testDataStream()
    {
        // TODO: Implement.
    }
};

void tst_QKnxNetIpKnxAddressesDIB::testDefaultConstructor()
{
    QKnxNetIpKnxAddressesDIB addresses;
    QCOMPARE(addresses.isValid(), false);
    QCOMPARE(addresses.size(), quint16(0));
    QCOMPARE(addresses.bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(addresses.payload().size(), quint16(0));
    QCOMPARE(addresses.payload().bytes<QByteArray>(), QByteArray(""));
    QCOMPARE(addresses.toString(), QString::fromLatin1("Total size { 0x00 }, "
        "Code { 0x00 }, Bytes {  }"));
    QCOMPARE(quint8(addresses.descriptionTypeCode()), quint8(0));
}

void tst_QKnxNetIpKnxAddressesDIB::testConstructorWithOneArgument()
{
    QKnxNetIpKnxAddressesDIB addresses(QKnxAddress::createIndividual(1, 1, 1));
    QCOMPARE(addresses.isValid(), true);
    QCOMPARE(addresses.size(), quint16(4));
    QCOMPARE(addresses.bytes<QByteArray>(), QByteArray::fromHex("04051101"));
    QCOMPARE(addresses.payload().size(), quint16(2));
    QCOMPARE(addresses.payload().bytes<QByteArray>(), QByteArray::fromHex("1101"));
    QCOMPARE(addresses.toString(), QString::fromLatin1("Total size { 0x04 }, "
        "Code { 0x05 }, Bytes { 0x11, 0x01 }"));
    QCOMPARE(addresses.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::KnxAddresses);
}

void tst_QKnxNetIpKnxAddressesDIB::testConstructorWithTwoArguments()
{
    QVector<QKnxAddress> qknxAddresses;
    qknxAddresses.append(QKnxAddress::createIndividual(1, 1, 0));
    qknxAddresses.append(QKnxAddress::createIndividual(1, 2, 5));
    qknxAddresses.append(QKnxAddress::createIndividual(2, 3, 8));
    QKnxNetIpKnxAddressesDIB addresses(qknxAddresses);
    QCOMPARE(addresses.isValid(), true);
    QCOMPARE(addresses.size(), quint16(8));
    QCOMPARE(addresses.bytes<QByteArray>(), QByteArray::fromHex("0805110012052308"));
    QCOMPARE(addresses.payload().size(), quint16(6));
    QCOMPARE(addresses.payload().bytes<QByteArray>(), QByteArray::fromHex("110012052308"));
    QCOMPARE(addresses.toString(), QString::fromLatin1("Total size { 0x08 }, "
        "Code { 0x05 }, Bytes { 0x11, 0x00, 0x12, 0x05, 0x23, 0x08 }"));
    QCOMPARE(addresses.descriptionTypeCode(),
        QKnxNetIp::DescriptionTypeCode::KnxAddresses);
}

void tst_QKnxNetIpKnxAddressesDIB::testIndividualAddresses()
{
    QVector<QKnxAddress> qknxAddresses;
    qknxAddresses.append(QKnxAddress::createIndividual(1, 1, 0));
    qknxAddresses.append(QKnxAddress::createIndividual(1, 2, 5));
    qknxAddresses.append(QKnxAddress::createIndividual(2, 3, 8));
    QKnxNetIpKnxAddressesDIB addresses(qknxAddresses);

    auto retrievedAddresses = addresses.individualAddresses<QVector<QKnxAddress>>();
    QCOMPARE(retrievedAddresses.size(), qknxAddresses.size());
    QCOMPARE(retrievedAddresses[0].toString(), qknxAddresses[0].toString());
    QCOMPARE(retrievedAddresses[1].toString(), qknxAddresses[1].toString());
    QCOMPARE(retrievedAddresses[2].toString(), qknxAddresses[2].toString());
}

QTEST_APPLESS_MAIN(tst_QKnxNetIpKnxAddressesDIB)

#include "tst_qknxnetipknxaddressesdib.moc"
