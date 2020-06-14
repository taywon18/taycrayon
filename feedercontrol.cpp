#include "feedercontrol.h"
#include "ui_feedercontrol.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QHttpPart>
#include <QSettings>
#include <QNetworkReply>
#include <QDebug>

#include "GtaKeybinder.h"
#include "mainwindow.h"

FeederControl::FeederControl(MainWindow *parent) :
	QWidget(nullptr),
	ui(new Ui::FeederControl),
	m_mainwin(parent)
{
	ui->setupUi(this);

	m_mngr = new QNetworkAccessManager(this);

	m_workLastTimeMutex.lock();
	ui->result_label->setText("never started");
	m_workLastTimeMutex.unlock();

	m_timer.setInterval(1000);
	connect(&m_timer, &QTimer::timeout, this, &FeederControl::feed);
	QSettings set;

	ui->feederLE->setText(set.value("feeder/url").toString());
	connect(ui->feederLE, &QLineEdit::textChanged, [&](QString i){
		QSettings sett;
		sett.setValue("feeder/url", ui->feederLE->text());
	});


	connect(ui->checkBox, &QCheckBox::stateChanged, [&](int state){
		QSettings sett;
		setEnabled(state);
		sett.setValue("feeder/active", (bool)state);
	});
	ui->checkBox->setChecked(set.value("feeder/active").toBool());
}

FeederControl::~FeederControl()
{
	delete ui;
}

void FeederControl::setEnabled(bool enabled)
{
	if(enabled)
		m_timer.start();
	else
		m_timer.stop();
}

void FeederControl::feed()
{
	GtaKeybinder& instance = GtaKeybinder::instance();
	auto gtaprocess = instance.getOpenedGtaProcess(false);

	QSettings sets;

	if(!sets.value("feeder/url").isValid())
	{
		ui->result_label->setText("feeder/url not set");
		return;
	}

	QUrl url = QUrl(sets.value("feeder/url").toString());
	if(!url.isValid())
	{
		ui->result_label->setText("feeder/url not url");
		return;
	}

	QString pseudo = sets.value("lastPseudo").toString();
	if(pseudo.isEmpty())
	{
		ui->result_label->setText("pas de pseudo détecté");
		return;
	}

	QString occ = sets.value("lastOccupation").toString();
	if(occ.isEmpty())
	{
		ui->result_label->setText("pas d'occupation détectée");
		return;
	}

	if(!gtaprocess)
	{
		ui->result_label->setText("gta not started");
		return;
	}

	auto bindenv = instance.getBindEnvironment(gtaprocess);
	QJsonObject pos{
		{"x", static_cast<double>(bindenv.posX)},
		{"y", static_cast<double>(bindenv.posY)},
		{"z", static_cast<double>(bindenv.posZ)},
		{"rotz", 0}
	};

	QJsonObject Player
	{
		{"name", pseudo},
		{"occupation", occ},
		{"position", pos}
	};



	QNetworkRequest req;
	req.setUrl(sets.value("feeder/url").toString());
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QByteArray jsonUTF8 = QJsonDocument(Player).toJson(QJsonDocument::Compact);

	QNetworkReply* res = m_mngr->post(req, jsonUTF8);
	ui->result_label->setText("message envoyé");

	connect(res, &QNetworkReply::finished, this, &FeederControl::messageReceived, Qt::QueuedConnection);
}

void FeederControl::messageReceived()
{
	auto res = qobject_cast<QNetworkReply*>(sender());
	if(res->error() == QNetworkReply::NoError)
		ui->result_label->setText("Ok");
	else
	{
		ui->result_label->setText(res->errorString());
		qDebug() << res->readAll();
	}
	res->deleteLater();
}

