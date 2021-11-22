/*
 * ComputerControlInterface.cpp - interface class for controlling a computer
 *
 * Copyright (c) 2017-2021 Tobias Junghans <tobydox@veyon.io>
 *
 * This file is part of Veyon - https://veyon.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include "BuiltinFeatures.h"
#include "ComputerControlInterface.h"
#include "Computer.h"
#include "FeatureManager.h"
#include "MonitoringMode.h"
#include "VeyonConfiguration.h"
#include "VeyonConnection.h"
#include "VncConnection.h"


ComputerControlInterface::ComputerControlInterface( const Computer& computer, int port, QObject* parent ) :
	QObject( parent ),
	m_computer( computer ),
	m_port( port )
{
	m_pingTimer.setInterval(ConnectionWatchdogPingDelay);
	m_pingTimer.setSingleShot(true);
	connect(&m_pingTimer, &QTimer::timeout, this, &ComputerControlInterface::ping);

	m_connectionWatchdogTimer.setInterval( ConnectionWatchdogTimeout );
	m_connectionWatchdogTimer.setSingleShot( true );
	connect( &m_connectionWatchdogTimer, &QTimer::timeout, this, &ComputerControlInterface::restartConnection );
}



ComputerControlInterface::~ComputerControlInterface()
{
	stop();
}



void ComputerControlInterface::start( QSize scaledFramebufferSize, UpdateMode updateMode, AuthenticationProxy* authenticationProxy )
{
	// make sure we do not leak
	stop();

	m_scaledFramebufferSize = scaledFramebufferSize;

	if( m_computer.hostAddress().isEmpty() == false )
	{
		m_connection = new VeyonConnection;
		m_connection->setAuthenticationProxy( authenticationProxy );

		auto vncConnection = m_connection->vncConnection();
		vncConnection->setHost( m_computer.hostAddress() );
		if( m_port > 0 )
		{
			vncConnection->setPort( m_port );
		}
		vncConnection->setQuality( VncConnection::Quality::Thumbnail );
		vncConnection->setScaledSize( m_scaledFramebufferSize );

		connect( vncConnection, &VncConnection::framebufferUpdateComplete, this, &ComputerControlInterface::resetWatchdog );
		connect( vncConnection, &VncConnection::framebufferUpdateComplete, this, &ComputerControlInterface::framebufferUpdated );

		connect( vncConnection, &VncConnection::framebufferSizeChanged, this, &ComputerControlInterface::framebufferSizeChanged );

		connect( vncConnection, &VncConnection::stateChanged, this, &ComputerControlInterface::updateState );
		connect( vncConnection, &VncConnection::stateChanged, this, &ComputerControlInterface::updateUser );
		connect( vncConnection, &VncConnection::stateChanged, this, &ComputerControlInterface::updateActiveFeatures );
		connect( vncConnection, &VncConnection::stateChanged, this, &ComputerControlInterface::updateScreens );
		connect( vncConnection, &VncConnection::stateChanged, this, &ComputerControlInterface::stateChanged );

		connect( m_connection, &VeyonConnection::featureMessageReceived, this, &ComputerControlInterface::handleFeatureMessage );
		connect( m_connection, &VeyonConnection::featureMessageReceived, this, &ComputerControlInterface::resetWatchdog );

		setUpdateMode( updateMode );

		vncConnection->start();
	}
	else
	{
		vWarning() << "computer host address is empty!";
	}
}



void ComputerControlInterface::stop()
{
	if( m_connection )
	{
		m_connection->stopAndDeleteLater();
		m_connection = nullptr;
	}

	m_pingTimer.stop();
	m_connectionWatchdogTimer.stop();

	m_state = State::Disconnected;
}



bool ComputerControlInterface::hasValidFramebuffer() const
{
	return vncConnection() && vncConnection()->hasValidFramebuffer();
}



QSize ComputerControlInterface::screenSize() const
{
	if( vncConnection() )
	{
		return vncConnection()->image().size();
	}

	return {};
}



void ComputerControlInterface::setScaledFramebufferSize( QSize scaledFramebufferSize )
{
	m_scaledFramebufferSize = scaledFramebufferSize;

	if( vncConnection() )
	{
		vncConnection()->setScaledSize( m_scaledFramebufferSize );
	}
}



QImage ComputerControlInterface::scaledFramebuffer() const
{
	if( vncConnection() && vncConnection()->isConnected() )
	{
		return vncConnection()->scaledFramebuffer();
	}

	return {};
}



QImage ComputerControlInterface::framebuffer() const
{
	if( vncConnection() && vncConnection()->isConnected() )
	{
		return vncConnection()->image();
	}

	return {};
}



void ComputerControlInterface::setUserInformation( const QString& userLoginName, const QString& userFullName, int sessionId )
{
	if( userLoginName != m_userLoginName ||
		userFullName != m_userFullName ||
		sessionId != m_userSessionId )
	{
		m_userLoginName = userLoginName;
		m_userFullName = userFullName;
		m_userSessionId = sessionId;

		Q_EMIT userChanged();
	}
}



void ComputerControlInterface::updateScreens()
{
	lock();

	if( vncConnection() && state() == State::Connected )
	{
		VeyonCore::builtinFeatures().monitoringMode().queryScreens( { weakPointer() } );
	}
	else
	{
		setScreens({});
	}

	unlock();
}



void ComputerControlInterface::setScreens(const ScreenList& screens)
{
	if(screens != m_screens)
	{
		m_screens = screens;

		Q_EMIT screensChanged();
	}
}



void ComputerControlInterface::setActiveFeatures( const FeatureUidList& activeFeatures )
{
	if( activeFeatures != m_activeFeatures )
	{
		m_activeFeatures = activeFeatures;

		Q_EMIT activeFeaturesChanged();
	}
}



void ComputerControlInterface::updateActiveFeatures()
{
	lock();

	if( vncConnection() && state() == State::Connected )
	{
		VeyonCore::builtinFeatures().monitoringMode().queryActiveFeatures({weakPointer()});
	}
	else
	{
		setActiveFeatures( {} );
	}

	unlock();
}



void ComputerControlInterface::sendFeatureMessage( const FeatureMessage& featureMessage, bool wake )
{
	if( m_connection && m_connection->isConnected() )
	{
		m_connection->sendFeatureMessage( featureMessage, wake );
	}
}



bool ComputerControlInterface::isMessageQueueEmpty()
{
	if( vncConnection() && vncConnection()->isConnected() )
	{
		return vncConnection()->isEventQueueEmpty();
	}

	return true;
}



void ComputerControlInterface::setUpdateMode( UpdateMode updateMode )
{
	m_updateMode = updateMode;

	const auto computerMonitoringUpdateInterval = VeyonCore::config().computerMonitoringUpdateInterval();

	switch( m_updateMode )
	{
	case UpdateMode::Disabled:
		if( vncConnection() )
		{
			vncConnection()->setFramebufferUpdateInterval( UpdateIntervalDisabled );
		}
		break;

	case UpdateMode::Basic:
	case UpdateMode::Monitoring:
	case UpdateMode::Live:
		if( vncConnection() )
		{
			vncConnection()->setFramebufferUpdateInterval( ( m_updateMode == UpdateMode::Basic ||
															 m_updateMode == UpdateMode::Monitoring ) ?
															   computerMonitoringUpdateInterval : -1 );
		}
		break;
	}

	if( m_updateMode == UpdateMode::Basic )
	{
		vncConnection()->setSkipHostPing( true );
	}
}



ComputerControlInterface::Pointer ComputerControlInterface::weakPointer()
{
	return Pointer( this, []( ComputerControlInterface* ) { } );
}



void ComputerControlInterface::ping()
{
	VeyonCore::builtinFeatures().monitoringMode().ping({weakPointer()});
}



void ComputerControlInterface::resetWatchdog()
{
	if( state() == State::Connected )
	{
		m_pingTimer.start();
		m_connectionWatchdogTimer.start();
	}
}



void ComputerControlInterface::restartConnection()
{
	if( vncConnection() )
	{
		vDebug();
		vncConnection()->restart();

		m_connectionWatchdogTimer.stop();
	}
}



void ComputerControlInterface::updateState()
{
	lock();

	if( vncConnection() )
	{
		switch( vncConnection()->state() )
		{
		case VncConnection::State::Disconnected: m_state = State::Disconnected; break;
		case VncConnection::State::Connecting: m_state = State::Connecting; break;
		case VncConnection::State::Connected: m_state = State::Connected; break;
		case VncConnection::State::HostOffline: m_state = State::HostOffline; break;
		case VncConnection::State::ServerNotRunning: m_state = State::ServerNotRunning; break;
		case VncConnection::State::AuthenticationFailed: m_state = State::AuthenticationFailed; break;
		default: m_state = VncConnection::State::Disconnected; break;
		}
	}
	else
	{
		m_state = State::Disconnected;
	}

	unlock();
}



void ComputerControlInterface::updateUser()
{
	lock();

	if( vncConnection() && state() == State::Connected )
	{
		if( userLoginName().isEmpty() )
		{
			VeyonCore::builtinFeatures().monitoringMode().queryLoggedOnUserInfo( { weakPointer() } );
		}
	}
	else
	{
		setUserInformation( {}, {}, -1 );
	}

	unlock();
}



void ComputerControlInterface::handleFeatureMessage( const FeatureMessage& message )
{
	lock();
	VeyonCore::featureManager().handleFeatureMessage( weakPointer(), message );
	unlock();
}



QDebug operator<<(QDebug stream, ComputerControlInterface::Pointer computerControlInterface)
{
	stream << qUtf8Printable(computerControlInterface->computer().hostAddress());
	return stream;
}



QDebug operator<<(QDebug stream, const ComputerControlInterfaceList& computerControlInterfaces)
{
	QStringList hostAddresses;
	hostAddresses.reserve(computerControlInterfaces.size());
	for(const auto& computerControlInterface : computerControlInterfaces)
	{
		hostAddresses.append(computerControlInterface->computer().hostAddress());
	}

	stream << QStringLiteral("[%1]").arg(hostAddresses.join(QLatin1Char(','))).toUtf8().constData();

	return stream;
}
