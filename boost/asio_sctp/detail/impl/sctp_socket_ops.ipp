//
// detail/impl/sctp_socket_ops.ipp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Hal's Software, Inc. (info at halssoftware dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_OPS_IPP
#define BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_OPS_IPP

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
#define CONTEXT_0	0		// User Context, not currently used

namespace boost {
namespace asio_sctp {
namespace detail {
namespace sctp_socket_ops {

inline void clear_last_error()
{
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
  WSASetLastError(0);
#else
  errno = 0;
#endif
}

template <typename ReturnType>
inline ReturnType error_wrapper(ReturnType return_value,
    boost::system::error_code& ec)
{
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
  ec = boost::system::error_code(WSAGetLastError(),
      boost::asio::error::get_system_category());
#else
  ec = boost::system::error_code(errno,
      boost::asio::error::get_system_category());
#endif
  return return_value;
}

inline int call_bind_add(boost::asio::detail::socket_type s,
	const boost::asio::detail::socket_addr_type* addr, std::size_t /* addrlen */)
{
	return ::sctp_bindx(s, const_cast<boost::asio::detail::socket_addr_type*>(addr), 1, SCTP_BINDX_ADD_ADDR);
}

int bind_add(boost::asio::detail::socket_type s, const boost::asio::detail::socket_addr_type* addr,
	std::size_t addrlen, boost::system::error_code& ec)
{
	if (s == invalid_socket)
	{
		ec = boost::asio::error::bad_descriptor;
		return socket_error_retval;
	}

	clear_last_error();
	int result = error_wrapper(call_bind_add(s, addr, addrlen), ec);
	if (result == 0)
		ec = boost::system::error_code();
	return result;
}

inline int call_bind_remove(boost::asio::detail::socket_type s,
	const boost::asio::detail::socket_addr_type* addr, std::size_t /* addrlen */)
{
	return ::sctp_bindx(s, const_cast<boost::asio::detail::socket_addr_type*>(addr), 1, SCTP_BINDX_REM_ADDR);
}

int bind_remove(boost::asio::detail::socket_type s, const boost::asio::detail::socket_addr_type* addr,
	std::size_t addrlen, boost::system::error_code& ec)
{
	if (s == invalid_socket)
	{
		ec = boost::asio::error::bad_descriptor;
		return socket_error_retval;
	}

	clear_last_error();
	int result = error_wrapper(call_bind_remove(s, addr, addrlen), ec);
	if (result == 0)
		ec = boost::system::error_code();
	return result;
}

inline int call_getladdrs(boost::asio::detail::socket_type s,
					boost::asio::detail::socket_addr_type** addrs)
{
	return ::sctp_getladdrs(s, 0, addrs);
}

int getladdrs(boost::asio::detail::socket_type s, boost::asio::detail::socket_addr_type** addrs,
	boost::system::error_code& ec)
{
	if (s == invalid_socket)
	{
		ec = boost::asio::error::bad_descriptor;
		return socket_error_retval;
	}

	clear_last_error();
	int result = error_wrapper(call_getladdrs(s, addrs), ec);
	if (result == 0)
		ec = boost::system::error_code();
	return result;
}

void freeladdrs(boost::asio::detail::socket_addr_type* addrs)
{
	sctp_freeladdrs(addrs);
}

inline int call_getpaddrs(boost::asio::detail::socket_type s,
	boost::asio::detail::socket_addr_type** addrs)
{
	return ::sctp_getpaddrs(s, 0, addrs);
}

int getpaddrs(boost::asio::detail::socket_type s, boost::asio::detail::socket_addr_type** addrs,
	boost::system::error_code& ec)
{
	if (s == invalid_socket)
	{
		ec = boost::asio::error::bad_descriptor;
		return socket_error_retval;
	}

	clear_last_error();
	int result = error_wrapper(call_getpaddrs(s, addrs), ec);
	if (result == 0)
		ec = boost::system::error_code();
	return result;
}

void freepaddrs(boost::asio::detail::socket_addr_type* addrs)
{
	::sctp_freepaddrs(addrs);
}

inline int call_sctp_sendmsg(boost::asio::detail::socket_type s,
	const buf* bufs, size_t count,
	const boost::asio::detail::socket_addr_type* pAddr,
	uint16_t stream_no, uint32_t ppid, int flags)
{
	sockaddr* mutable_pAddr = (sockaddr*)pAddr;
	socklen_t tolen = sizeof(sockaddr);
	return sctp_sendmsg(s, const_cast<buf*>(bufs), count, mutable_pAddr, tolen, ppid, flags, stream_no, TIME_TO_LIVE_MS, CONTEXT_0);
}

int sendmsg(boost::asio::detail::socket_type s, const buf* bufs, size_t count,
	const boost::asio::detail::socket_addr_type* pAddr,
	uint16_t stream_no, uint32_t ppid, int flags, boost::system::error_code& ec)
{
	if (s == invalid_socket)
	{
		ec = boost::asio::error::bad_descriptor;
		return socket_error_retval;
	}

	clear_last_error();
	int result = error_wrapper(call_sctp_sendmsg(s, bufs, count, pAddr, stream_no, ppid, flags), ec);
	if (result == 0)
		ec = boost::system::error_code();
	return result;
}

inline int call_sctp_recvmsg(boost::asio::detail::socket_type s,
	const buf* bufs, size_t count,
	const boost::asio::detail::socket_addr_type* pAddr,
	uint16_t& stream_no, uint32_t& ppid, int* flags)
{
	sockaddr mutable_Addr = *pAddr;
	socklen_t fromlen = 0;
	struct sctp_sndrcvinfo sinfo;
	int result = sctp_recvmsg(s, const_cast<buf*>(bufs), count, &mutable_Addr, &fromlen, &sinfo, flags);
	stream_no = sinfo.sinfo_stream;
	ppid = sinfo.sinfo_ppid;
	return result;
}

int recvmsg(boost::asio::detail::socket_type s,
	buf* bufs, size_t count,
	const boost::asio::detail::socket_addr_type* pAddr,
	uint16_t& stream_no, uint32_t& ppid, int* flags, boost::system::error_code& ec)
{
	if (s == invalid_socket)
	{
		ec = boost::asio::error::bad_descriptor;
		return socket_error_retval;
	}

	clear_last_error();
	int result = error_wrapper(call_sctp_recvmsg(s, bufs, count, pAddr, stream_no, ppid, flags), ec);
	if (result == 0)
		ec = boost::system::error_code();
	return result;
}

} // namespace sctp_socket_ops
} // namespace detail
} // namespace asio_sctp
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_OPS_IPP
