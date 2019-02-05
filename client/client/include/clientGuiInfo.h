#ifndef CLIENT_GUI_INFO_H
# define CLIENT_GUI_INFO_H

#include <string>
#include <vector>

enum class WindStat
{
	UNKNONW,
	CONNECTING,
	CONNECTED,
	CALL
};

enum class GUIResquest
{
	UNKNOWN,
	AUTH,
	REGIST,
	ADDFRIEND,
	REMOVEFRIEND,
	DISCONNECTION,
	CALL,
	ENDCALL,
	CLOSE,
	ACCEPTED,
	REFUSED,
	NONE,
};

struct GUIInfo
{
	WindStat windStat;
	GUIResquest guiResquest;
	std::string userName;
	std::string passWord;
	std::string friendName;
	std::string ip;
	std::string friendIp;
	int port;
	std::vector<std::string> contactList;
	int connected;
};

#endif /*!CLIENT_GUI_INFO_H*/