/**
 * @file vacProgressBar.cpp
 * @brief
 * 
 *
 * @see vacProgressBar
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#include "vacProgressBar.h"

/**
 * @brief vacProgressBar()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacProgressBar::vacProgressBar() 
{
	setWindowModality(Qt::WindowModal);
    setMinimumDuration(5);
	setWindowTitle(tr("��ȴ�"));
	setLabelText(tr("���ڴ���..."));
	setCancelButtonText(tr("Cancel"));
    setRange(0, 0);
	setCancelButton(0);
}

/**
 * @brief OnStopProgressBar()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacProgressBar::OnStopProgressBar()
{
    close();
	if(wasCanceled())
	return;
}