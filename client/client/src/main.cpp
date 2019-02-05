#include "ClientNetwork.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ClientNetwork client;
	return a.exec();
}
