//
// basic_socket_acceptor.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Hal's Software, Inc. (info at halssoftware dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SCTP_SCTP_SOCKET_ACCEPTOR_HPP
#define BOOST_ASIO_SCTP_SCTP_SOCKET_ACCEPTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/basic_socket_acceptor.hpp>
#include <boost/asio_sctp/sctp_socket_acceptor_service.hpp>
#include <boost/asio_sctp/detail/sctp_socket_types.hpp>
#include <boost/asio_sctp/detail/sctp_socket_ops.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio_sctp {

template <typename Protocol>
class sctp_socket_acceptor
  : public boost::asio::basic_socket_acceptor<Protocol, sctp_socket_acceptor_service<Protocol> >
{
public:
   typedef typename boost::asio::basic_socket_acceptor<Protocol, sctp_socket_acceptor_service<Protocol> >::protocol_type protocol_type;
   typedef typename boost::asio::basic_socket_acceptor<Protocol, sctp_socket_acceptor_service<Protocol> >::endpoint_type endpoint_type;
   typedef typename boost::asio::basic_socket_acceptor<Protocol, sctp_socket_acceptor_service<Protocol> >::native_type native_type;

   /// Construct an acceptor without opening it.
   explicit sctp_socket_acceptor(boost::asio::io_service& io_service)
     : boost::asio::basic_socket_acceptor<Protocol, sctp_socket_acceptor_service<Protocol> >(io_service)
   {
   }

   /// Construct an open acceptor.
   sctp_socket_acceptor(boost::asio::io_service& io_service,
       const protocol_type& protocol)
     : boost::asio::basic_socket_acceptor<Protocol, sctp_socket_acceptor_service<Protocol> >(io_service, protocol)
   {
   }

   /// Construct an acceptor opened on the given endpoint.
   sctp_socket_acceptor(boost::asio::io_service& io_service,
       const endpoint_type& endpoint, bool reuse_addr = true)
     : boost::asio::basic_socket_acceptor<Protocol, sctp_socket_acceptor_service<Protocol> >(io_service, endpoint, reuse_addr)
   {
   }

   /// Construct a basic_socket_acceptor on an existing native acceptor.
   sctp_socket_acceptor(boost::asio::io_service& io_service,
       const protocol_type& protocol,
       const native_type& native_acceptor)
     : boost::asio::basic_socket_acceptor<Protocol, sctp_socket_acceptor_service<Protocol> >(io_service, protocol, native_acceptor)
   {
   }

   /// Add an endpoint to the set used by the acceptor.
   /**
    * This function adds the specified local-machine endpoint to the set to
    * which the socket acceptor in bound.
    *
    * @param endpoint An endpoint on the local machine to add to the set to
    * which the socket acceptor is bound.
    *
    * @throws boost::system::system_error Thrown on failure.
    *
    * @par Example
    * @code
    * boost::asio::ip::sctp::acceptor acceptor(io_service);
    * acceptor.open(boost::asio::ip::sctp::v4());
    * acceptor.bind_add(boost::asio::ip::sctp::endpoint(12345));
    * @endcode
    */
   void bind_add(const endpoint_type& endpoint)
   {
     boost::system::error_code ec;
     this->service.bind_add(this->implementation, endpoint, ec);
     boost::asio::detail::throw_error(ec);
   }

   /// Add an endpoint to the set used by the acceptor.
   /**
    * This function adds the specified local-machine endpoint to the set to
    * which the socket acceptor in bound.
    *
    * @param endpoint An endpoint on the local machine to add to the set to
    * which the socket acceptor is bound.
    *
    * @param ec Set to indicate what error occurred, if any.
    *
    * @par Example
    * @code
    * boost::asio::ip::sctp::acceptor acceptor(io_service);
    * acceptor.open(boost::asio::ip::sctp::v4());
    * boost::system::error_code ec;
    * acceptor.bind_add(boost::asio::ip::sctp::endpoint(12345), ec);
    * if (ec)
    * {
    *   // An error occurred.
    * }
    * @endcode
    */
   boost::system::error_code bind_add(const endpoint_type& endpoint,
       boost::system::error_code& ec)
   {
     return this->service.bind_add(this->implementation, endpoint, ec);
   }

   /// Remove an endpoint to the set used by the acceptor.
   /**
    * This function removes the specified local-machine endpoint to the set to
    * which the socket acceptor in bound.
    *
    * @param endpoint An endpoint on the local machine to remove from the set to
    * which the socket acceptor is bound.
    *
    * @throws boost::system::system_error Thrown on failure.
    *
    * @par Example
    * @code
    * boost::asio::ip::sctp::acceptor acceptor(io_service);
    * // Code which calls bind and/or bind_add
    * acceptor.bind_remove(boost::asio::ip::sctp::endpoint(12345));
    * @endcode
    */
   void bind_remove(const endpoint_type& endpoint)
   {
     boost::system::error_code ec;
     this->service.bind_remove(this->implementation, endpoint, ec);
     boost::asio::detail::throw_error(ec);
   }

   /// Remove an endpoint to the set used by the acceptor.
   /**
    * This function removes the specified local-machine endpoint to the set to
    * which the socket acceptor in bound.
    *
    * @param endpoint An endpoint on the local machine to remove from the set to
    * which the socket acceptor is bound.
    *
    * @param ec Set to indicate what error occurred, if any.
    *
    * @par Example
    * @code
    * boost::asio::ip::sctp::acceptor acceptor(io_service);
    * // Code which calls bind and/or bind_add
    * boost::system::error_code ec;
    * acceptor.bind_remove(boost::asio::ip::sctp::endpoint(12345), ec);
    * if (ec)
    * {
    *   // An error occurred.
    * }
    * @endcode
    */
   boost::system::error_code bind_remove(const endpoint_type& endpoint,
       boost::system::error_code& ec)
   {
     return this->service.bind_remove(this->implementation, endpoint, ec);
   }

};

} // namespace asio_sctp
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_SCTP_SCTP_SOCKET_ACCEPTOR_HPP

