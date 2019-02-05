#pragma once

#ifndef SERVER_H_
# define SERVER_H_

#include <unordered_map>
#include <map>
#include <vector>
#include <iostream>
#include "Connect.h"
#include "ContactList.h"
#include "Protocol.h"

class Server;

typedef void (Server::*fct)(NetworkUser*);

class Server
{
public:
	Server(int port);
	~Server();
	void	start();
	void	update_clients();
	void	subscription(NetworkUser*);
	void	connection(NetworkUser*);
	void	addContact(NetworkUser*);
	void	deleteContact(NetworkUser*);
	void	callContact(NetworkUser*);
	void	callAccepted(NetworkUser*);
	void	endCall(NetworkUser*);
	void	disconnect(NetworkUser*);

private:
	std::unordered_map<int, fct>	fcts;
	ContactList			contacts;
	Connect				*network;
	int					port;
	std::string			str = "";
	std::string			add_del_friend = "";
	std::string			ip = "";
	std::vector<Friends>	friends;
	Protocol			p;
	Protocol			other;
	std::string			sendp;
};

#endif /* SERVER_H_ */