#ifndef LOGEXPLORER_H
#define LOGEXPLORER_H

#include <QWidget>
#include <QMap>
#include <QTextCharFormat>
#include <QFileInfo>
#include "chathighlighter.h"
#include "temporalfilewatcher.h"


namespace Ui {
class LogExplorer;
}

class LogExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit LogExplorer(QWidget *parent = nullptr);
    ~LogExplorer();

	TemporalFileWatcher& watcher();

public slots:
	void refreshFileList();
    void select(QFileInfo fi = QFileInfo());
    void setOutline(bool b);
    void onlineChanged(bool);
	void openLogDirectory();

private slots:
    void indexChanged(int index);
    void addLine(QString line);
    void addLine(QString line, QTextCharFormat format);
    void copySelectedText();
	void onChatNewLine(QString str);

private:
    Ui::LogExplorer *ui;
    QList<QPair<QTextCharFormat, QRegularExpression>> _regexps;
    QTextCharFormat _defaultFormat;
    ChatHighlighter* _highlighter;
    TemporalFileWatcher _watcher;
	void keyPressEvent(QKeyEvent *event);
};

#endif // LOGEXPLORER_H
