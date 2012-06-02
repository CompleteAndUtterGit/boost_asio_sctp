//
// ip/sctp.hpp
// ~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Hal's Software, Inc. (info at halssoftware dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SCTP_IP_SCTP_HPP
#define BOOST_ASIO_SCTP_IP_SCTP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio_sctp/sctp_socket_acceptor.hpp>
#include <boost/asio_sctp/sctp_stream_socket.hpp>
#include <boost/asio_sctp/detail/sctp_socket_types.hpp>  // for definition of sctp_event_subscribe

typedef struct sctp_event_subscribe sctp_event_subs_t;

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio_sctp {
namespace ip {

/// Encapsulates the flags needed for SCTP.
/**
 * The boost::asio::ip::sctp class contains flags necessary for SCTP sockets.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Safe.
 *
 * @par Concepts:
 * Protocol, InternetProtocol.
 */
class sctp
{
public:
	/// The type of a SCTP endpoint.
	typedef boost::asio::ip::basic_endpoint<sctp> endpoint;

	/// The type of a resolver query.
	typedef boost::asio::ip::basic_resolver_query<sctp> resolver_query;

	/// The type of a resolver iterator.
	typedef boost::asio::ip::basic_resolver_iterator<sctp> resolver_iterator;

	/// Construct to represent the IPv4 SCTP protocol.
	static sctp v4()
	{
		return sctp(PF_INET);
	}

	/// Construct to represent the IPv6 SCTP protocol.
	static sctp v6()
	{
		return sctp(PF_INET6);
	}

	/// Obtain an identifier for the type of the protocol.
	int type() const
	{
		return SOCK_STREAM;
	}

	/// Obtain an identifier for the protocol.
	int protocol() const
	{
		return IPPROTO_SCTP;
	}

	/// Obtain an identifier for the protocol family.
	int family() const
	{
		return family_;
	}

	/// The SCTP socket type.
	typedef sctp_stream_socket<sctp> socket;

	/// The SCTP acceptor type.
	typedef sctp_socket_acceptor<sctp> acceptor;

	/// The SCTP resolver type.
	typedef boost::asio::ip::basic_resolver<sctp> resolver;

	/// The SCTP iostream type.
	typedef boost::asio::basic_socket_iostream<sctp> iostream;

	/// Socket option for disabling the Nagle algorithm.
	/**
	* Implements the IPPROTO_SCTP/SCTP_NODELAY socket option.
	*
	* @par Examples
	* Setting the option:
	* @code
	* boost::asio::ip::sctp::socket socket(io_service);
	* ...
	* boost::asio::ip::sctp::no_delay option(true);
	* socket.set_option(option);
	* @endcode
	*
	* @par
	* Getting the current option value:
	* @code
	* boost::asio::ip::sctp::socket socket(io_service);
	* ...
	* boost::asio::ip::sctp::no_delay option;
	* socket.get_option(option);
	* bool is_set = option.value();
	* @endcode
	*
	* @par Concepts:
	* Socket_Option, Boolean_Socket_Option.
	*/
#if defined(GENERATING_DOCUMENTATION)
	typedef implementation_defined no_delay;
#else
	typedef boost::asio::detail::socket_option::boolean<
		IPPROTO_SCTP, SCTP_NODELAY> no_delay;
#endif

	/// Compare two protocols for equality.
	friend bool operator==(const sctp& p1, const sctp& p2)
	{
		return p1.family_ == p2.family_;
	}

	/// Compare two protocols for inequality.
	friend bool operator!=(const sctp& p1, const sctp& p2)
	{
		return p1.family_ != p2.family_;
	}

private:
	// Construct with a specific family.
	explicit sctp(int family)
		: family_(family)
	{
	}

	int family_;
};

} // namespace ip

namespace socket_option {

class sctp_ack_delay
{
public:
	// Construct with a specific option value.
	explicit sctp_ack_delay(uint32_t delayMs)
	{
		sackInfo.sack_assoc_id = 0;
		if(delayMs == 0)
		{
			sackInfo.sack_delay = 1;  // dummy value, must be non-zero
			sackInfo.sack_freq = 1;  // sack_freq = 1 disables delayed-ACK algorithm
		}
		else
		{
			sackInfo.sack_delay = delayMs;
			sackInfo.sack_freq = 2;  // send SACK if 2 packets are received
		}
	}

	// Set the current value of the delay.
	sctp_ack_delay& operator=(uint32_t delayMs)
	{
		sackInfo.sack_assoc_id = 0;
		if(delayMs == 0)
		{
			sackInfo.sack_delay = 1;  // dummy value, must be non-zero
			sackInfo.sack_freq = 1;  // sack_freq = 1 disables delayed-ACK algorithm
		}
		else
		{
			sackInfo.sack_delay = delayMs;
			sackInfo.sack_freq = 2;  // send SACK if 2 packets are received
		}
		return *this;
	}

	// Get the current value of the delay.
	uint32_t value() const
	{
		return sackInfo.sack_delay;
	}

	// Get the level of the socket option.
	template <typename Protocol>
	int level(const Protocol&) const
	{
		return IPPROTO_SCTP;
	}

	// Get the name of the socket option.
	template <typename Protocol>
	int name(const Protocol&) const
	{
#if defined(WIN32)
		return SCTP_DELAYED_SACK;
#else
		return SCTP_DELAYED_ACK;
#endif
	}

	// Get the address of the data.
	template <typename Protocol>
	const int* data(const Protocol&) const
	{
		return (int*)&sackInfo;
	}

	// Get the size of the data.
	template <typename Protocol>
	std::size_t size(const Protocol&) const
	{
		return sizeof(sackInfo);
	}

private:
	struct sctp_sack_info sackInfo;
};

class sctp_event_subscribe
{
public:
	// Construct with a specific option value.
	explicit sctp_event_subscribe(const sctp_event_subs_t& newSubs)
	{
		memcpy(&eventSubs, &newSubs, sizeof(eventSubs));
	}

	// Set the current value of the structure
	sctp_event_subscribe& operator=(const sctp_event_subs_t newSubs)
	{
		memcpy(&eventSubs, &newSubs, sizeof(eventSubs));
		return *this;
	}

	// Get the current value of the Data_IO_Event flag
//	uint32_t value() const
//	{
//		return eventSubs.sctp_data_io_event;
//	}

	// Get the level of the socket option.
	template <typename Protocol>
	int level(const Protocol&) const
	{
		return IPPROTO_SCTP;
	}

	// Get the name of the socket option.
	template <typename Protocol>
	int name(const Protocol&) const
	{
		return SCTP_EVENTS;
	}

	// Get the address of the data.
	template <typename Protocol>
	const int* data(const Protocol&) const
	{
		return (int*)&eventSubs;
	}

	// Get the size of the data.
	template <typename Protocol>
	std::size_t size(const Protocol&) const
	{
		return sizeof(eventSubs);
	}

private:
	sctp_event_subs_t eventSubs;
};

class sctp_peer_addr_params
{
public:
	// Construct with a specific option value.
	explicit sctp_peer_addr_params(const sctp_paddrparams& newParams)
	{
		memcpy(&peerAddrParams, &newParams, sizeof(peerAddrParams));
	}

	// Set the current value of the structure
	sctp_peer_addr_params& operator=(const sctp_paddrparams newParams)
	{
		memcpy(&peerAddrParams, &newParams, sizeof(peerAddrParams));
		return *this;
	}

	// Get the level of the socket option.
	template <typename Protocol>
	int level(const Protocol&) const
	{
		return IPPROTO_SCTP;
	}

	// Get the name of the socket option.
	template <typename Protocol>
	int name(const Protocol&) const
	{
		return SCTP_PEER_ADDR_PARAMS;
	}

	// Get the address of the data.
	template <typename Protocol>
	const int* data(const Protocol&) const
	{
		return (int*)&peerAddrParams;
	}

	// Get the size of the data.
	template <typename Protocol>
	std::size_t size(const Protocol&) const
	{
		return sizeof(peerAddrParams);
	}

private:
	sctp_paddrparams peerAddrParams;
};

} // namespace socket_option
} // namespace asio_sctp
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_SCTP_IP_SCTP_HPP

