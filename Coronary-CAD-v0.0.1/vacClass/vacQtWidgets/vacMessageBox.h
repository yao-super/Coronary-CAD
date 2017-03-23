/**
 * @file vacMessageBox.h
 * @brief
 * 
 *
 * @see vacMessageBox
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef VACMESSAGEBOX_H
#define VACMESSAGEBOX_H

#include <QDialog>

class QLabel;

/** 
 * @class vacMessageBox
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacMessageBox : public QDialog
{
	Q_OBJECT
public:
	vacMessageBox(QWidget *parent = 0);
public slots:
	void OnShowMessage(QString text);
private:
	QLabel *m_MessageLabel;
};

#endif