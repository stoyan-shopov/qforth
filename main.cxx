#include "mainwindow.hxx"
#include <QApplication>
#include <QCommandLineParser>
#include <QProcess>
#include <QMessageBox>

#include "sforth.hxx"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCommandLineParser p;

	p.addOption(QCommandLineOption("sforth-mode"));
	p.process(a);

	if (p.isSet("sforth-mode"))
	{
		SForth f;
		f.start();
		f.wait();
		return 0;
	}
	QProcess sforth_process;
	sforth_process.waitForStarted();
	sforth_process.setProgram(QString(argv[0])/* + "xxx"*/);
	sforth_process.setArguments(QStringList() << "--sforth-mode");
	MainWindow w(sforth_process);

	w.show();

	return a.exec();
}
