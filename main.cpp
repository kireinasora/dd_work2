#include "dd_work.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>


int main(int argc, char *argv[])
{

	QTextCodec *codec = QTextCodec::codecForName("Big5-ETen");

	QApplication a(argc, argv);
	dd_work w;
	w.show();
	return a.exec();
}
