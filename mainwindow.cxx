#include <QMessageBox>

#include "mainwindow.hxx"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QProcess &sforth_process, QWidget *parent) :
	QMainWindow(parent),
	sforth_process(sforth_process),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QFile f(":/stylesheet.txt");
	f.open(QFile::ReadOnly);
	setStyleSheet(f.readAll());

	connect(&sforth_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
		[&] (int exitCode, QProcess::ExitStatus exitStatus)
	{
		if (exitStatus != QProcess::NormalExit)
		{
			auto t = QMessageBox::question(0, "sforth process crashed", "The sforth process crashed. Do you want to restart it?");
			if (t == QMessageBox::StandardButton::Yes)
				sforth_process.start();
		}
		else
		{
			auto t = QMessageBox::question(0, "sforth process terminated", "The sforth process terminated. Do you want to restart it?");
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
		connect(sforth_socket, & QLocalSocket::readyRead, [=]
		{
			QTextCursor c(ui->plainTextEdit->textCursor());
			c.movePosition(QTextCursor::End);
			auto t = sforth_socket->readAll();
			c.insertText(t);
		});
	});
	connect(ui->lineEdit, & QLineEdit::returnPressed, [&]
	{
		sforth_socket->write((ui->lineEdit->text() + '\n').toLocal8Bit());
		ui->lineEdit->clear();
	});
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	sforth_frontend_server.disconnect();
	sforth_process.disconnect();
	sforth_process.disconnect();
	QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
	delete ui;
}
