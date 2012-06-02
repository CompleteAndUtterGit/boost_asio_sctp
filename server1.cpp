#include <boost/thread.hpp>

#include "SctpServer.h"

boost::asio::io_service g_IO_Service;

/**
 * Runs a Boost-ASIO IO service to be used by timers and servers
 * in other threads.  This function is run as a boost::thread from main().
 */
void RunIoService(void)
{
	boost::asio::io_service::work work(g_IO_Service);  // force IO_Service to keep running until explicitly stopped
	g_IO_Service.run();
}

int main(int argc, char* argv[])
{
	boost::thread IoServiceThread(RunIoService);

	CSctpServer myServer(g_IO_Service, boost::asio::ip::address_v4::any() );

	myServer.StartAccept();

	do
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	}
	while(1);
}

