
#include <vector>
#include <QtNetwork>
#include "clientToClient.h"


ClientToClient::ClientToClient(QtGUI *_qtGui, QObject *parent) : QObject(parent)
{
	qtGui = _qtGui;
	if (compression.init() == 1)
		return;
	if (audio.init() == 1)
		return;
	
	timer = new QTimer(this);
	ipsender = nullptr;
	udpSocket = nullptr;
	udpSocketServer = nullptr;

	captured.resize(bufferSize * channels);
	decoded.resize(bufferSize * channels);
	encodedSend.resize(bufferSize * channels * 2);
	encodedReceive.resize(bufferSize * channels * 2);
}

ClientToClient::~ClientToClient()
{

}

void ClientToClient::connectTo(std::string const &addr, int const &port)
{
	qDebug() << "connectTo " << addr.c_str() << ":" << port;
	udpSocket = new QUdpSocket(this);
	iphost = addr;
	this->port = port;
	connect(timer, SIGNAL(timeout()), this, SLOT(writeAudio()));
	timer->start(0);
}

void ClientToClient::listen(int port)
{
	udpSocketServer = new QUdpSocket(this);
	udpSocketServer->bind(QHostAddress::Any, port);

	connect(udpSocketServer, SIGNAL(readyRead()), this, SLOT(readAudio()));
}


void ClientToClient::writeAudio()
{
	audio.read(captured);
	enc_bytes = compression.encode(captured, encodedSend);

	std::string enc_size = "enc" + std::to_string(enc_bytes);
	if (!iphost.empty())
	{
		udpSocket->writeDatagram(enc_size.c_str(), enc_size.size(), QHostAddress(iphost.c_str()), port);
		udpSocket->writeDatagram(reinterpret_cast<char*>(encodedSend.data()), encodedSend.size(), QHostAddress(iphost.c_str()), port);
	}
	else
	{
		udpSocket->writeDatagram(enc_size.c_str(), enc_size.size(), QHostAddress(ipsender.toString()), port);
		udpSocket->writeDatagram(reinterpret_cast<char*>(encodedSend.data()), encodedSend.size(), QHostAddress(ipsender.toString()), port);
	}
}

void ClientToClient::readAudio()
{
	while (udpSocketServer->hasPendingDatagrams())
	{
		encodedReceive.resize(udpSocketServer->pendingDatagramSize());
		udpSocketServer->readDatagram(reinterpret_cast<char*>(encodedReceive.data()), encodedReceive.size(), &ipsender, nullptr);
		if (encodedReceive.data()[0] == 'e' && encodedReceive.data()[1] == 'n' && encodedReceive.data()[2] == 'c')
		{
			std::string enc_size = reinterpret_cast<char*>(encodedReceive.data());
			enc_size = enc_size.substr(3);
			enc_bytes = std::atoi(enc_size.c_str());
		}
		else if (enc_bytes > 0)
		{
			compression.decode(encodedReceive, decoded, enc_bytes);
			audio.write(decoded);
			enc_bytes = 0;
			return;
		}
	}
}