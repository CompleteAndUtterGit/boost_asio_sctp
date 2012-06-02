//
// sctp_stream_socket.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009 Hal's Software, Inc. (info at halssoftware dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SCTP_SCTP_STREAM_SOCKET_HPP
#define BOOST_ASIO_SCTP_SCTP_STREAM_SOCKET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio_sctp/sctp_stream_socket_service.hpp>
#include <boost/asio_sctp/detail/sctp_socket_types.hpp>
#include <boost/asio_sctp/detail/sctp_socket_ops.hpp>
#include <vector>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
	namespace asio_sctp {

		template <typename Protocol>
		class sctp_stream_socket
			: public boost::asio::basic_stream_socket<Protocol, sctp_stream_socket_service<Protocol> >
		{
		public:
			typedef typename boost::asio::basic_stream_socket<Protocol, sctp_stream_socket_service<Protocol> >::protocol_type protocol_type;
			typedef typename boost::asio::basic_stream_socket<Protocol, sctp_stream_socket_service<Protocol> >::endpoint_type endpoint_type;
			typedef typename boost::asio::basic_stream_socket<Protocol, sctp_stream_socket_service<Protocol> >::native_type native_type;

			/// Construct a basic_stream_socket without opening it.
			explicit sctp_stream_socket(boost::asio::io_service& io_service)
				: boost::asio::basic_stream_socket<Protocol, sctp_stream_socket_service<Protocol> >(io_service)
			{
			}

			/// Construct and open a basic_stream_socket.
			sctp_stream_socket(boost::asio::io_service& io_service,
				const protocol_type& protocol)
				: boost::asio::basic_stream_socket<Protocol, sctp_stream_socket_service<Protocol> >(io_service, protocol)
			{
			}

			/// Construct a basic_stream_socket, opening it and binding it to the given
			/// local endpoint.
			sctp_stream_socket(boost::asio::io_service& io_service,
				const endpoint_type& endpoint)
				: boost::asio::basic_stream_socket<Protocol, sctp_stream_socket_service<Protocol> >(io_service, endpoint)
			{
			}

			/// Construct a basic_stream_socket on an existing native socket.
			sctp_stream_socket(boost::asio::io_service& io_service,
				const protocol_type& protocol,
				const native_type& native_socket)
				: boost::asio::basic_stream_socket<Protocol, sctp_stream_socket_service<Protocol> >(
				io_service, protocol, native_socket)
			{
			}

			/// Get the local endpoints of the socket.
			/**
			* This function is used to obtain the locally bound endpoints of the socket.
			*
			* @param endpoints A vector to hold the local endpoints of the socket.
			*
			* @throws boost::system::system_error Thrown on failure.
			*
			* @par Example
			* @code
			* boost::asio::ip::sctp::socket socket(io_service);
			* ...
			* std::vector<boost::asio::ip::sctp::endpoint> endpoints;
			* socket.local_endpoints(endpoints);
			* @endcode
			*/
			void local_endpoints(std::vector<endpoint_type> &endpoints) const
			{
				boost::system::error_code ec;
				this->service.local_endpoints(this->implementation, endpoints, ec);
				boost::asio::detail::throw_error(ec);
			}

			/// Get the local endpoints of the socket.
			/**
			* This function is used to obtain the locally bound endpoints of the socket.
			*
			* @param endpoints A vector to hold the local endpoints of the socket.
			* @param ec Set to indicate what error occurred, if any.
			*
			* @par Example
			* @code
			* boost::asio::ip::sctp::socket socket(io_service);
			* ...
			* boost::system::error_code ec;
			* std::vector<boost::asio::ip::sctp::endpoint> endpoints;
			* socket.local_endpoints(endpoints, ec);
			* if (ec)
			* {
			*   // An error occurred.
			* }
			* @endcode
			*/
			void local_endpoints(std::vector<endpoint_type> &endpoints,
				boost::system::error_code& ec) const
			{
				this->service.local_endpoints(this->implementation, endpoints, ec);
			}

			/// Get the remote endpoints of the socket.
			/**
			* This function is used to obtain the remote endpoints of the socket.
			*
			* @param endpoints A vector to hold the remote endpoints of the socket.
			*
			* @throws boost::system::system_error Thrown on failure.
			*
			* @par Example
			* @code
			* boost::asio::ip::sctp::socket socket(io_service);
			* ...
			* std::vector<boost::asio::ip::sctp::endpoint> endpoints;
			* socket.remote_endpoints(endpoints);
			* @endcode
			*/
			void remote_endpoints(std::vector<endpoint_type> &endpoints) const
			{
				boost::system::error_code ec;
				this->service.remote_endpoints(this->implementation, endpoints, ec);
				boost::asio::detail::throw_error(ec);
			}

			/// Get the remote endpoints of the socket.
			/**
			* This function is used to obtain the remote endpoints of the socket.
			*
			* @param endpoints A vector to hold the remote endpoints of the socket.
			* @param ec Set to indicate what error occurred, if any.
			*
			* @par Example
			* @code
			* boost::asio::ip::sctp::socket socket(io_service);
			* ...
			* boost::system::error_code ec;
			* std::vector<boost::asio::ip::sctp::endpoint> endpoints;
			* socket.remote_endpoints(endpoints, ec);
			* if (ec)
			* {
			*   // An error occurred.
			* }
			* @endcode
			*/
			void remote_endpoints(std::vector<endpoint_type> &endpoints,
				boost::system::error_code& ec) const
			{
				this->service.remote_endpoints(this->implementation, endpoints, ec);
			}

			/// Send some data on the socket.
			/**
			* This function is used to send data on the stream socket. The function
			* call will block until one or more bytes of the data has been sent
			* successfully, or an until error occurs.
			*
			* @param buffers One or more data buffers to be sent on the socket.
			*
			* @returns The number of bytes sent.
			*
			* @throws boost::system::system_error Thrown on failure.
			*
			* @note The send operation may not transmit all of the data to the peer.
			* Consider using the @ref write function if you need to ensure that all data
			* is written before the blocking operation completes.
			*
			* @par Example
			* To send a single data buffer use the @ref buffer function as follows:
			* @code
			* socket.send(boost::asio::buffer(data, size));
			* @endcode
			* See the @ref buffer documentation for information on sending multiple
			* buffers in one go, and how to use it with arrays, boost::array or
			* std::vector.
			*/
			template <typename ConstBufferSequence>
			std::size_t send(const ConstBufferSequence& buffers)
			{
				boost::asio::detail::buffer_sequence_adapter<boost::asio::const_buffer,
					ConstBufferSequence> bufs(buffers);
				boost::system::error_code ec;
				std::size_t s = this->get_service().send(
					this->get_implementation(), bufs, &(this->payloadInfo), 0, ec);
				boost::asio::detail::throw_error(ec, "send");
				return s;
			}

			/**
			* Sends data using a specified Stream Number and Payload Protocol ID.
			* */
			template <typename ConstBufferSequence>
			std::size_t send(const ConstBufferSequence& buffers,
								boost::asio::detail::socket_addr_type* pAddr,
								uint16_t stream_no, uint32_t ppid)
			{
				boost::system::error_code ec;
				std::size_t s = this->get_service().send_by_sctp(
					this->get_implementation(),	buffers, pAddr, stream_no, ppid, 0, ec);
				boost::asio::detail::throw_error(ec, "send");
				return s;
			}
		};

	} // namespace asio_sctp
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_SCTP_SCTP_STREAM_SOCKET_HPP

