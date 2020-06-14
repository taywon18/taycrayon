#include "logexplorer.h"
#include "ui_logexplorer.h"

#include <QFontDatabase>
#include <QtDebug>
#include <QPaintEngine>
#include <QDirIterator>
#include <QFile>
#include <QClipboard>
#include <QDesktopServices>
#include "utils.h"
#include <QTextDocumentFragment>

LogExplorer::LogExplorer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogExplorer)
{
    ui->setupUi(this);

	ui->searchGroupBox->hide();

    _highlighter = new ChatHighlighter(ui->logDisplayer->document());

    /*QFont arial("Arial", 10, QFont::Black);
    arial.setWeight(99);

    QPen outline;
    outline.setStyle(Qt::SolidLine);
    outline.setWidthF(0.7);
    outline.setBrush(Qt::black);
    outline.setCapStyle(Qt::FlatCap);
    outline.setJoinStyle(Qt::MiterJoin);
    outline.setCosmetic(true);


    _defaultFormat.setFont(arial);
    _defaultFormat.setTextOutline(outline);
    _defaultFormat.setForeground(QBrush(QColor(255,255,255)));

    QTextCharFormat pub = _defaultFormat;
    pub.setForeground(QBrush(QColor(0,255,0)));
    _regexps.append(qMakePair(pub, QRegularExpression("\\[[0-9]+:[0-9]+:[0-9]+\\] \\[PUBLICITÉ\\](.+)")));
    _regexps.append(qMakePair(pub, QRegularExpression("\\[[0-9]+:[0-9]+:[0-9]+\\] Contact: (.+)")));

    QTextCharFormat pm = _defaultFormat;
    pm.setForeground(QBrush(QColor(255,255,0)));
    _regexps.append(qMakePair(pm, QRegularExpression("\\[[0-9]+:[0-9]+:[0-9]+\\] \\(\\( MP (.+)")));

    QTextCharFormat radio = _defaultFormat;
    radio.setForeground(QBrush(QColor(99,159,255)));
    _regexps.append(qMakePair(radio, QRegularExpression("\\[[0-9]+:[0-9]+:[0-9]+\\] \\[Radio\\] (.+)")));

    QTextCharFormat me = _defaultFormat;
    me.setForeground(QBrush(QColor(194,162,218)));
    _regexps.append(qMakePair(me, QRegularExpression("\\[[0-9]+:[0-9]+:[0-9]+\\] \\* (.+)")));
    */

	QSettings set(Utils::GetConfigPath(), QSettings::IniFormat);

    connect(ui->logChoiceCB, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LogExplorer::indexChanged);
    connect(ui->outlineCB, &QCheckBox::stateChanged, this, &LogExplorer::setOutline);
    connect(ui->CopyButton, &QToolButton::clicked, this, &LogExplorer::copySelectedText);
    connect(ui->setLogManagementCB, &QCheckBox::stateChanged, this, &LogExplorer::onlineChanged);
	connect(ui->openLogDirectoryButton, &QAbstractButton::clicked, this, &LogExplorer::openLogDirectory);
	connect(&_watcher, &TemporalFileWatcher::newLine, this, &LogExplorer::onChatNewLine);

    bool logactive = set.value("log/isOnline", ui->setLogManagementCB->isChecked()).toBool();
    _watcher.setActive(logactive);
    ui->setLogManagementCB->setChecked(logactive);
    setOutline(set.value("log/isOutline", ui->outlineCB->isChecked()).toBool());

    refreshFileList();
}

LogExplorer::~LogExplorer()
{
    delete ui;
}

void LogExplorer::refreshFileList()
{
    ui->logChoiceCB->clear();
    QDirIterator it(Utils::GetSampDirectoryPath() +  "/chatlogs", QDir::Files);
    while (it.hasNext()) {
        it.next();

        QFileInfo fi = it.fileInfo();
        if(!fi.isFile())
            continue;

        qDebug() << fi.fileName();
        ui->logChoiceCB->addItem(fi.fileName(), QVariant(fi.filePath()));
    }
}

void LogExplorer::select(QFileInfo fi)
{
    ui->logDisplayer->clear();
    QPalette p = ui->logDisplayer->palette();
    p.setColor(QPalette::Base, QColor(175, 238, 238));
    ui->logDisplayer->setPalette(p);
    if(!fi.exists() || !fi.isFile())
        return;

    QFile f(fi.filePath());
    if(!f.open(QFile::ReadOnly | QFile::Text ))
        return;

    QTextStream ts(&f);
    QString buff = ts.readAll();
    for(QString line : buff.split('\n', QString::SkipEmptyParts))
        addLine(line);
}

void LogExplorer::setOutline(bool b)
{
	QSettings set(Utils::GetConfigPath(), QSettings::IniFormat);
    _highlighter->setOutlined(b);
    ui->logDisplayer->setText(ui->logDisplayer->toPlainText());
    ui->outlineCB->setChecked(b);
    set.setValue("log/isOutline", ui->outlineCB->isChecked());
}

void LogExplorer::onlineChanged(bool)
{
	QSettings set(Utils::GetConfigPath(), QSettings::IniFormat);
    set.setValue("log/isOnline", ui->setLogManagementCB->isChecked());
    _watcher.setActive(ui->setLogManagementCB->isChecked());

}

void LogExplorer::openLogDirectory()
{
	QDesktopServices::openUrl( QUrl::fromLocalFile( Utils::GetSampDirectoryPath()+"/chatlogs" ) );
}

void LogExplorer::indexChanged(int index)
{
    if(index == -1)
        select();

    QString path = ui->logChoiceCB->itemData(index).toString();
    select(QFileInfo(path));
}

void LogExplorer::addLine(QString line)
{

    for(auto i : _regexps)
        if(i.second.match(line).hasMatch())
        {
            addLine(line, i.first);
            return;
        }

    addLine(line, _defaultFormat);
}

void LogExplorer::addLine(QString line, QTextCharFormat format)
{
    //ui->logDisplayer->textCursor().insertText(line + "\n", format);

    ui->logDisplayer->append(line);
}

void LogExplorer::copySelectedText()
{
    QTextCursor cursor = ui->logDisplayer->textCursor();
    QString text = ui->logDisplayer->toPlainText();


    int begin = cursor.selectionStart();
    int end = cursor.selectionEnd();

    while(begin >= 0 && text[begin] != '\n')
        begin--;
    if(text[begin] == '\n')
        begin++;

    while(end < text.length() && text[end] != '\n')
        end++;

    QString unformated = ui->logDisplayer->toPlainText().mid(begin, end - begin);
    qDebug() << "unformated: " << unformated;
    QString formated = _highlighter->toBBCode(unformated);
    qDebug() << "formated: " << formated;


    QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText(formated);
}

void LogExplorer::onChatNewLine(QString str)
{
	QSettings set;

	QRegularExpression regexp_server("^\\[[0-9]+:[0-9]+:[0-9]+\\] Connecting to ([0-9\\.:]+)\\.\\.\\.$");
	auto r = regexp_server.match(str);
	if(r.hasMatch())
	{
		QString address = r.capturedTexts()[1];
		if(address == ":7777")
			return;

		set.setValue("lastAddress", address);
		set.setValue("lastPseudo", QVariant());
		set.setValue("lastOccupation", QVariant());
	}

	QRegularExpression regexp_name("^\\[[0-9]+:[0-9]+:[0-9]+\\] Vous êtes maintenant loggé avec le personnage ([a-zA-Z\\_]+) \\!$");
	r = regexp_name.match(str);
	if(r.hasMatch())
	{
		QString pseudo = r.capturedTexts()[1];
		set.setValue("lastPseudo", pseudo);
	}

	QRegularExpression regexp_occupation("^\\[[0-9]+:[0-9]+:[0-9]+\\] Vous venez de modifier votre occupation: \\{FFFFFF\\}(.+)$");
	r = regexp_occupation.match(str);
	if(r.hasMatch())
	{
		QString occ = r.capturedTexts()[1];
		set.setValue("lastOccupation", occ);
	}
}

void LogExplorer::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
		this->close();
}

TemporalFileWatcher& LogExplorer::watcher()
{
	return _watcher;
}

