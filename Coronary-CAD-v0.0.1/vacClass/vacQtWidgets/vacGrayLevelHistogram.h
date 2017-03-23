/**
 * @file vacGrayLevelHistogram.h
 * @brief
 * 
 *
 * @see vacGrayLevelHistogram
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef  MYLABEL_H
#define  MYLABEL_H
#include <iostream>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <vector>
using namespace std;

class QLabel;

/** 
 * @class vacGrayLevelHistogram
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacGrayLevelHistogram : public QLabel
{
	Q_OBJECT
public:
	explicit vacGrayLevelHistogram (QWidget *parent = 0);
public slots:
	void OnGetImgGrayVal(QImage img);
protected:
	void paintEvent(QPaintEvent *e);
private:
	QImage m_GrayImage;
};

#endif //MYLABEL_H