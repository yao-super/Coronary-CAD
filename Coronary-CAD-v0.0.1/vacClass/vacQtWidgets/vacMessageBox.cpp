/**
 * @file vacMessageBox.cpp
 * @brief
 * 
 *
 * @see vacMessageBox
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#include<QtGui>
#include"vacMessageBox.h"

/**
 * @brief vacMessageBox()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacMessageBox::vacMessageBox(QWidget *parent /* = 0 */)
	:QDialog(parent)
{
	m_MessageLabel = new QLabel;
	QHBoxLayout *mainlayout = new QHBoxLayout;
	mainlayout->addWidget(m_MessageLabel);
	setLayout(mainlayout);
	setFixedHeight(sizeHint().height());
}

/**
 * @brief OnShowMessage()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vacMessageBox::OnShowMessage(QString text)
{ 
	
	m_MessageLabel->setText(text);
}