#include "QtClient.h"
#include "clientGuiInfo.h"
#include <QTextCodec>
#include "Protocol.h"
#include <sstream>
#include <algorithm>
#include "ClientNetwork.h"

extern GUIInfo clientInfo;

QtClient::QtClient(ClientNetwork *net)
{
	clientNetwork = net;
}

QtClient::~QtClient()
{
}

void QtClient::Connect(std::string const &address, int const &port)
{
	socket = new QTcpSocket(this);
	socket->connectToHost(address.c_str(), port);

	if (socket->waitForConnected(3000))
	{
		clientInfo.guiResquest = GUIResquest::ACCEPTED;
		qDebug() << "Connected!";
		connect(socket, SIGNAL(readyRead()), this, SLOT(readAnwser()));
		connect(socket, SIGNAL(disconnected()), this, SLOT(Disconnect()));
	}
	else
	{
		clientInfo.guiResquest = GUIResquest::REFUSED;
		qDebug() << "Not connected : " << socket->error() << " : " << socket->errorString();
	}
}

void QtClient::Disconnect()
{
	socket->close();
	clientNetwork->Disconnect();
}

void QtClient::send(Protocol message)
{
	socket->write(reinterpret_cast<char*>(&message), sizeof(Protocol));
}

void QtClient::waitResponse()
{
	socket->waitForReadyRead(500);
}

void QtClient::readAnwser()
{
	QByteArray array;
	if (nb_contacts != 0)
	{
		std::vector<Friends>	tmp;

		tmp.resize(nb_contacts);
		socket->read(reinterpret_cast<char*>(&tmp[0]), nb_contacts * sizeof(Friends));
		for (auto & string : tmp)
			clientInfo.contactList.emplace_back(string.friends);
		nb_contacts = 0;
	}
	else
	{
		Protocol info;
		socket->read(reinterpret_cast<char*>(&info), sizeof(Protocol));
		clientNetwork->interpretProtocol(info);
	}
}