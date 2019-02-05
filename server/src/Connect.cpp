#include "Connect.h"

Connect::Connect(int port) : acceptor(io_service, tcp::endpoint(tcp::v4(), port))
{
	acceptClient();
}

Connect::~Connect()
{
}

void	Connect::acceptClient()
{
	NetworkUser	*user = new NetworkUser(io_service);

	acceptor.async_accept(user->getSocket(), boost::bind(&Connect::connectClient, this, user, boost::asio::placeholders::error));
}

void	Connect::connectClient(NetworkUser *user, const boost::system::error_code &ec)
{
	if (!ec)
	{
		user->setIp();
		users.push_back(user);
		user->startRead();
	}
	else
		delete user;
	acceptClient();
}

void	Connect::update()
{
	io_service.poll();
}

void	Connect::update_clients()
{
	int	i = 0;

	for (NetworkUser *tmp : users)
	{
		if (tmp->getConnected() == false)
		{
			tmp->getSocket().close();
			delete tmp;
			users.erase(users.begin() + i);
		}
		i++;
	}
}

NetworkUser	*Connect::getUserDisconnected(size_t &i) const
{
	while (i < users.size())
	{
		if (users[i]->getConnected() == false)
		{
			return (users[i]);
		}
		i++;
	}
	return NULL;
}

NetworkUser	*Connect::getUserByRequestId()
{
	for (NetworkUser *tmp : users)
	{
		if (tmp->getId() != -1)
			return (tmp);
	}
	return NULL;
}

NetworkUser	*Connect::getUser(const std::string &username) const
{
	for (NetworkUser *tmp : users)
	{
		if (tmp->getName().compare(username) == 0)
			return (tmp);
	}
	return (NULL);
}
