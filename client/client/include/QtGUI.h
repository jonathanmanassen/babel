#ifndef QT_GUI_H_
# define QT_GUI_H_

#include <QWidget>
#include <QMainWindow>
#include <QLayout>
#include <QCloseEvent>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include "clientGuiInfo.h"
#include "clientToClient.h"

class QAction;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class ClientGUI;
class ClientToClient;

struct FriendSlide
{
	QLabel *nameLabel;
	QPushButton *callButton;
	QPushButton *removeButton;
	QGroupBox *friendBox;
	std::string name;
};

class QtGUI : public QMainWindow
{
	Q_OBJECT
	ClientGUI *clientGUI;
public:
	QtGUI(ClientGUI*);
	void initSelectIpAndPortWid();
	void initReceiveCallWid();
	ClientToClient	*qtAudio;
	void receiveEndCall();
	void addFriend(std::string const &);
	void delFriend(std::string const &);
	virtual void initCallWid();
	virtual void initConnectedWid();
	virtual void initTryCallWid();
	virtual void initConnectingWid();
	void messageBox(std::string const &titre, std::string const &content);

private slots:
	virtual void tryConnect();
	virtual void endCall();
	virtual void searchRequest();
	virtual void disconnectRequest();
	virtual void removeFriend();
	void callFriend();
	void answerCall();
private:
	std::string getFriendSlideName(QObject * ob);
	void createHorizontalGroupBox();
	void createVerticalGroupBox();
	void clearFriendSlide(int const &nb);
	void closeEvent(QCloseEvent*event);

	QWidget *mainWidget;
	QWidget *connectingWidget;
	QWidget *connectedWidget;
	QWidget *connectingToServWidget;
	QWidget *callWidget;
	QGroupBox *horizontalGroupBox;
	QGroupBox *verticalGroupBox;
	QLineEdit *lineEdits[2];
	QLineEdit *searchEdit;
	QPushButton *connectButton;
	QPushButton *registerButton;
	QPushButton *connectToServerButton;
	QVBoxLayout *connectedLayout;
	QVBoxLayout *connectingLayout;
	QVBoxLayout *connectingToServLayout;
	QGridLayout *callLayout;
	WindStat	windStat = WindStat::CONNECTING;
	std::map<std::string, QGroupBox*>	friends;
	std::vector<FriendSlide*> friendSlides;
};

#endif /*!QT_GUI_H_*/