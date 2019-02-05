#include "Server.h"

Server::Server(int _port) : port(_port)
{
	fcts.emplace(std::make_pair(EProtocol::REGISTRATION, &Server::subscription));
	fcts.emplace(std::make_pair(EProtocol::CONNECTION, &Server::connection));
	fcts.emplace(std::make_pair(EProtocol::ADD_CONTACT, &Server::addContact));
	fcts.emplace(std::make_pair(EProtocol::DELETE_CONTACT, &Server::deleteContact));
	fcts.emplace(std::make_pair(EProtocol::CALL_CONTACT, &Server::callContact));
	fcts.emplace(std::make_pair(EProtocol::CALL_ACCEPTED, &Server::callAccepted));
	fcts.emplace(std::make_pair(EProtocol::END_CALL, &Server::endCall));
	fcts.emplace(std::make_pair(EProtocol::DISCONNECTION, &Server::disconnect));
}

Server::~Server()
{
	delete network;
}

void	Server::start()
{
	network = new Connect(port);
	while (1)
	{
		try
		{
			network->update();
			update_clients();
			network->update_clients();
			NetworkUser	*user = network->getUserByRequestId();
			if (user != NULL)
			{
				(this->*fcts[user->getId()])(user);
				user->id = -1;
			}
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

void	Server::update_clients()
{
	NetworkUser *user;
	size_t			i = 0;
	
	while (1)
	{
		user = network->getUserDisconnected(i);
		if (user == NULL)
			break;
		contacts.setConnected(user->getName(), false);
		i++;
	}
}

void	Server::subscription(NetworkUser *user)
{
	if (contacts.addUser(user->p.user, user->p.pass) == true)
	{
		p.id = EProtocol::OK_REGISTRATION;
		sendp = reinterpret_cast<char*>(&p);
		user->write(p);
	}
	else
	{
		p.id = EProtocol::NOK_REGISTRATION;
		user->write(p);
	}
}

void	Server::connection(NetworkUser *user)
{
	if (contacts.connectUser(user->p.user, user->p.pass))
	{
		friends.clear();
		std::vector<std::string>	tmp_friends;

		p.id = EProtocol::SEND_CONTACTS;
		tmp_friends = contacts.sendFriendList(user->p.user);
		user->setName(user->p.user);
		for (auto & tmp_friend : tmp_friends)
		{
			Friends	tmp;

			for (int j = 0; j < tmp_friend.size() && j < 29; j++)
			{
				tmp.friends[j] = tmp_friend[j];
				tmp.friends[j + 1] = '\0';
			}
			friends.push_back(tmp);
		}
		p.nb_contacts = friends.size();
		user->write(p);
		user->write(friends);
	}
	else
	{
		p.id = EProtocol::NOK_CONNECTION;
		user->write(p);
	}
}

void	Server::addContact(NetworkUser *user)
{
	if (contacts.exists(user->p.user) && contacts.addFriend(user->getName(), user->p.user, true))
	{
		p.id = EProtocol::ADD_CONTACT_OK;
		user->write(p);
		other.id = EProtocol::ADD_CONTACT;
		for (int i = 0; i < user->getName().size() && i < 29; i++)
		{
			other.user[i] = user->getName()[i];
			other.user[i + 1] = '\0';
		}
		NetworkUser *second_user = network->getUser(user->p.user);
		if (second_user != nullptr)
			second_user->write(other);
	}
	else
	{
		p.id = EProtocol::NOK_ADD_CONTACT;
		user->write(p);
	}
}

void	Server::deleteContact(NetworkUser *user)
{
	if (contacts.removeFriend(user->getName(), user->p.user, true))
	{
		p.id = EProtocol::DELETE_CONTACT;
		for (int i = 0; i < user->getName().size() && i < 29; i++)
		{
			p.user[i] = user->getName()[i];
			p.user[i + 1] = '\0';
		}
		NetworkUser *second_user = network->getUser(user->p.user);
		if (second_user != NULL)
			second_user->write(p);
	}
}

void	Server::callContact(NetworkUser *user)
{
	if (contacts.isConnected(user->p.user) == true && network->getUser(user->p.user)->getOccupied() == false)
	{
		user->setOccupied(true);
		p.id = EProtocol::SEND_IP;
		for (int i = 0; i <= 15 && i < network->getUser(user->p.user)->getIp().size(); i++)
		{
			p.ip[i] = network->getUser(user->p.user)->getIp()[i];
			p.ip[i + 1] = '\0';
		}
		user->setContacting(user->p.user);
		network->getUser(user->p.user)->setContacting(user->getName());
		user->write(p);
		other.id = EProtocol::RECEIVE_CALL;
		for (int i = 0; i < user->getName().size() && i < 29; i++)
		{
			other.user[i] = user->getName()[i];
			other.user[i + 1] = '\0';
		}
		for (int i = 0; i <= 15 && i < user->getIp().size(); i++)
		{
			other.ip[i] = user->getIp()[i];
			other.ip[i + 1] = '\0';
		}
		NetworkUser	*second_user = network->getUser(user->p.user);
		if (second_user != NULL)
		{
			second_user->setOccupied(true);
			second_user->write(other);
		}
	}
	else
	{
		p.id = EProtocol::CONTACT_BUSY;
		user->write(p);
	}
}

void	Server::callAccepted(NetworkUser *user)
{
	NetworkUser *dest = network->getUser(user->getContacting());

	if (dest != NULL)
	{
		p.id = EProtocol::CALL_ACCEPTED;
		dest->write(p);
	}
}

void	Server::endCall(NetworkUser *user)
{
	user->setOccupied(false);
	NetworkUser	*second_user = network->getUser(user->getContacting());
	if (second_user != NULL)
	{
		second_user->setOccupied(false);
		p.id = EProtocol::END_CALL;
		second_user->write(p);
		second_user->setContacting("");
	}
	user->setContacting("");
}

void	Server::disconnect(NetworkUser *user)
{
	contacts.setConnected(user->getName(), false);
	user->setName("");
}
