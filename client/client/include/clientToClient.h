#ifndef CLIENTTOCLIENT_H
#define CLIENTTOCLIENT_H

#include <iostream>
#include <QObject>
#include <QHostAddress>
#include "audio.h"
#include "compression.h"
#include "QtGUI.h"

class QtGUI;
class QTimer;
class QUdpSocket;

class ClientToClient : public QObject
{
	Q_OBJECT
	QtGUI *qtGui;
public:
	ClientToClient(QtGUI*, QObject *parent = 0);
	~ClientToClient();
	void connectTo(std::string const &addr, int const &port);
	void listen(int port);

	private slots:
	void readAudio();
	void writeAudio();

private:
	QUdpSocket *udpSocket;
	QUdpSocket *udpSocketServer;
	QTimer *timer;
	QHostAddress ipsender;

	Audio audio;
	Compression compression;
	std::string iphost;
	int port;
	int enc_bytes;
	int const channels = 1;
	int const bufferSize = 480;
	int const sampleRate = 48000;
	std::vector<unsigned short> captured;
	std::vector<unsigned char> encodedSend;
	std::vector<unsigned char> encodedReceive;
	std::vector<unsigned short> decoded;
};

#endif /*!CLIENT_TO_CLIENT_H*/