/**
*	@file
*	$Rev: 148 $
*	$Date: 2011-10-26 12:17:59 +0100 (Wed, 26 Oct 2011) $
*
*	@section Purpose
*
*	Declarations related to the SCTP server
*/

/** Multiple-include guard */
#ifndef _SCTP_SERVER_H_
#define _SCTP_SERVER_H_

/* System includes */
#include <boost/asio.hpp>
#include <boost/asio_sctp/ip/sctp.hpp>
#include <boost/noncopyable.hpp>
#include <list>
#include <string>

/* Project / bespoke includes */
/* None */

/* Macros / defines */
#define SERVER_PORT	54321

typedef unsigned char BYTE;
typedef unsigned short UINT16;
typedef unsigned long UINT32;

/* External global declarations */
/* None */

/* External function prototypes */
/* None */

/* Class definitions */

/**
*	@class CFemtoConnection
*	Encapsulates an SCTP connection
*/
class CSctpConnection : public boost::noncopyable
{
friend class CSctpServer;

public:
	static CSctpConnection* Create(boost::asio::io_service& IO_Service);
	~CSctpConnection(void);
	void StartReceiving(void);
	void Close(void);
	void Send(const BYTE* pData, size_t numBytes, UINT16 streamNum, UINT32 payloadProtocolID);
	bool GetPeerIpAddr(boost::asio::ip::address& rPeerAddress);

private:
	CSctpConnection(boost::asio::io_service& IO_Service);
	void OnReceive(const boost::system::error_code& Error, size_t numBytes, UINT16 streamNum, UINT32 payloadProtocolID);
	void ReceiveLoop(void);
	BYTE RxBuffer[1024];
	BYTE TxBuffer[1024];
	boost::asio_sctp::ip::sctp::socket m_Socket;
	struct sctp_sndrcvinfo m_RxSRI;
	struct sockaddr m_PeerAddr;
};

/**
*	@class CSctpServer
*	Implements the server which creates new SCTP connections.
*/
class CSctpServer : public boost::noncopyable
{
public:
	CSctpServer(boost::asio::io_service& IO_Service, boost::asio::ip::address addr);
	void StartAccept(void);
	void Stop(void);

private:
	void OnAccept(CSctpConnection* pNewConnection, const boost::system::error_code& error);
	boost::asio_sctp::ip::sctp::acceptor m_Acceptor;
//	boost::asio_sctp::sctp_socket_acceptor<boost::asio_sctp::ip::sctp> m_Acceptor;
};

#endif  /* _SCTP_SERVER_H_ */
