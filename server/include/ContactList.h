#pragma once

#ifndef CONTACTLIST_H_
# define CONTACTLIST_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class ContactList
{
public:
	ContactList();
	~ContactList();

private:
	std::vector<std::pair<std::string, bool>>	usernames;
	std::vector<std::string>					pwd;
	std::vector<std::vector<std::string>>		friends;

public:
	std::vector<std::string>	sendFriendList(const std::string&);
	bool	addUser(const std::string&, const std::string&);
	bool	addFriend(const std::string&, const std::string&, const bool&);
	bool	removeFriend(const std::string&, const std::string&, const bool&);
	bool	connectUser(const std::string&, const std::string&);
	bool	isConnected(const std::string&);
	void	setConnected(const std::string&, const bool&);
	bool	exists(const std::string&);
	void	updateFile();
	void	loadList();
};

#endif /* CONTACTLIST_H_ */