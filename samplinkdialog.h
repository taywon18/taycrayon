#ifndef SAMPLINKDIALOG_H
#define SAMPLINKDIALOG_H

#include <QDialog>

namespace Ui {
class SampLinkDialog;
}

class SampLinkDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit SampLinkDialog(QWidget *parent = nullptr);
		~SampLinkDialog();
		bool createShortcut(QString name, QString targetpath, QString args);

	public slots:
		void accept();

	private:
		Ui::SampLinkDialog *ui;
};

#endif // SAMPLINKDIALOG_H
