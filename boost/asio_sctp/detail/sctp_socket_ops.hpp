//
// sctp_socket_ops.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Hal's Software, Inc. (info at halssoftware dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_OPS_HPP
#define BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_OPS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio.hpp>
#include <boost/asio/detail/socket_ops.hpp>
#include <boost/asio/detail/buffer_sequence_adapter.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/detail/socket_types.hpp>

#include <boost/asio/detail/push_options.hpp>

#define TIME_TO_LIVE_MS	100

namespace boost {
namespace asio_sctp {
namespace detail {
namespace sctp_socket_ops {

#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
typedef WSABUF buf;
#else // defined(BOOST_WINDOWS) || defined(__CYGWIN__)
typedef iovec buf;
#endif // defined(BOOST_WINDOWS) || defined(__CYGWIN__)

BOOST_ASIO_DECL int bind_add(boost::asio::detail::socket_type s,
	const boost::asio::detail::socket_addr_type* addr,
	std::size_t addrlen, boost::system::error_code& ec);

BOOST_ASIO_DECL int bind_remove(boost::asio::detail::socket_type s,
	const boost::asio::detail::socket_addr_type* addr,
	std::size_t addrlen, boost::system::error_code& ec);

BOOST_ASIO_DECL int getladdrs(boost::asio::detail::socket_type s,
	boost::asio::detail::socket_addr_type** addrs,
	boost::system::error_code& ec);

BOOST_ASIO_DECL void freeladdrs(boost::asio::detail::socket_addr_type* addrs);

BOOST_ASIO_DECL int getpaddrs(boost::asio::detail::socket_type s,
	boost::asio::detail::socket_addr_type** addrs,
	boost::system::error_code& ec);

BOOST_ASIO_DECL void freepaddrs(boost::asio::detail::socket_addr_type* addrs);

BOOST_ASIO_DECL int sendmsg(boost::asio::detail::socket_type s,
	const buf* bufs, size_t count,
	const boost::asio::detail::socket_addr_type* pAddr,
	uint16_t stream_no, uint32_t ppid, int flags, boost::system::error_code& ec);

BOOST_ASIO_DECL int recvmsg(boost::asio::detail::socket_type s,
	buf* bufs, size_t count,
	const boost::asio::detail::socket_addr_type* pAddr,
	uint16_t& stream_no, uint32_t& ppid, int* flags, boost::system::error_code& ec);

} // namespace sctp_socket_ops
} // namespace detail
} // namespace asio_sctp
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_OPS_HPP
