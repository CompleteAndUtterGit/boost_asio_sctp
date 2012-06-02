//
// sctp_socket_acceptor_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Hal's Software, Inc. (info at halssoftware dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SCTP_SCTP_SOCKET_ACCEPTOR_SERVICE_HPP
#define BOOST_ASIO_SCTP_SCTP_SOCKET_ACCEPTOR_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/socket_acceptor_service.hpp>
#include <boost/asio_sctp/detail/sctp_socket_types.hpp>
#include <boost/asio_sctp/detail/sctp_socket_ops.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio_sctp {

template <typename Protocol>
class sctp_socket_acceptor_service
  : public boost::asio::socket_acceptor_service<Protocol>
{
public:
   typedef typename boost::asio::socket_acceptor_service<Protocol>::implementation_type implementation_type;
   typedef typename boost::asio::socket_acceptor_service<Protocol>::endpoint_type endpoint_type;

   /// Construct a new socket acceptor service for the specified io_service.
   explicit sctp_socket_acceptor_service(boost::asio::io_service& io_service)
     : boost::asio::socket_acceptor_service<Protocol>(io_service)
   {
   }

   /// Add an endpoint to the set used by the acceptor.
  boost::system::error_code bind_add(implementation_type& impl,
      const endpoint_type& endpoint, boost::system::error_code& ec)
  {
    if (!is_open(impl))
    {
      ec = boost::asio::error::bad_descriptor;
      return ec;
    }

    boost::asio_sctp::detail::sctp_socket_ops::bind_add(native(impl), endpoint.data(), endpoint.size(), ec);
    return ec;
  }

  /// Remove an endpoint to the set used by the acceptor.
  boost::system::error_code bind_remove(implementation_type& impl,
      const endpoint_type& endpoint, boost::system::error_code& ec)
  {
    if (!is_open(impl))
    {
      ec = boost::asio::error::bad_descriptor;
      return ec;
    }

    boost::asio_sctp::detail::sctp_socket_ops::bind_remove(native(impl), endpoint.data(), endpoint.size(), ec);
    return ec;
  }
};

} // namespace asio_sctp
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_SCTP_SCTP_SOCKET_ACCEPTOR_SERVICE_HPP

