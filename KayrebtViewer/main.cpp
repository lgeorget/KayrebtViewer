#include "viewer.h"
#include "preferencesdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("IRISA");
	QCoreApplication::setApplicationName("Kayrebt::Viewer");

	QApplication a(argc, argv);

	PreferencesDialog prefs;
	if (prefs.exec() != QDialog::Accepted) {
		return 0; //early exit
	}

	Viewer w;
	w.show();

	return a.exec();
}
