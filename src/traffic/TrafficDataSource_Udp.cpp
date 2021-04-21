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

#include <QNetworkDatagram>

#include "positioning/PositionProvider.h"
#include "traffic/TrafficDataSource_Udp.h"


// Member functions

Traffic::TrafficDataSource_Udp::TrafficDataSource_Udp(quint16 port, QObject *parent) :
    Traffic::TrafficDataSource_AbstractSocket(parent), m_port(port) {

    // Initialize timers
    m_trueAltitudeTimer.setInterval(5s);
    m_trueAltitudeTimer.setSingleShot(true);

    // Create socket
    connect(&m_socket, &QUdpSocket::errorOccurred, this, &Traffic::TrafficDataSource_Udp::onErrorOccurred);
    connect(&m_socket, &QUdpSocket::readyRead, this, &Traffic::TrafficDataSource_Udp::onReadyRead);
    connect(&m_socket, &QUdpSocket::stateChanged, this, &Traffic::TrafficDataSource_Udp::onStateChanged);

    //
    // Initialize properties
    //
    onStateChanged(m_socket.state());
}


Traffic::TrafficDataSource_Udp::~TrafficDataSource_Udp()
{

    Traffic::TrafficDataSource_Udp::disconnectFromTrafficReceiver();
    setReceivingHeartbeat(false); // This will release the WiFi lock if necessary

}


void Traffic::TrafficDataSource_Udp::connectToTrafficReceiver()
{

    m_socket.abort();
    setErrorString();
    m_socket.bind(m_port);

    // Update properties
    onStateChanged(m_socket.state());
}


void Traffic::TrafficDataSource_Udp::disconnectFromTrafficReceiver()
{

    // Disconnect socket.
    m_socket.abort();

    // Update properties
    onStateChanged(m_socket.state());

}


void Traffic::TrafficDataSource_Udp::onReadyRead()
{

    while (m_socket.hasPendingDatagrams()) {
        QByteArray data = m_socket.receiveDatagram().data();

        if (data.startsWith("XGPS") || data.startsWith("XTRA")) {
            processXGPSString(data);
        } else {
            processGDLMessage(data);
        }
    }

}
