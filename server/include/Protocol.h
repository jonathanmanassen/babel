#pragma once

/*enum Protocol
{
    REGISTRATION,                //envoyer 0:user:pass
    CONNECTION,                    //envoyer 1:user:pass
    OK_REGISTRATION,            //envoyer 2
    NOK_REGISTRATION,            //envoyer 3
    SEND_CONTACTS,                //envoyer 4:contact1:contact2:...:contactn  (équivalent de OK_CONNECTION)
    NOK_CONNECTION,                //envoyer 5
    ADD_CONTACT,                //envoyer 6:user
	ADD_CONTACT_OK,                //envoyer 7
    NOK_ADD_CONTACT,            //envoyer 8
    DELETE_CONTACT,                //envoyer 9:user
    CALL_CONTACT,                //envoyer 10:user
    SEND_IP,                    //envoyer 11:ip     (envoyer à celui qui appelle)
    RECEIVE_CALL,                //envoyer 12:name:ip        (envoyer à celui qui reçoit)
    CALL_ACCEPTED,		        //envoyer 13
    CONTACT_BUSY,                //envoyer 14
    END_CALL,                    //envoyer 15        (à recevoir du client qui termine l'appel puis à envoyer au second)
    DISCONNECTION,                //envoyer 16
};*/

enum EProtocol
{
	REGISTRATION,                //envoyer 0:user:pass
	CONNECTION,                    //envoyer 1:user:pass
	OK_REGISTRATION,            //envoyer 2
	NOK_REGISTRATION,            //envoyer 3
	SEND_CONTACTS,                //envoyer 4:contact1:contact2:...:contactn  (équivalent de OK_CONNECTION)
	NOK_CONNECTION,                //envoyer 5
	ADD_CONTACT,                //envoyer 6:user
	ADD_CONTACT_OK,                //envoyer 7
	NOK_ADD_CONTACT,            //envoyer 8
	DELETE_CONTACT,                //envoyer 9:user
	CALL_CONTACT,                //envoyer 10:user
	SEND_IP,                    //envoyer 11:ip     (envoyer à celui qui appelle)
	RECEIVE_CALL,                //envoyer 12:name:ip        (envoyer à celui qui reçoit)
	CALL_ACCEPTED,		        //envoyer 13
	CONTACT_BUSY,                //envoyer 14
	END_CALL,                    //envoyer 15        (à recevoir du client qui termine l'appel puis à envoyer au second)
	DISCONNECTION,                //envoyer 16
};

struct Protocol
{
	EProtocol	id;
	char		user[30];
	char		pass[30];
	char		ip[17];
	int			nb_contacts;
};

struct Friends
{
	char	friends[30];
};