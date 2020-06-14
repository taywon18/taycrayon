#ifndef FEEDERCONTROL_H
#define FEEDERCONTROL_H

#include <QWidget>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QMutex>

class MainWindow;

namespace Ui {
class FeederControl;
}

class FeederControl : public QWidget
{
		Q_OBJECT

	public:
		explicit FeederControl(MainWindow *parent = nullptr);
		~FeederControl();

	private:
		Ui::FeederControl *ui;
		QNetworkAccessManager *m_mngr;
		QTimer m_timer;
		QString m_workLastTime;
		QMutex m_workLastTimeMutex;
		MainWindow* m_mainwin;


	public slots:
		void setEnabled(bool enabled);

	private slots:
		void feed();
		void messageReceived();
};

#endif // FEEDERCONTROL_H
