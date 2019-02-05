#include "QtGUI.h"
#include <QtWidgets>
#include <QMainWindow>
#include <QWidgetAction>
#include <iostream>
#include "Protocol.h"
#include "ClientGUI.h"

extern GUIInfo clientInfo;

void QtGUI::tryConnect()
{
	QObject * tmpOb = sender();
	connectionType type;
	if (tmpOb == connectButton)
		type = connectionType::AUTH;
	else if (tmpOb == registerButton)
		type = connectionType::REGISTER;
	else
		type = connectionType::CONNECTION;
	clientGUI->tryConnect(type, lineEdits[0]->text().toStdString(), lineEdits[1]->text().toStdString());
}

QtGUI::QtGUI(ClientGUI *gui)
{
	clientGUI = gui;
	setWindowTitle(tr("BabelGui v2.0"));
}

void QtGUI::initConnectingWid()
{
	connectingLayout = new QVBoxLayout;
	createVerticalGroupBox();
	createHorizontalGroupBox();

	QTextEdit *bigEditor = new QTextEdit;
	bigEditor->setPlainText(tr("Welcome to Babel!"));

	connectingLayout->addWidget(verticalGroupBox);
	connectingLayout->addWidget(horizontalGroupBox);
	connectingLayout->addWidget(bigEditor);
	connectingWidget = new QWidget;
	connectingWidget->setLayout(connectingLayout);
	setCentralWidget(connectingWidget);
}

void QtGUI::searchRequest()
{
	clientGUI->searchRequest(searchEdit->text().toStdString());
	searchEdit->clear();
}

void QtGUI::disconnectRequest()
{
	clientGUI->disconnectRequest();
}

void QtGUI::removeFriend()
{
	QObject * tmpOb = sender();
	std::string const &name = getFriendSlideName(tmpOb);
	clientGUI->removeFriend(name);
}

void QtGUI::initConnectedWid()
{
	friendSlides.clear();
	connectedLayout = new QVBoxLayout;
	searchEdit = new QLineEdit(this);
	QMenuBar *menuBar = new QMenuBar;
	QWidgetAction *widgetAction = new QWidgetAction(this);
	widgetAction->setDefaultWidget(searchEdit);
	QMenu * tabMenu = menuBar->addMenu("Add contact");
	tabMenu->addAction(widgetAction);
	QAction *actAdd = tabMenu->addAction("Add");
	QAction *actDisconnect = menuBar->addMenu(clientInfo.userName.c_str())->addAction("Disconnection");

	menuBar->addMenu(tabMenu);
	connectedLayout->setMenuBar(menuBar);

	connect(actAdd, SIGNAL(triggered()), this, SLOT(searchRequest()));
	connect(actDisconnect, SIGNAL(triggered()), this, SLOT(disconnectRequest()));

	for (std::string str : clientInfo.contactList)
		addFriend(str);
	connectedWidget = new QWidget;
	connectedWidget->setLayout(connectedLayout);
	setCentralWidget(connectedWidget);
}

std::string		QtGUI::getFriendSlideName(QObject *ob)
{
	for (FriendSlide *tmp : friendSlides)
	{
		if (ob == tmp->removeButton || ob == tmp->callButton)
			return tmp->name;
	}
	return "";
}

void QtGUI::receiveEndCall()
{
	clientGUI->receiveEndCall();
}

void QtGUI::endCall()
{
	clientGUI->endCall();
}

void QtGUI::callFriend()
{
	QObject * tmpOb = sender();
	std::string const &name = getFriendSlideName(tmpOb);
	clientGUI->callFriend(name);
}

void QtGUI::answerCall()
{
	clientGUI->answerCall();
}

void QtGUI::initCallWid()
{
	std::string name = clientInfo.friendName;
	callLayout = new QGridLayout;
	QLabel *tmpLabel = new QLabel;
	tmpLabel->setText(QString::fromStdString(name));
	QSlider *volumeSlider = new QSlider(this);
	volumeSlider->setRange(0, 100);
	volumeSlider->setFixedWidth(100);
	volumeSlider->setValue(100);
	QPushButton *tmpButton = new QPushButton("End call");
	QPushButton *tmpButton2 = new QPushButton("Mute");

	connect(tmpButton, SIGNAL(released()), this, SLOT(endCall()));

	callLayout->addWidget(tmpLabel, 0, 2);
	callLayout->addWidget(tmpButton, 2, 1);
	callLayout->addWidget(tmpButton2, 2, 2);
	callLayout->addWidget(volumeSlider, 2, 0);

	callWidget = new QWidget;
	callWidget->setLayout(callLayout);
	setCentralWidget(callWidget);
}

void QtGUI::initTryCallWid()
{
	std::string name = clientInfo.friendName;
	callLayout = new QGridLayout;
	QLabel *tmpLabel = new QLabel;
	tmpLabel->setText(QString::fromStdString("Calling : " + name + "..."));
	QPushButton *tmpButton = new QPushButton("End call");

	connect(tmpButton, SIGNAL(released()), this, SLOT(endCall()));

	callLayout->addWidget(tmpLabel, 0, 0);
	callLayout->addWidget(tmpButton, 1, 0);

	callWidget = new QWidget;
	callWidget->setLayout(callLayout);
	setCentralWidget(callWidget);
}

void QtGUI::initReceiveCallWid()
{
	std::string name = clientInfo.friendName;
	callLayout = new QGridLayout;
	QLabel *tmpLabel = new QLabel;
	tmpLabel->setText(QString::fromStdString("Incoming call : " + name));
	QPushButton *tmpButton = new QPushButton("Accept");
	QPushButton *tmpButton1 = new QPushButton("Refuse");

	connect(tmpButton, SIGNAL(released()), this, SLOT(answerCall()));
	connect(tmpButton1, SIGNAL(released()), this, SLOT(endCall()));

	callLayout->addWidget(tmpLabel, 0, 1);
	callLayout->addWidget(tmpButton, 1, 0);
	callLayout->addWidget(tmpButton1, 1, 2);

	callWidget = new QWidget;
	callWidget->setLayout(callLayout);
	setCentralWidget(callWidget);
}

void QtGUI::addFriend(std::string const &name)
{
	FriendSlide *friendSlide = new FriendSlide;
	QHBoxLayout *tmp = new QHBoxLayout;
	friendSlide->friendBox = new QGroupBox;
	friendSlide->name = name;
	friendSlide->nameLabel = new QLabel(this);
	friendSlide->nameLabel->setText(QString::fromStdString(name));
	friendSlide->callButton = new QPushButton("Call");
	friendSlide->removeButton = new QPushButton("Delete");
	connect(friendSlide->callButton, SIGNAL(released()), this, SLOT(callFriend()));
	connect(friendSlide->removeButton, SIGNAL(released()), this, SLOT(removeFriend()));
	tmp->addWidget(friendSlide->nameLabel);
	tmp->addWidget(friendSlide->callButton);
	tmp->addWidget(friendSlide->removeButton);
	friendSlide->friendBox->setLayout(tmp);
	connectedLayout->addWidget(friendSlide->friendBox);
	friends.emplace(name, friendSlide->friendBox);
	friendSlides.push_back(friendSlide);
}

void QtGUI::clearFriendSlide(int const &nb)
{
	delete friendSlides[nb]->nameLabel;
	delete friendSlides[nb]->callButton;
	delete friendSlides[nb]->removeButton;
	delete friendSlides[nb]->friendBox;
}

void QtGUI::delFriend(std::string const &name)
{
	connectedLayout->removeWidget(friends.at(name));
	friends.erase(name);
	int i = 0;
	int j = 0;
	for (FriendSlide *tmp : friendSlides)
	{
		if (tmp->name == name)
			break;
		i++;
	}
	clearFriendSlide(i);
	friendSlides.erase(friendSlides.begin() + i);
	for (int k = 0; k < clientInfo.contactList.size(); k++)
	{
		if (clientInfo.contactList[k] == name)
			i = k;
	}
	clientInfo.contactList.erase(clientInfo.contactList.begin() + i);
}

void QtGUI::closeEvent(QCloseEvent * event)
{
	clientGUI->closeEvent();
}

void QtGUI::initSelectIpAndPortWid()
{
	connectingToServLayout = new QVBoxLayout;
	verticalGroupBox = new QGroupBox;
	QVBoxLayout *layout = new QVBoxLayout;
	lineEdits[0] = new QLineEdit;
	lineEdits[1] = new QLineEdit;
	lineEdits[0]->setPlaceholderText("Server address");
	lineEdits[1]->setPlaceholderText("Port");
	lineEdits[1]->setEchoMode(QLineEdit::Password);
	layout->addWidget(lineEdits[0]);
	layout->addWidget(lineEdits[1]);
	verticalGroupBox->setLayout(layout);
	connectToServerButton = new QPushButton("Connection", this);
	connect(connectToServerButton, SIGNAL(released()), this, SLOT(tryConnect()));
	connectingToServLayout->addWidget(verticalGroupBox);
	connectingToServLayout->addWidget(connectToServerButton);
	connectingToServWidget = new QWidget;
	connectingToServWidget->setLayout(connectingToServLayout);
	setCentralWidget(connectingToServWidget);
}

void QtGUI::createHorizontalGroupBox()
{
	horizontalGroupBox = new QGroupBox;
	QHBoxLayout *layout = new QHBoxLayout;

	//for (int i = 0; i < NumButtons; ++i) {
	registerButton = new QPushButton("Register", this);
	connectButton = new QPushButton("Connection", this);
	layout->addWidget(registerButton);
	layout->addWidget(connectButton);
	horizontalGroupBox->setLayout(layout);

	//connectButton = buttons[1]->addAction(tr("tryConnect"));

	connect(connectButton, SIGNAL(released()), this, SLOT(tryConnect()));
	connect(registerButton, SIGNAL(released()), this, SLOT(tryConnect()));
}

void QtGUI::createVerticalGroupBox()
{
	verticalGroupBox = new QGroupBox;
	QVBoxLayout *layout = new QVBoxLayout;

	//for (int i = 0; i < NumButtons; ++i) {
	lineEdits[0] = new QLineEdit;
	lineEdits[1] = new QLineEdit;
	lineEdits[0]->setPlaceholderText("Username");
	lineEdits[1]->setPlaceholderText("Password");
	lineEdits[1]->setEchoMode(QLineEdit::Password);
	layout->addWidget(lineEdits[0]);
	layout->addWidget(lineEdits[1]);
	verticalGroupBox->setLayout(layout);
}

void QtGUI::messageBox(std::string const &titre, std::string const &content)
{
	QMessageBox::information(0, QString(titre.c_str()), QString(content.c_str()) , QMessageBox::Ok);
}
