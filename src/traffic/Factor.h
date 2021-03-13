/***************************************************************************
 *   Copyright (C) 2021 by Stefan Kebekus                                  *
 *   stefan.kebekus@gmail.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#pragma once

#include <QGeoPositionInfo>
#include <QTimer>

#include "AviationUnits.h"

namespace Traffic {

/*! \brief Traffic opponents
 *
 *  Objects of this class represent traffic opponents, as detected by FLARM and
 *  similar devices.  Instances of this class will be generated by the
 *  Navigation::FLARMAdaptor class. Consumers of this class will never have to
 *  set or construct instances of the class themselves
 */

class Factor : public QObject {
    Q_OBJECT

    // Only FLARMAdaptor can set properties
    friend class AbstractTrafficDataSource;
    friend class TcpTrafficDataSource;
    friend class TrafficDataProvider;

public:
    /*! \brief Aircraft type */
    enum AircraftType
    {
        unknown, /*!< Unknown aircraft type */
        Aircraft, /*!< Fixed wing aircraft */
        Airship, /*!< Airship, such as a zeppelin or a blimp */
        Balloon, /*!< Balloon */
        Copter, /*!< Helicopter, gyrocopter or rotorcraft */
        Drone, /*!< Drone */
        Glider, /*!< Glider, including powered gliders and touring motor gliders */
        HangGlider, /*!< Hang glider */
        Jet, /*!< Jet aircraft */
        Paraglider, /*!< Paraglider */
        Skydiver, /*!< Skydiver */
        StaticObstacle, /*!< Static obstacle */
        TowPlane /*!< Tow plane */
    };
    Q_ENUM(AircraftType)

    /*! \brief Default constructor
     *
     * @param parent The standard QObject parent pointer
     */
    explicit Factor(QObject *parent = nullptr);

    // Standard destructor
    ~Factor() override = default;

    //
    // Methods
    //

    /*! \brief Estimates if this traffic object has higher priority than other
     *  traffic object
     *
     * The following criteria are applied in order.
     *
     * - Valid traffic objects have higher priority than invalid objects.
     * - Traffic objects with higher alarm level have higher priority.
     * - Traffic objects that are closer have higher priority.
     *
     * @param rhs Right hand side of the comparison
     *
     * @returns Boolean with the result
     */
    bool hasHigherPriorityThan(const Factor &rhs);

    //
    // PROPERTIES
    //

    /*! \brief Alarm Level, as reported by FLARM
     *
     * This is the alarm level associated with the traffic object. Alarm levels
     * are not computed by this class, but by the FLARM device that reports the
     * traffic. This is an integer in the range 0, …, 3 with the following
     * meaning.
     *
     *  - 0 = no alarm (also used for no-alarm traffic information)
     *  - 1 = alarm, 13-18 seconds to impact
     *  - 2 = alarm, 9-12 seconds to impact
     *  - 3 = alarm, 0-8 seconds to impact
     */
    Q_PROPERTY(int alarmLevel READ alarmLevel NOTIFY alarmLevelChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property alarmLevel
     */
    int alarmLevel() const
    {
        return _alarmLevel;
    }

    /*! \brief Indicates if changes in properties should be animated in the GUI
     *
     *  This method indicates if changes in properties should be animated in the
     *  GUI. It is "true" if the class believes that changes in the properties
     *  indicate movement of the traffic opponent. It is "false" if the class
     *  believes that changes in the properties indicate that a new traffic
     *  opponent is being tracked.
     */
    Q_PROPERTY(bool animate READ animate NOTIFY animateChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property animate
     */
    bool animate() const
    {
        return _animate;
    }

    /*! \brief Climb rate at the time of report
     *
     *  If known, this property holds the horizontal climb rate of the traffic
     *  at the time of report.  Otherwise, it contains NaN.
     */
    Q_PROPERTY(AviationUnits::Speed climbRate READ climbRate NOTIFY climbRateChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property climbRate
     */
    AviationUnits::Speed climbRate() const
    {
        return _climbRate;
    }

    /*! \brief Climb rate at the time of report, in m/s
     *
     *  If known, this property holds the horizontal climb rate of the traffic
     *  at the time of report, in m/s.  Otherwise, it contains NaN.
     */

    Q_PROPERTY(double climbRateInMPS READ climbRateInMPS NOTIFY hDistChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property climbRateInMPS
     */
    double climbRateInMPS() const
    {
        return _climbRate.toMPS();
    }

    /*! \brief Suggested color for GUI representation of the traffic
     *
     *  This propery suggests a color, depending on the alarmLevel.
     *
     *  - alarmLevel == 0: green
     *  - alarmLevel == 1: yellow
     *  - alarmLevel >= 2: red
     */
    Q_PROPERTY(QString color READ color NOTIFY colorChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property color
     */
    QString color() const
    {
        return _color;
    }

    /*! \brief Coordinate of the traffic, as reported by FLARM
     *
     *  This property contains the coordinate part of the positionInfo. The
     *  property exists for better cooperation with QML
     */
    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate NOTIFY coordinateChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property coordinate
     */
    QGeoCoordinate coordinate() const
    {
        return _positionInfo.coordinate();
    }

    /*! \brief Description of the traffic, for use in GUI
     *
     *  This method holds a human-readable, translated description of the
     *  traffic. This is a rich-text string of the form "Glider<br>+15 0m" or
     *  "Airship<br>Position unknown<br>-45 ft".
     */
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property description
     */
    QString description() const
    {
        return _description;
    }

    /*! \brief Ground speed the time of report
     *
     *  If known, this property holds the ground speed of the traffic
     *  at the time of report.  Otherwise, it contains NaN.
     */
    Q_PROPERTY(AviationUnits::Speed groundSpeed READ groundSpeed NOTIFY groundSpeedChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property groundSpeed
     */
    AviationUnits::Speed groundSpeed() const
    {
        return _groundSpeed;
    }

    /*! \brief Horizontal distance from own position to the traffic,
     *  at the time of report
     *
     *  If known, this property holds the horizontal distance from the own
     *  position to the traffic, at the time of report.  Otherwise, it contains
     *  NaN.
     */
    Q_PROPERTY(AviationUnits::Distance hDist READ hDist NOTIFY hDistChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property hDist
     */
    AviationUnits::Distance hDist() const
    {
        return _hDist;
    }

    /*! \brief Horizontal distance from own position to the traffic in meters,
     *  at the time of report
     *
     *  This property is identical to hDist, but contains the distance in
     *  meters, for better cooperation with QML.
     */
    Q_PROPERTY(double hDistM READ hDistM NOTIFY hDistChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property hDistM
     */
    double hDistM() const
    {
        return _hDist.toM();
    }

    /*! \brief Suggested icon
     *
     *  Depending on alarm level and movement of the traffic opponent, this
     *  property suggests an icon for GUI representation of the traffic.
     */
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property icon
     */
    QString icon() const
    {
        return _icon;
    }

    /*! \brief Indentifier string of the traffic
     *
     *  This property holds an identifier string for the traffic, as assigned by
     *  the FLARM device that reported the traffic. This can be the FLARM ID, or
     *  an empty string if no meaningful ID can be assigned.
     */
    Q_PROPERTY(QString ID READ ID NOTIFY IDChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property ID
     */
    QString ID() const
    {
        return _ID;
    }

    /*! \brief Type of aircraft, as reported by FLARM */
    Q_PROPERTY(AircraftType type READ type NOTIFY typeChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property vDist
     */
    AircraftType type() const
    {
        return _type;
    }

    /*! \brief True track of traffic, as reported by FLARM
     *
     *  This property holds the true track of the traffic, or NaN if no track is
     *  known.
     */
    Q_PROPERTY(double TT READ TT NOTIFY ttChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property TT
     */
    double TT() const
    {
        if (_positionInfo.hasAttribute(QGeoPositionInfo::Direction)) {
            return _positionInfo.attribute(QGeoPositionInfo::Direction);
        }
        return qQNaN();
    }

    /*! \brief Validity
     *
     * A traffic object is considered valid if the data is meaningful and if the
     * report is no older than the time specified in timeoutMS.  Only valid
     * traffic objects should be shown in the GUI.
     */
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property valid
     */
    bool valid() const
    {
        return _valid;
    }

    /*! \brief Vertical distance from own position to the traffic, at the time
     *  of report
     *
     *  If known, this property holds the vertical distance from the own
     *  position to the traffic, at the time of report.  Otherwise, it contains
     *  NaN.
     */
    Q_PROPERTY(AviationUnits::Distance vDist READ vDist NOTIFY vDistChanged)

    /*! \brief Getter method for property with the same name
     *
     *  @returns Property vDist
     */
    AviationUnits::Distance vDist() const
    {
        return _vDist;
    }

signals:
    /*! \brief Notifier signal */
    void alarmLevelChanged();

    /*! \brief Notifier signal */
    void animateChanged();

    /*! \brief Notifier signal */
    void climbRateChanged();

    /*! \brief Notifier signal */
    void colorChanged();

    /*! \brief Notifier signal */
    void coordinateChanged();

    /*! \brief Notifier signal */
    void descriptionChanged();

    /*! \brief Notifier signal */
    void groundSpeedChanged();

    /*! \brief Notifier signal */
    void iconChanged();

    /*! \brief Notifier signal */
    void IDChanged();

    /*! \brief Notifier signal */
    void positionInfoChanged();

    /*! \brief Notifier signal */
    void ttChanged();

    /*! \brief Notifier signal */
    void typeChanged();

    /*! \brief Notifier signal */
    void hDistChanged();

    /*! \brief Notifier signal */
    void vDistChanged();

    /*! \brief Notifier signal */
    void validChanged();

private slots:
    // Setter function for the property color. This slot is bound to the
    // notifier signals of all the properties that color depends on.
    void setColor();

    // Setter function for the property description. This slot is bound to the
    // notifier signals of all the properties that color depends on.
    void setDescription();

    // Setter function for the property icon. This slot is bound to the notifier
    // signals of all the properties that color depends on.
    void setIcon();

    // Setter function for the property valid. This slot is bound to the
    // notifier signals of all the properties that color depends on.  This
    // method will also set the timer, to ensure that it is called again once
    // the object times out.
    void setValid();

private:
    // Copy data from other object
    void copyFrom(const Factor & other)
    {
        setData(other._alarmLevel, other._ID, other._hDist, other._vDist, other._groundSpeed, other._climbRate, other._type, other._positionInfo);
    }

    // Set data
    void setData(int newAlarmLevel,
                 const QString & newID,
                 AviationUnits::Distance newHDist,
                 AviationUnits::Distance newVDist,
                 AviationUnits::Speed newGroundSpeed,
                 AviationUnits::Speed newClimbRate,
                 AircraftType newType,
                 const QGeoPositionInfo & newPositionInfo);

private:
    // Setter function for property animate
    void setAnimate(bool a);

    //
    // Property values
    //
    int _alarmLevel {0};
    bool _animate {true};
    AviationUnits::Speed _climbRate;
    QString _color {QStringLiteral("red")};
    QString _description;
    AviationUnits::Speed _groundSpeed;
    AviationUnits::Distance _hDist;
    QString _icon;
    QString _ID;
    QGeoPositionInfo _positionInfo;
    AircraftType _type {AircraftType::unknown};
    bool _valid {false};
    AviationUnits::Distance _vDist;

    // Timer for timeout. Traffic objects become invalid if their data has not been
    // refreshed for timeoutMS milliseconds
    QTimer timeoutCounter;
    static constexpr qint64 timeoutMS = 10*1000;
};

}
