#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include <QSettings>
#include <QFileDialog>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);
	QSettings settings;
	ui->sourceTreeEdit->setText(settings.value("source tree", QString()).toString());
	ui->dbEdit->setText(settings.value("symbol database", QString()).toString());
	ui->diagramEdit->setText(settings.value("diagrams dir", QString()).toString());
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::accept()
{
	QSettings settings;
	QString srcTree = ui->sourceTreeEdit->text();
	if (!srcTree.endsWith("/"))
		srcTree.append("/");
	QString diagDir = ui->diagramEdit->text();
	if (!diagDir.endsWith("/"))
		diagDir.append("/");
	settings.setValue("source tree", srcTree);
	settings.setValue("symbol database", ui->dbEdit->text());
	settings.setValue("diagrams dir", diagDir);
	QDialog::accept();
}

void PreferencesDialog::selectSourceTree()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"));
	if (!dir.isEmpty())
		ui->sourceTreeEdit->setText(dir);
}

void PreferencesDialog::selectDatabase()
{
	QString db = QFileDialog::getOpenFileName(this, tr("Open Directory"), "", tr("SQLITE3 database (*.db *.sqlite)"));
	if (!db.isEmpty())
		ui->dbEdit->setText(db);
}

void PreferencesDialog::selectDiagramsDir()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"));
	if (!dir.isEmpty())
		ui->diagramEdit->setText(dir);
}
