//
// sctp_stream_socket_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Hal's Software, Inc. (info at halssoftware dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SCTP_SCTP_STREAM_SOCKET_SERVICE_HPP
#define BOOST_ASIO_SCTP_SCTP_STREAM_SOCKET_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)
#include <boost/asio/error.hpp>
#include <boost/asio/io_service.hpp>

#include <boost/asio/stream_socket_service.hpp>
#include <boost/asio_sctp/detail/sctp_socket_types.hpp>
#include <boost/asio_sctp/detail/sctp_socket_ops.hpp>
#include <vector>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio_sctp {

template<typename Protocol>
class sctp_stream_socket_service: public boost::asio::stream_socket_service<
		Protocol> {
public:
	typedef typename boost::asio::stream_socket_service<Protocol>::implementation_type
			implementation_type;
	typedef typename boost::asio::stream_socket_service<Protocol>::endpoint_type
			endpoint_type;

	/// Construct a new stream socket service for the specified io_service.
	explicit sctp_stream_socket_service(boost::asio::io_service& io_service) :
		boost::asio::stream_socket_service<Protocol>(io_service)
	{
	}

	/// Get the local endpoints.
	void local_endpoints(const implementation_type& impl, std::vector<
			endpoint_type>& endpoints, boost::system::error_code& ec) const
	{
		endpoints.clear();

		if (!is_open(impl)) {
			ec = boost::asio::error::bad_descriptor;
			return;
		}

		typename endpoint_type::data_type *addrs;
		int cnt = detail::sctp_socket_ops::getladdrs(native(impl), &addrs, ec);
		if (cnt > 0)
		{
			endpoints.resize((typename std::vector<endpoint_type>::size_type) cnt);
		}

		for (int i = 0; i < cnt; ++i)
		{
			std::memcpy(endpoints[i].data(), addrs[i], endpoints[i].capacity());
		}

		detail::sctp_socket_ops::freeladdrs(addrs);
	}

	/// Get the remote endpoints.
	void remote_endpoints(const implementation_type& impl, std::vector<
			endpoint_type>& endpoints, boost::system::error_code& ec) const
	{
		endpoints.clear();

		if (!is_open(impl)) {
			ec = boost::asio::error::bad_descriptor;
			return;
		}

		typename endpoint_type::data_type *addrs;
		boost::asio::detail::socket_addr_type* pAddrs;
		boost::asio::detail::socket_type s = impl.socket_;
		int cnt = detail::sctp_socket_ops::getpaddrs(s, &pAddrs, ec);
		if (cnt > 0)
		{
			endpoints.resize(
					(typename std::vector<endpoint_type>::size_type) cnt);
		}

		for (int i = 0; i < cnt; ++i)
		{
			std::memcpy(endpoints[i].data(), (void*) &(pAddrs[i]),
					endpoints[i].capacity());
		}

		detail::sctp_socket_ops::freepaddrs(addrs);
	}

	size_t send_by_sctp(const implementation_type& impl,
			boost::asio::const_buffer* buf,
			const boost::asio::detail::socket_addr_type* pAddr,
			uint16_t stream_no, uint32_t ppid, int flags,
			boost::system::error_code& ec)
	{
		return detail::sctp_socket_ops::sendmsg(impl.socket_, buf, pAddr,
				stream_no, ppid, flags, ec);
	}

	size_t recv_by_sctp(const implementation_type& impl,
			boost::asio::mutable_buffer* buf,
			const boost::asio::detail::socket_addr_type* pAddr,
			uint16_t& stream_no, uint32_t& ppid, int flags,
			boost::system::error_code& ec)
	{
		return detail::sctp_socket_ops::recvmsg(impl.socket_, buf, pAddr,
				stream_no, ppid, flags, ec);
	}

};

} // namespace asio_sctp
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_SCTP_SCTP_STREAM_SOCKET_SERVICE_HPP
