/**
 * @file vacProgressBar.h
 * @brief
 * 
 *
 * @see vacProgressBar
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef   VACPROGRESSBAR_H
#define   VACPROGRESSBAR_H
#include <QtGui/QDialog>
#include <QProgressDialog>

/** 
 * @class vac3Dto2DTransformer
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacProgressBar : public QProgressDialog{
	Q_OBJECT
public:
	vacProgressBar();
private slots:
	void OnStopProgressBar();
};

#endif //VACPROGRESSBAR_H