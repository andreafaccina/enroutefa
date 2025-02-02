/***************************************************************************
 *   Copyright (C) 2021-2022 by Stefan Kebekus                             *
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

#include <QDBusInterface>
#include <QMap>

#include "platform/Notifier_Abstract.h"

namespace Platform {

// This class implements notifications for the linux desktop

class Notifier: public Notifier_Abstract
{
    Q_OBJECT

public:
    // Constructor
    explicit Notifier(QObject* parent = nullptr);

    // Destructor
    ~Notifier() override;

public slots:
    // Implementation of pure virtual function
    Q_INVOKABLE void hideNotification(Platform::Notifier_Abstract::NotificationTypes notificationType) override;

    // Implementation of pure virtual function
    void showNotification(Platform::Notifier_Abstract::NotificationTypes notificationType, const QString& text, const QString& longText) override;

private slots:
    // This slot receives ActionInvoked messages from the DBus
    void onActionInvoked(uint id, const QString &key);

    // This slot receives NotificationClosed messages from the DBus
    void onNotificationClosed(uint id, uint reason);

private:
    Q_DISABLE_COPY_MOVE(Notifier)

    // App icon in a format that can be transferred via QDBus
    QVariant iconVariant;

    // Maps NotificationTypes, to IDs of the ongoing notation for the NotificationType
    QMap<Platform::Notifier_Abstract::NotificationTypes, uint> notificationIDs;

    // QDBusInterface to org.freedesktop.Notifications
    // This implementation of notifications uses the specification found here:
    // https://specifications.freedesktop.org/notification-spec/latest/ar01s09.html
    //
    // Help with DBus programming is found here:
    // https://develop.kde.org/docs/d-bus/accessing_dbus_interfaces/
    QDBusInterface notificationInterface {QStringLiteral("org.freedesktop.Notifications"), QStringLiteral("/org/freedesktop/Notifications"), QStringLiteral("org.freedesktop.Notifications"), QDBusConnection::sessionBus()};
};

} // namespace Platform
