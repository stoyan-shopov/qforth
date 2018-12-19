#include <QMessageBox>

#include "mainwindow.hxx"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QProcess &sforth_process, QWidget *parent) :
	QMainWindow(parent),
	sforth_process(sforth_process),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(&sforth_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
		[&] (int exitCode, QProcess::ExitStatus exitStatus)
	{
		if (exitStatus == QProcess::CrashExit)
		{
			auto t = QMessageBox::question(0, "sforth process crashed", "The sforth process crashed. Do you want to restart it?");
			if (t == QMessageBox::StandardButton::Yes)
				sforth_process.start();
		}
	});

	sforth_frontend_server.listen("sforth-server");
	sforth_process.start();
	connect(& sforth_frontend_server, & QLocalServer::newConnection, [&]
	{
		sforth_frontend_server.waitForNewConnection(-1);
		sforth_socket = sforth_frontend_server.nextPendingConnection();
		connect(sforth_socket, & QLocalSocket::readyRead, [=] { ui->plainTextEdit->appendPlainText(sforth_socket->readAll()); });
	});
	connect(ui->lineEdit, & QLineEdit::returnPressed, [&]
	{
		sforth_socket->write((ui->lineEdit->text() + '\n').toLocal8Bit());
		ui->lineEdit->clear();
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}
