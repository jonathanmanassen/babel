#ifndef QTCLIENT_H
# define QTCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <string>
#include <QtGUI.h>
#include <thread>
#include "Protocol.h"

class QtGUI;
class ClientNetwork;

class QtClient : QObject
{
	Q_OBJECT
	ClientNetwork *clientNetwork;
public:
	QtClient(ClientNetwork*);
	~QtClient();

	void Connect(std::string const &, int const &port);
	void send(Protocol message);
	void waitResponse();

public slots :
	void Disconnect();
	void readAnwser();

private:
	QTcpSocket *socket;
public:
	int	nb_contacts = 0;
};

#endif // !QTCLIENT_H
