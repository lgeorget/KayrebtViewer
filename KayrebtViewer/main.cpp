/**
 * @brief main.cpp
 * @author Laurent Georget
 * @date 2015-06-17
 * @brief Contains the entry point main()
 */
#include "viewer.h"
#include "preferencesdialog.h"
#include <QApplication>


/**
 * \brief Starts the program.
 *
 * This function first displays the preferences dialog, and once the settings
 * are accepted, it shows the main window.
 *
 * \param argc number of arguments, passed to QApplication's constructor
 * \param argv[] the arguments, passed to QApplication's constructor
 *
 * \return the result of QApplication::exec(), i.e. 0 if everything went well,
 * otherwise an error code
 */
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
