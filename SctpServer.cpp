/**
*	@file
*	$Rev: 187 $
*	$Date: 2012-01-16 10:21:49 +0000 (Mon, 16 Jan 2012) $
*
*	@section Purpose
*
*	Implements a (partly) asynchronous SCTP server
*/

/* System includes */
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

/* Project / bespoke includes */
#include "SctpServer.h"

/* Macros / defines */
#define SCTP_MIN_LENGTH	4

#define TIME_TO_LIVE_MS		100
#define DEFAULT_CONTEXT		0

/**
 * CSctpConnection factory function
 *
 * @param IO_Service reference to the global Boost ASIO service
 *
 * @return a new SCTP connection object
 */
CSctpConnection* CSctpConnection::Create(boost::asio::io_service& IO_Service)
{
	return new CSctpConnection(IO_Service);
}

/**
 * Private constructor
 */
CSctpConnection::CSctpConnection(boost::asio::io_service& IO_Service)
	: m_Socket(IO_Service)
{
}

/**
 * Destructor
 */
CSctpConnection::~CSctpConnection(void)
{
#ifdef _DEBUG
	std::cout << "CSctpConnection destructor called" << std::endl;
#endif
}

void CSctpConnection::StartReceiving(void)
{
/*  Eventually, would like to be able to:
	async_read(m_Socket,
				boost::asio::buffer(RxBuffer, sizeof(RxBuffer)),
				boost::asio::transfer_at_least(SCTP_MIN_LENGTH),
				boost::bind(&CSctpConnection::OnReceive,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred,
							boost::asio_sctp::stream_number,
							boost::asio_sctp::payload_protocol_id);

    But for now, have to be content with:
*/
	boost::thread Receiver(boost::bind(&CSctpConnection::ReceiveLoop, this));
}

/**
 * Main (synchronous) receive loop
 */
void CSctpConnection::ReceiveLoop(void)
{
	while(m_Socket.is_open())
	{
		socklen_t len = sizeof(struct sockaddr_in);
		int msgFlags;
		int numBytes = sctp_recvmsg(m_Socket.native_handle(), (char*)RxBuffer, sizeof(RxBuffer), &m_PeerAddr, &len, &m_RxSRI, &msgFlags);
		boost::system::error_code ec;
		if(m_RxSRI.sinfo_flags & SCTP_EOF)
		{
			ec = boost::asio::error::eof;
		}
		else if(m_RxSRI.sinfo_flags & SCTP_ABORT)
		{
			ec = boost::asio::error::connection_reset;
		}
		UINT16 streamNum = ntohs(m_RxSRI.sinfo_stream);
		UINT32 payloadProtocolID = ntohl(m_RxSRI.sinfo_ppid);
		if(numBytes >= 0)
		{
			this->OnReceive(ec, numBytes, streamNum, payloadProtocolID);
		}
		boost::this_thread::yield();
	}
}

/**
 * Sends a raw byte sequence in an SCTP packet, with defined stream number and payload protocol ID.
 */
void CSctpConnection::Send(const BYTE* pData, size_t numBytes, UINT16 streamNum, UINT32 payloadProtocolID)
{
	if(m_Socket.is_open())
	{
		socklen_t len = sizeof(struct sockaddr);
		UINT32 txFlags = 0;
		sctp_sendmsg(m_Socket.native_handle(), pData, numBytes, &m_PeerAddr, len, htonl(payloadProtocolID), txFlags, htons(streamNum),
				TIME_TO_LIVE_MS, DEFAULT_CONTEXT);
	}
}

/**
 * Closes the socket associated with this connection and notifies the PicoMaster thread
 * that the connection has closed
 */
void CSctpConnection::Close(void)
{
	if(m_Socket.is_open())
	{
		m_Socket.shutdown(boost::asio::socket_base::shutdown_both);
		m_Socket.close();
		std::cout << "Connection closed" << std::endl;
	}
}

/**
 * Gets the IP address of the femtocell on the far end of this connection
 *
 * @param rPeerAddress reference to an ip::address output variable
 *
 * @return TRUE if the address was returned correctly, else FALSE
 */
bool CSctpConnection::GetPeerIpAddr(boost::asio::ip::address& rPeerAddress)
{
	boost::system::error_code ec;
	boost::asio_sctp::ip::sctp::endpoint peerEndpoint = m_Socket.remote_endpoint(ec);
	if(ec)
	{
		return false;
	}
	rPeerAddress = peerEndpoint.address();
	return true;
}

/**
 * Called when a minimum number of bytes have been received from the client.  Parses
 * the received packet and initiates the corresponding action and response
 *
 * @param Error the socket error condition - e.g. EOF for orderly disconnection
 * @param numBytes the number of bytes received
 */
void CSctpConnection::OnReceive(const boost::system::error_code& Error, size_t numBytes, UINT16 streamNum, UINT32 payloadProtocolID)
{
	if((Error == boost::asio::error::eof)  // received FIN (orderly closure) from client
		|| (Error == boost::asio::error::connection_reset))  // connection reset
	{
		std::cout << "CSctpConnection::OnReceive - connection closed by client" << std::endl;
		Close();
		return;
	}
	else if(Error.value() == ECANCELED)
	{
		std::cout << "CSctpConnection::OnReceive - Operation Aborted " << std::endl;
		Close();
		return;
	}
	else if(Error != 0)
	{
		std::cout << "CSctpConnection::OnReceive - error " << std::dec << Error.value() << std::endl;
		return;
	}

	std::cout << "CSctpConnection::OnReceive - numBytes = " << std::dec << numBytes << std::endl;
	BYTE version = RxBuffer[0];
	BYTE messageClass = RxBuffer[2];
	BYTE messageType = RxBuffer[3];
	// etc. - application-specific processing of received packet and formulation of response
}

/**
 * Constructor
 */
CSctpServer::CSctpServer(boost::asio::io_service& IO_Service, boost::asio::ip::address addr)
: m_Acceptor(IO_Service, boost::asio_sctp::ip::sctp::endpoint(addr, SERVER_PORT), true)
{
}

/**
 * Starts the server - creates a new unassigned connection and waits for a client to connect
 */
void CSctpServer::StartAccept(void)
{
	CSctpConnection* new_connection = CSctpConnection::Create(m_Acceptor.get_io_service());
	m_Acceptor.async_accept(new_connection->m_Socket,
		boost::bind(&CSctpServer::OnAccept, this, new_connection,
		boost::asio::placeholders::error));
	m_Acceptor.listen();  // bomb-out occurs even without this listen() being executed
}

/**
 * Called when a client device connects
 *
 * @param newConnection pointer to the associated connection object
 * @param error the acceptor-socket's error condition
 */
void CSctpServer::OnAccept(CSctpConnection* pNewConnection, const boost::system::error_code& error)
{
	if (!error)
	{
		boost::asio_sctp::ip::sctp::no_delay noDelayOption(true);  // disable Nagel algorithm - we need to send small packets in near-real time
		pNewConnection->m_Socket.set_option(noDelayOption);

		boost::asio_sctp::socket_option::sctp_ack_delay ackDelay(0);  // disable delayed-SACK algorithm
		pNewConnection->m_Socket.set_option(ackDelay);

		const struct sctp_event_subscribe subs = {1, 0, 0, 0, 0, 0, 0, 0, 0};
		boost::asio_sctp::socket_option::sctp_event_subscribe eventSubs(subs);
		pNewConnection->m_Socket.set_option(eventSubs);

		struct sctp_paddrparams params;
		memset((char*)&params, 0, sizeof(params));
		params.spp_hbinterval = 2000;
		params.spp_flags = SPP_HB_ENABLE;
		params.spp_pathmaxrxt = 3; // max 3 tries before considering connection unavailable
		boost::asio_sctp::socket_option::sctp_peer_addr_params peerParams(params);
		pNewConnection->m_Socket.set_option(peerParams);

		StartAccept();  // resume listening for other incoming connections
		std::cout << "CSctpServer::OnAccept" << std::endl;
	}
	else
	{
		std::cout << "CSctpServer::OnAccept - error " << std::dec << error.message() << std::endl;
	}
}

/**
 * Stops the server by closing the acceptor socket
 */
void CSctpServer::Stop(void)
{
	m_Acceptor.close();
}

