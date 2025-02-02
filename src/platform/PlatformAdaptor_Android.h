/***************************************************************************
 *   Copyright (C) 2019-2023 by Stefan Kebekus                             *
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

#include <QObject>
#include <QTimer>
#include <QtGlobal>

#include "platform/PlatformAdaptor_Abstract.h"

namespace Platform {

/*! \brief Implementation of PlatformAdaptor for Android devices */


class PlatformAdaptor : public Platform::PlatformAdaptor_Abstract
{
    Q_OBJECT

public:
    /*! \brief Standard constructor
     *
     * @param parent Standard QObject parent pointer
     */
    explicit PlatformAdaptor(QObject *parent = nullptr);

    ~PlatformAdaptor() override = default;


    //
    // Methods
    //

    /*! \brief Implements pure virtual method from PlatformAdaptor_Abstract
     *
     *  @returns see PlatformAdaptor_Abstract
     */
    Q_INVOKABLE QString checkPermissions() override;

    /*! \brief Implements pure virtual method from PlatformAdaptor_Abstract
     *
     *  @returns see PlatformAdaptor_Abstract
     */
    Q_INVOKABLE QString currentSSID() override;

    /*! \brief Implements pure virtual method from PlatformAdaptor_Abstract */
    void disableScreenSaver() override;

    /*! \brief Implements pure virtual method from PlatformAdaptor_Abstract
     *
     *  @param lock see PlatformAdaptor_Abstract
     */
    Q_INVOKABLE void lockWifi(bool lock) override;

    /*! \brief Information about the system, in HTML format
     *
     * @returns Info string
     */
    Q_INVOKABLE virtual QString systemInfo() override;

    /*! \brief Implements pure virtual method from PlatformAdaptor_Abstract */
    Q_INVOKABLE void vibrateBrief() override;


public slots:
    /*! \brief Implements pure virtual method from PlatformAdaptor_Abstract */
    void onGUISetupCompleted() override;

    /*! \brief Implements pure virtual method from GlobalObject */
    void requestPermissionsSync() override;


protected:
    /*! \brief Implements virtual method from PlatformAdaptor_Abstract */
    void deferredInitialization() override;

private:
    Q_DISABLE_COPY_MOVE(PlatformAdaptor)

    bool splashScreenHidden {false};
};

} // namespace Platform
