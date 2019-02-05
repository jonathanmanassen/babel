#pragma once

#ifndef CONNECT_H_
# define CONNECT_H_

#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "NetworkUser.h"

using boost::asio::ip::tcp;

class Connect
{
public:
	Connect(int);
	~Connect();
	void	acceptClient();
	void	connectClientToClient() {}
	void	connectClient(NetworkUser*, const boost::system::error_code&);
	void	update();
	void	update_clients();

private:
	std::vector<NetworkUser*>		users;
	boost::asio::io_service			io_service;
	tcp::acceptor					acceptor;

public:
	NetworkUser	*getUserDisconnected(size_t&) const;
	NetworkUser	*getUserByRequestId();
	NetworkUser	*getUser(const std::string&) const;
};

#endif /* CONNECT_H_ */
