#ifndef CLIENT_NETWORK_H_
# define CLIENT_NETWORK_H_

#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include "clientGuiInfo.h"
#include "Protocol.h"

class ClientGUI;
class QtClient;

class ClientNetwork
{
	QtClient *qtClient;
	ClientGUI *clientGui;
public:
	ClientNetwork();
	~ClientNetwork();

	void Connect(std::string const &, int const &port);
	void interpretProtocol(Protocol const &info);
	void Disconnect();
	void userUniqueAction(EProtocol const &eProto, std::string const&name);
	void userAuthRegist(std::string const &name, std::string const &pwd, EProtocol const &eProto);
	void waitResponse();
};

#endif /*!CLIENT_NETWORK_H_*/