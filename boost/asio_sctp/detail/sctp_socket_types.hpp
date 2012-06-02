//
// detail/sctp_socket_types.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Hal's Software, Inc. (info at halssoftware dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_TYPES_HPP
#define BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_TYPES_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/socket_types.hpp>

#include <boost/asio/detail/push_options.hpp>
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
# include <ws2sctp.h>  // use Bruce Cran's SctpDrv implementation
#else
# include <netinet/sctp.h>
#endif
#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_SCTP_DETAIL_SCTP_SOCKET_TYPES_HPP
