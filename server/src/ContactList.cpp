#include "ContactList.h"

ContactList::ContactList()
{
	loadList();
}

ContactList::~ContactList()
{
}

void	ContactList::loadList()
{
	std::string line;
	std::ifstream myfile("file.txt");

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			std::vector<std::string>	tmp;
			std::string					str;
			int							i = 0;
			size_t						j = 0;

			while (j < line.size())
			{
				size_t k = line.find(" ", j);
				if (k == std::string::npos)
				{
					k = line.size();
				}
				str = line.substr(j, (k - j));

				if (i == 0)
				{
					usernames.push_back(std::make_pair(str, false));
				}
				else if (i == 1)
				{
					pwd.push_back(str);
				}
				else
				{
					tmp.push_back(str);
				}
				j = k + 1;
				i++;
			}
			friends.push_back(tmp);
		}
		myfile.close();
	}
}

bool	ContactList::addUser(const std::string &username, const std::string &_pwd)
{
	bool	newUser = true;

	for (const std::pair<std::string, bool> &tmp : usernames)
	{
		if (tmp.first == username)
			newUser = false;
	}
	if (newUser)
	{
		usernames.push_back(std::make_pair(username, false));
		pwd.push_back(_pwd);
		std::vector<std::string>	tmp;
		friends.push_back(tmp);
		updateFile();
	}
	return (newUser);
}

bool		ContactList::addFriend(const std::string &username, const std::string &_friend, const bool &rec)
{
	bool	add = false;
	bool	exists = false;
	int		i = 0;

	if (username.compare(_friend) != 0)
	{
		for (const std::pair<std::string, bool> &tmp : usernames)
		{
			if (tmp.first.compare(username) == 0)
			{
				for (const std::string &tmp_friends : friends[i])
				{
					if (tmp_friends.compare(_friend) == 0)
					{
						exists = true;
					}
				}
				if (exists == false)
				{
					friends[i].push_back(_friend);
					add = true;
					if (rec == true)
						addFriend(_friend, username, false);
				}
			}
			i++;
		}
	}
	if (add == true)
	{
		updateFile();
		return (true);
	}
	return (false);
}

bool	ContactList::removeFriend(const std::string &username, const std::string &_friend, const bool &rec)
{
	bool	exists = false;
	bool	f_exists = false;
	int		i = 0;

	for (const std::pair<std::string, bool> &tmp : usernames)
	{
		if (tmp.first.compare(username) == 0)
		{
			int j = 0;

			exists = true;
			for (const std::string &tmp_friends : friends[i])
			{
				if (tmp_friends.compare(_friend) == 0)
				{
					f_exists = true;
					friends[i].erase(friends[i].begin() + j);
					removeFriend(_friend, username, false);
				}
				j++;
			}
		}
		i++;
	}
	if (exists == true && f_exists == true)
	{
		updateFile();
		return (true);
	}
	return (false);
}

bool	ContactList::connectUser(const std::string &username, const std::string &password)
{
	int	i = 0;

	for (std::pair<std::string, bool> &tmp : usernames)
	{
		if (tmp.first.compare(username) == 0 && tmp.second == false)
		{
			if (pwd[i].compare(password) == 0)
			{
				usernames[i].second = true;
				return (true);
			}
		}
		i++;
	}
	return (false);
}

bool	ContactList::isConnected(const std::string &username)
{
	for (std::pair<std::string, bool> tmp : usernames)
	{
		if (tmp.first.compare(username) == 0)
			return (tmp.second);
	}
	return (false);
}

void	ContactList::setConnected(const std::string &username, const bool &connected)
{
	int	i = 0;
	for (std::pair<std::string, bool> tmp : usernames)
	{
		if (tmp.first.compare(username) == 0)
			usernames[i].second = connected;
		i++;
	}
}

bool	ContactList::exists(const std::string &username)
{
	for (std::pair<std::string, bool> tmp : usernames)
	{
		if (tmp.first.compare(username) == 0)
			return (true);
	}
	return (false);
}

void	ContactList::updateFile()
{
	std::fstream	file;
	unsigned int	i = 0;

	file.open("file.txt", std::fstream::out | std::fstream::trunc);
	while (i < usernames.size())
	{
		file << usernames[i].first << " " << pwd[i];
		for (const std::string &tmp : friends[i])
			file << " " << tmp;
		file << std::endl;
		i++;
	}
	file.close();
}

std::vector<std::string>	ContactList::sendFriendList(const std::string &name)
{
	int	i = 0;

	for (const std::pair<std::string, bool> &tmp : usernames)
	{
		if (tmp.first.compare(name) == 0)
			return (friends[i]);
		i++;
	}
	std::vector<std::string> a;
	return (a);
}
