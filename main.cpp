#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QtGui/QApplication>
#else
	#include <QtWidgets/QApplication>
#endif

#include <QtCore/QScopedPointer>

#include "control.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Control control;

	return app.exec();
}
