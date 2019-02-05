#ifndef IGUI_H
# define IGUI_H

#include <string>

enum class connectionType
{
	AUTH,
	REGISTER,
	CONNECTION,
};

class IGUI
{
private:
	virtual void tryConnect(connectionType const &, std::string const&, std::string const &) = 0;
	virtual void initCallWid() = 0;
	virtual void searchRequest(std::string const &) = 0;
	virtual void disconnectRequest() = 0;
	virtual void removeFriend(std::string const &) = 0;
	virtual void endCall() = 0;
	virtual void initConnectingWid() = 0;
	virtual void initConnectedWid() = 0;
};

#endif // !IGUI_H
#pragma once
