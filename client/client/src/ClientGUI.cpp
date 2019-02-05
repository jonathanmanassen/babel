#include "ClientGUI.h"
#include "clientGUIInfo.h"
#include <QtWidgets>
#include <QMainWindow>
#include <QWidgetAction>
#include <iostream>
#include "Protocol.h"
#include "ClientNetwork.h"
#include "QtGUI.h"

GUIInfo clientInfo;

void ClientGUI::tryConnect(connectionType const &type, std::string const &field1, std::string const &field2)
{
	std::string titre = "Connection Information";
	std::string noLogMessage;
	std::string connectedMessage;
	std::string badLogMessage;
	switch (type)
	{
	case connectionType::AUTH:
		noLogMessage = "Please enter username and password.";
		connectedMessage = "Connection successful. Welcome ";
		badLogMessage = "Connection refused : username or password is incorrect.";
		break;
	case connectionType::CONNECTION:
		noLogMessage = "Please enter valid server address and port.";
		connectedMessage = "Connection successful. Welcome of the server.";
		badLogMessage = "Connexion failed : Please enter valid server address and port";
		break;
	case connectionType::REGISTER:
		noLogMessage = "Please enter username and password.";
		connectedMessage = "Register successful. You can login now, ";
		badLogMessage = "Register refused : username already exist.";
		break;
	}
	if (field1.size() == 0 || field2.size() == 0)
		qtGUI->messageBox(titre, noLogMessage);
	else
	{
		switch (type)
		{
		case connectionType::AUTH:
			clientInfo.userName = field1;
			clientInfo.passWord = field2;
			clientInfo.guiResquest = GUIResquest::AUTH;
			clientNetwork->userAuthRegist(clientInfo.userName, clientInfo.passWord, EProtocol::CONNECTION);
			break;
		case connectionType::REGISTER:
			clientInfo.userName = field1;
			clientInfo.passWord = field2;
			clientInfo.guiResquest = GUIResquest::REGIST;
			clientNetwork->userAuthRegist(clientInfo.userName, clientInfo.passWord, EProtocol::REGISTRATION);
			break;
		case connectionType::CONNECTION:
			clientInfo.ip = field1;
			clientInfo.port = atoi(field2.c_str());
			clientInfo.guiResquest = GUIResquest::AUTH;
			clientNetwork->Connect(clientInfo.ip, clientInfo.port);
			break;
		}
		if (type == connectionType::AUTH || type == connectionType::REGISTER)
			clientNetwork->waitResponse();
		if (clientInfo.guiResquest == GUIResquest::ACCEPTED)
		{
			std::string tmp = connectedMessage + field1;
			if (type == connectionType::REGISTER)
				qtGUI->messageBox(titre, tmp);
			switch (type)
			{
			case connectionType::AUTH:
				qtAudio = new ClientToClient(qtGUI);
				initConnectedWid();
				break;
			case connectionType::CONNECTION:
				initConnectingWid();
				break;
			}
		}
		else
			qtGUI->messageBox(titre, badLogMessage);
	}
	clientInfo.guiResquest = GUIResquest::NONE;
}

ClientGUI::ClientGUI(ClientNetwork *net)
{
	clientNetwork = net;
	qtGUI = new QtGUI(this);
	clientInfo.windStat = WindStat::CONNECTING;
	clientInfo.guiResquest = GUIResquest::NONE;
	clientInfo.userName = "";
	clientInfo.friendName = "";
	clientInfo.passWord = "";
	clientInfo.connected = 0;
	initSelectIpAndPortWid();
}

void ClientGUI::show()
{
	qtGUI->show();
}

void ClientGUI::initConnectingWid()
{
	qtGUI->initConnectingWid();
}

void ClientGUI::searchRequest(std::string const &name)
{
	if (name.size() != 0)
	{
		clientInfo.friendName = name;
		clientInfo.guiResquest = GUIResquest::ADDFRIEND;
		clientNetwork->userUniqueAction(EProtocol::ADD_CONTACT, clientInfo.friendName);
		clientNetwork->waitResponse();
		if (clientInfo.guiResquest == GUIResquest::ACCEPTED)
			addFriend(name);
		else
			qtGUI->messageBox("Connection Information", "Error : this contact is already in your contact list or doesn't exist.");
	}
	clientInfo.guiResquest = GUIResquest::NONE;
}

void ClientGUI::disconnectRequest()
{
	clientInfo.guiResquest = GUIResquest::DISCONNECTION;
	clientNetwork->userUniqueAction(EProtocol::DISCONNECTION, "");
	initConnectingWid();
	clientInfo.guiResquest = GUIResquest::NONE;
}

void ClientGUI::removeFriend(std::string const &name)
{
	clientInfo.friendName = name;
	clientInfo.guiResquest = GUIResquest::REMOVEFRIEND;
	clientNetwork->userUniqueAction(EProtocol::DELETE_CONTACT, name);
	delFriend(name);
	clientInfo.guiResquest = GUIResquest::NONE;
}

void ClientGUI::initConnectedWid()
{
	qtGUI->initConnectedWid();
}

void ClientGUI::disconnectedFromServer()
{
	if (qtAudio != nullptr)
		delete qtAudio;
	qtAudio = new ClientToClient(qtGUI);
	clientInfo.windStat = WindStat::CONNECTING;
	clientInfo.guiResquest = GUIResquest::NONE;
	clientInfo.userName = "";
	clientInfo.friendName = "";
	clientInfo.passWord = "";
	clientInfo.connected = 0;
	clientInfo.contactList.clear();
	initSelectIpAndPortWid();
}

void ClientGUI::receiveEndCall()
{
	delete qtAudio;
	qtAudio = new ClientToClient(qtGUI);
	initConnectedWid();
	clientInfo.guiResquest = GUIResquest::NONE;
}

void ClientGUI::endCall()
{
	//ici close appel
	clientInfo.guiResquest = GUIResquest::ENDCALL;
	clientNetwork->userUniqueAction(EProtocol::END_CALL, "");
	delete qtAudio;
	qtAudio = new ClientToClient(qtGUI);
	//qtClient->waitResponce();
	initConnectedWid();
	clientInfo.guiResquest = GUIResquest::NONE;
}

void ClientGUI::callFriend(std::string const &name)
{
	clientInfo.friendName = name;
	clientInfo.guiResquest = GUIResquest::CALL;
	clientNetwork->userUniqueAction(EProtocol::CALL_CONTACT, clientInfo.friendName);
	clientNetwork->waitResponse();
	if (clientInfo.guiResquest == GUIResquest::ACCEPTED)
	{
		qtAudio->connectTo(clientInfo.ip, clientInfo.port);
		qtAudio->listen(clientInfo.port);
		initTryCallWid();
	}
	else
		qtGUI->messageBox("Call Information", "This contact is unreachable now.");
	clientInfo.guiResquest = GUIResquest::NONE;
}

void ClientGUI::answerCall()
{
	clientNetwork->userUniqueAction(EProtocol::CALL_ACCEPTED, "");
	qtAudio->listen(clientInfo.port);
	qtAudio->connectTo(clientInfo.friendIp, clientInfo.port);
	initCallWid();
}

void ClientGUI::initCallWid()
{
	qtGUI->initCallWid();
}

void ClientGUI::initTryCallWid()
{
	qtGUI->initTryCallWid();
}

void ClientGUI::initReceiveCallWid()
{
	qtGUI->initReceiveCallWid();
}

void ClientGUI::addFriend(std::string const &name)
{
	qtGUI->addFriend(name);
}

void ClientGUI::delFriend(std::string const &name)
{
	qtGUI->delFriend(name);
}

void ClientGUI::closeEvent()
{
	clientInfo.guiResquest = GUIResquest::CLOSE;
}

void ClientGUI::initSelectIpAndPortWid()
{
	qtGUI->initSelectIpAndPortWid();
}
