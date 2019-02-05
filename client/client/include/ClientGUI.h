#ifndef CLIENT_GUI_H_
# define CLIENT_GUI_H_

#include <QWidget>
#include <QMainWindow>
#include <QLayout>
#include <QCloseEvent>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include "IGUI.h"
#include "clientGuiInfo.h"
#include "clientToClient.h"

class ClientNetwork;
class ClientToClient;
class QtGUI;

class ClientGUI : public IGUI
{
	ClientNetwork	*clientNetwork;
	QtGUI			*qtGUI;
public:
	ClientGUI(ClientNetwork *);
	void show();
	void initSelectIpAndPortWid();
	void initReceiveCallWid();
	void receiveEndCall();
	void addFriend(std::string const &);
	void delFriend(std::string const &);
	void disconnectedFromServer();
	virtual void initCallWid();

	virtual void tryConnect(connectionType const &, std::string const&, std::string const &);
	virtual void endCall();
	virtual void searchRequest(std::string const &name);
	virtual void disconnectRequest();
	virtual void removeFriend(std::string const &name);
	void callFriend(std::string const &name);
	void answerCall();
	void closeEvent();
	ClientToClient	*qtAudio = nullptr;
private:
	virtual void initConnectedWid();
	virtual void initTryCallWid();
	virtual void initConnectingWid();

	WindStat	windStat = WindStat::CONNECTING;
};

#endif /*!CLIENT_GUI_H_*/