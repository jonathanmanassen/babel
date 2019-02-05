#include "NetworkUser.h"

NetworkUser::NetworkUser(boost::asio::io_service &io_service) : socket(io_service)
{
}

NetworkUser::~NetworkUser()
{
}

void	NetworkUser::setIp()
{
	ip_address = socket.remote_endpoint().address().to_string();
}

void	NetworkUser::setIdProtocol()
{
	if (p.id >= 0 && p.id <= 16)
	{
		id = static_cast<int>(p.id);
	}
}

void	NetworkUser::startRead()
{
	socket.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(&p), sizeof(Protocol)),
		boost::bind(&NetworkUser::readClient, this, boost::asio::placeholders::error));
}

void	NetworkUser::readClient(const boost::system::error_code &ec)
{
	if (!((boost::asio::error::eof == ec) ||
		(boost::asio::error::connection_reset == ec)))
	{
		if (id == -1)
		{
			setIdProtocol();
		}
		startRead();
	}
	else
	{
		connected = false;
	}
}

void	NetworkUser::write(Protocol	p)
{
	socket.async_write_some(boost::asio::buffer(reinterpret_cast<char*>(&p), sizeof(Protocol)),
		boost::bind(&NetworkUser::writeClient, this, boost::asio::placeholders::error));
}

void	NetworkUser::write(std::vector<Friends>	p)
{
	socket.async_write_some(boost::asio::buffer(reinterpret_cast<char*>(&p[0]), p.size() * sizeof(Friends)),
		boost::bind(&NetworkUser::writeClient, this, boost::asio::placeholders::error));
}

void	NetworkUser::writeClient(const boost::system::error_code &ec)
{
	if ((boost::asio::error::eof == ec) ||
		(boost::asio::error::connection_reset == ec))
	{
		connected = false;
	}
}