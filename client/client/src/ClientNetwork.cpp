#include "ClientNetwork.h"
#include "clientGuiInfo.h"
#include <QTextCodec>
#include "Protocol.h"
#include <sstream>
#include <algorithm>
#include "QtClient.h"
#include "ClientGUI.h"

extern GUIInfo clientInfo;

ClientNetwork::ClientNetwork()
{
	qtClient = new QtClient(this);
	clientGui = new ClientGUI(this);
	clientGui->show();
}

ClientNetwork::~ClientNetwork()
{
}

void ClientNetwork::Connect(std::string const &address, int const &port)
{
	qtClient->Connect(address, port);
}

void ClientNetwork::Disconnect()
{
	clientGui->disconnectedFromServer();
}

void ClientNetwork::userAuthRegist(std::string const &name, std::string const &pwd, EProtocol const &eProto)
{
	Protocol info;
	info.id = eProto;
	for (int i = 0; i < USERMAXSIZE && name[i]; i++)
	{
		info.user[i] = name[i];
		info.user[i + 1] = '\0';
	}
	for (int i = 0; i < PASSMAXSIZE && pwd[i]; i++)
	{ 
		info.pass[i] = pwd[i];
		info.pass[i + 1] = '\0';
	}

	qtClient->send(info);
}

void ClientNetwork::waitResponse()
{
	qtClient->waitResponse();
}

void ClientNetwork::interpretProtocol(Protocol const &info)
{
	switch (info.id)
	{
	case EProtocol::OK_REGISTRATION:
		clientInfo.guiResquest = GUIResquest::ACCEPTED;
		break;
	case EProtocol::NOK_REGISTRATION:
		clientInfo.guiResquest = GUIResquest::REFUSED;
		break;
	case EProtocol::NOK_CONNECTION:
		clientInfo.guiResquest = GUIResquest::REFUSED;
		break;
	case EProtocol::SEND_CONTACTS:
		qtClient->nb_contacts = info.nb_contacts;
		clientInfo.guiResquest = GUIResquest::ACCEPTED;
		break;
	case EProtocol::ADD_CONTACT:
		clientInfo.contactList.push_back(info.user);
		clientGui->addFriend(info.user);
		break;
	case EProtocol::DELETE_CONTACT:
		clientGui->delFriend(info.user);
		break;
	case EProtocol::ADD_CONTACT_OK:
		clientInfo.guiResquest = GUIResquest::ACCEPTED;
		clientInfo.contactList.push_back(clientInfo.friendName);
		break;
	case EProtocol::NOK_ADD_CONTACT:
		clientInfo.guiResquest = GUIResquest::REFUSED;
		break;
	case EProtocol::SEND_IP:
		if (info.ip[0] != '\0')
		{
			clientInfo.ip = info.ip;
			clientInfo.guiResquest = GUIResquest::ACCEPTED;
		}
		else
			clientInfo.guiResquest = GUIResquest::REFUSED;
		break;
	case EProtocol::CALL_ACCEPTED:
		clientGui->initCallWid();
		break;
	case EProtocol::CONTACT_BUSY:
		clientInfo.guiResquest = GUIResquest::REFUSED;
		break;
	case EProtocol::END_CALL:
		if (clientInfo.guiResquest == GUIResquest::ENDCALL)
			clientInfo.guiResquest = GUIResquest::ACCEPTED;
		else if (clientInfo.guiResquest == GUIResquest::NONE)
			clientGui->receiveEndCall();
		break;
	case EProtocol::RECEIVE_CALL:
		clientInfo.friendName = info.user;
		clientInfo.friendIp = info.ip;
		clientGui->initReceiveCallWid();
		break;
	default:
		clientInfo.guiResquest = GUIResquest::NONE;
		break;
	}
}

void ClientNetwork::userUniqueAction(EProtocol const &eProto, std::string const &name)
{
	Protocol info;
	info.id = eProto;
	if (eProto == ADD_CONTACT || eProto == DELETE_CONTACT || eProto == CALL_CONTACT)
		for (int i = 0; i < USERMAXSIZE && name[i]; i++)
		{
			info.user[i] = name[i];
			info.user[i + 1] = '\0';
		}
	if (eProto == DISCONNECTION)
		clientInfo.contactList.clear();
	qtClient->send(info);
}