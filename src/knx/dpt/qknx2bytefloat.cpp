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

#include "qknx2bytefloat.h"
#include "qknxdatapointtype_p.h"

#include "qknxutils.h"
#include "qmath.h"

QT_BEGIN_NAMESPACE

/*!
    \class QKnx2ByteFloat
    \inherits QKnxFixedSizeDatapointType
    \inmodule QtKnx

    \brief The QKnx2ByteFloat class is a datapoint type for a 2-byte float
    value.

    This is a fixed size datapoint type with the length of 2 bytes.
    It is a base class for datapoint types for temperature, brightness in LUX,
    and wind speed, as well as air pressure, humidity, quality, and flow, for
    example.

    The range for the value of this datapoint type is from \c {-671 088.64}
    to \c {670 760.96}.

    The float is encoded as \c (0.01*M)*2^E where \c E and \c M are
    encoded as follows in the 2 bytes:

    \code
    MEEEEMMM  MMMMMMMM
    \endcode

    This class is a base class for the following datapoint types:

    \table
    \header
        \li Class
        \li Description
        \li Range
    \row
        \li \c QKnxAirFlow
        \li Air flow in m3/h.
        \li -670760 - 670760
    \row
        \li \c QKnxAirQuality
        \li Air quality in ppm.
        \li 0 - 670760
    \row
        \li \c QKnxAmountRain
        \li Amount of Rain in liters per square meter (l/m2).
        \li -671088.64 - 670760.96
    \row
        \li \c QKnxCurrent
        \li Current in milliampere (mA).
        \li -670760 - 670760
    \row
        \li \c QKnxHumidity
        \li Humidity as a percentage.
        \li 0 - 670760
    \row
        \li \c QKnxKelvinPerPercent
        \li Kelvin per percent (K/Percent).
        \li -670760 - 670760
    \row
        \li \c QKnxPower
        \li Power in kilowatts (kW).
        \li -670760 - 670760
    \row
        \li \c QKnxPowerDensity
        \li Power Density in watts per square meter (W/m2).
        \li -670760 - 670760
    \row
        \li \c QKnxPressure
        \li Pressure in Pascal (Pa).
        \li 0 - 670760
    \row
        \li \c QKnxTemperatureCelsius
        \li Temperature in degrees Celsius.
        \li -273 - 670760
    \row
        \li \c QKnxTemperatureChange
        \li Change in temperature (K) per hour.
        \li -670760 - 670760
    \row
        \li \c QKnxTemperatureFahrenheit
        \li Temperature in degrees Fahrenheit.
        \li -459.6 - 670760.96
    \row
        \li \c QKnxTemperatureKelvin
        \li Temperature in degrees Kelvin.
        \li -670760 - 670760
    \row
        \li \c QKnxTimeMilliSecond
        \li Time in milliseconds (ms).
        \li -670760 - 670760
    \row
        \li \c QKnxTimeSecond
        \li Time in seconds.
        \li -670760 - 670760
    \row
        \li \c QKnxValueLux
        \li Brightness in LUX.
        \li 0 - 670760
    \row
        \li \c QKnxVoltage
        \li Voltage in milli-volts (mV).
        \li -670760 - 670760
    \row
        \li \c QKnxVolumeFlow
        \li Volume flow in liters per hour (l/h).
        \li -670760 - 670760
    \row
        \li \c QKnxWindSpeed
        \li Wind speed in meters per second (m/s).
        \li 0 - 670760
    \row
        \li \c QKnxWindSpeedKmPerHour
        \li Wind speed in kilometers per hour (km/h).
        \li 0 - 670760.96
    \endtable

    \sa QKnxDatapointType
*/

/*!
    Creates a fixed size datapoint type.
*/
QKnx2ByteFloat::QKnx2ByteFloat()
    : QKnx2ByteFloat(0.0)
{}

/*!
    Creates a fixed size datapoint type with the float \a value.
*/
QKnx2ByteFloat::QKnx2ByteFloat(float value)
    : QKnx2ByteFloat(SubType, value)
{}

/*!
    Creates a fixed size datapoint type with the subtype \a subType and float
    \a value.
*/
QKnx2ByteFloat::QKnx2ByteFloat(int subType, float value)
    : QKnxFixedSizeDatapointType(MainType, subType, TypeSize)
{
    setDescription(tr("2-byte float"));
    setRangeText(tr("Minimum Value, -671 088.64"), tr("Maximum Value, 670 760.96"));
    setRange(QVariant::fromValue(-671088.64), QVariant::fromValue(670760.96));

    setValue(value);
}

/*!
    Returns the float stored in the datapoint type.
*/
float QKnx2ByteFloat::value() const
{
    quint16 temp = QKnxUtils::QUint16::fromBytes(bytes());
    quint16 encodedM = (temp & 0x87ff);
    // Turning on bits reserved for E.
    // Only needed for reinterpretation of negative values
    if (encodedM > 2047)
        encodedM += 0x7800;

    qint16 M = qint16(encodedM);
    quint8 E = (temp & 0x7800) >> 11;
    return float(0.01 * (M) * qPow(2, qreal(E)));
}

/*!
    Sets the float of the datapoint type to \a value.

    If the value is outside the allowed range, returns \c false and does not set
    the value.
*/
bool QKnx2ByteFloat::setValue(float value)
{
    if (value < minimum().toFloat() || value > maximum().toFloat())
        return false;

    quint8 E = 0;
    if (qAbs(qreal(value)) > 20.48)
        E = quint8(qFloor(qLn(qAbs(qreal(value) * 100 / 2048.)) / qLn(2) + 1));
    qint32 M = qint32(qRound((value*float(qPow(2, -E)) * 100)));
    if (E > 15 || M > 2047 || M < -2048)
        return false; // Should never happen considering the ranges of value.

    quint16 encodedM = quint16(M);
    if (value < 0)
        encodedM &= 0x87ff;
    encodedM |= E << 11;
    setBytes(QKnxUtils::QUint16::bytes(encodedM), 0, 2);
    return true;
}

/*!
    \reimp
*/
bool QKnx2ByteFloat::isValid() const
{
    return QKnxDatapointType::isValid()
        && value() >= minimum().toFloat() && value() <= maximum().toFloat();
}

#define CREATE_CLASS_BODY(CLASS, DESCRIPTION, RANGE_TEXT_MINIMUM, RANGE_TEXT_MAXIMUM, UNIT, \
    RANGE_VALUE_MINIMUM, RANGE_VALUE_MAXIMUM) \
CLASS::CLASS() \
    : QKnx2ByteFloat(SubType, 0.0) \
{ \
    setUnit(tr(UNIT)); \
    setDescription(tr(DESCRIPTION)); \
    setRangeText(tr(RANGE_TEXT_MINIMUM), tr(RANGE_TEXT_MAXIMUM)); \
    setRange(QVariant::fromValue(RANGE_VALUE_MINIMUM), QVariant::fromValue(RANGE_VALUE_MAXIMUM)); \
} \
CLASS::CLASS(float value) \
    : CLASS() \
{ \
    setValue(value); \
}

CREATE_CLASS_BODY(QKnxTemperatureCelsius, "Temperature in degree Celsius",
    "Minimum Value, -273", "Maximum Value, 670 760","degree Celsius", -273, 670760)
CREATE_CLASS_BODY(QKnxTemperatureKelvin, "Temperature in degree Kelvin",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "degree Kelvin", -670760, 670760)
CREATE_CLASS_BODY(QKnxTemperatureChange, "Change in Temperature (K) per hour",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "Lux", -670760, 670760)
CREATE_CLASS_BODY(QKnxValueLux, "Brightness in Lux",
    "Minimum Value, 0", "Maximum Value, 670 760", "Lux", 0, 670760)
CREATE_CLASS_BODY(QKnxWindSpeed, "Wind Speed in meter per second",
    "Minimum Value, 0", "Maximum Value, 670 760", "m/s", 0, 670760)
CREATE_CLASS_BODY(QKnxPressure, "Pressure in Pascal",
    "Minimum Value, 0", "Maximum Value, 670 760", "Pa", 0, 670760)
CREATE_CLASS_BODY(QKnxHumidity, "Humidity in percent",
    "Minimum Value, 0", "Maximum Value, 670 760", "Percent", 0, 670760)
CREATE_CLASS_BODY(QKnxAirQuality, "Air Quality in ppm",
    "Minimum Value, 0", "Maximum Value, 670 760", "ppm", 0, 670760)
CREATE_CLASS_BODY(QKnxAirFlow, "Air Flow in m3/h",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "m3/h", -670760, 670760)
CREATE_CLASS_BODY(QKnxTimeSecond, "Time in second",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "s", -670760, 670760)
CREATE_CLASS_BODY(QKnxTimeMilliSecond, "Time in milli-Second",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "ms", -670760, 670760)
CREATE_CLASS_BODY(QKnxVoltage, "Voltage in milli-Volt",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "mV", -670760, 670760)
CREATE_CLASS_BODY(QKnxCurrent, "Current in milli-Amper",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "mA", -670760, 670760)
CREATE_CLASS_BODY(QKnxPowerDensity, "Power Density in Watt per square meter",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "W/m2", -670760, 670760)
CREATE_CLASS_BODY(QKnxKelvinPerPercent, "Kelvin per Percent",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "K/Percent", -670760, 670760)
CREATE_CLASS_BODY(QKnxPower, "Power in kilo Watt",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "kW", -670760, 670760)
CREATE_CLASS_BODY(QKnxVolumeFlow, "Volume Flow in liter per hour",
    "Minimum Value, -670 760", "Maximum Value, 670 760", "l/h", -670760, 670760)
CREATE_CLASS_BODY(QKnxAmountRain, "Amount of Rain in liter per square meter",
    "Minimum Value, -671 088.64", "Maximum Value, 670 760", "l/m2", -671088.64, 670760.96)
CREATE_CLASS_BODY(QKnxTemperatureFahrenheit, "Temperature in Fahrenheit",
    "Minimum Value, -459.6", "Maximum Value, 670 760", "degree F", -459.6, 670760.96)
CREATE_CLASS_BODY(QKnxWindSpeedKmPerHour, "Wind Speed in kilometer per hour",
    "Minimum Value, 0", "Maximum Value, 670 760.96", "km/h", 0, 670760.96)

#undef CREATE_CLASS_BODY

QT_END_NAMESPACE
