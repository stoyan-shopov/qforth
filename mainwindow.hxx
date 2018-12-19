#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QProcess& sforth_process, QWidget *parent = nullptr);
	~MainWindow();
private:
	QProcess&	sforth_process;
	QLocalServer	sforth_frontend_server;
	QLocalSocket	* sforth_socket;
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HXX
