#pragma once

#ifndef NETWORKUSER_H_
# define NETWORKUSER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "Protocol.h"

class NetworkUser
{
public:
	NetworkUser(boost::asio::io_service&);
	~NetworkUser();
	void	startRead();
	void	readClient(const boost::system::error_code&);
	void	write(Protocol);
	void	write(std::vector<Friends>);
	void	writeClient(const boost::system::error_code&);

private:
	boost::asio::ip::tcp::socket	socket;
public:
	char							str[1024] = "";
	bool							connected = true;
	std::string						ip_address = "";
	int								id = -1;
	bool							occupied = false;
	std::string						username = "";
	std::string						contacting = "";
	Protocol						p;

public:
	boost::asio::ip::tcp::socket	&getSocket() { return socket; }
	const bool						&getConnected() const { return connected; }
	const int						&getId() const { return id; }
	const bool						&getOccupied() const { return occupied; }
	const std::string				&getIp() const { return ip_address; }
	const std::string				&getName() const { return username; }
	const std::string				&getContacting() const { return contacting; }
	void							setOccupied(const bool &_occupied) { occupied = _occupied; }
	void							setName(const std::string &name) { username = name; }
	void							setId(const int &_id) { id = _id; }
	void							setContacting(const std::string &_contacting) { contacting = _contacting; }
	void							setConnected(const bool &_connected) { connected = _connected; }
	void							setIp();
	void							setIdProtocol();
};

#endif /* NETWORKUSER_H_ */