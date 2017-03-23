/**
 * @file vacQtView.h
 * @brief
 * 
 *
 * @see vacQtView
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef VACQTVIEW_H
#define VACQTVIEW_H

#include <QGroupBox>
#include <iostream>
#include <QDialog>
#include <QTextBrowser>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

#include "qxtspanslider.h"

class QLabel;
class QSpinBox;
class QSlider;
class QGroupBox;
class QTextBrowser;
class QSlider;
class QSpinBox;
class vacGrayLevelHistogram ;
class QxtSpanSlider;
class vac3Dto2DTransformer;

/** 
 * @class vacQtView
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacQtView : public QDialog
{
	Q_OBJECT
public:
	vacQtView(QWidget *parent = 0);
	~vacQtView();
	bool ShowStrImgFun();
	bool ShowOriImgFun();
	void SetXYBrowser(int x,int y);
	void SetStrImgGrayVal(int val);
	void SetOriImgGrayVal(int val);
public slots:
	void OnReShowImg(int level);
	void OnIncreaseLayers();
	void OnDecreaseLayers();
	void OnStrImgMaxThrCh(int value);
	void OnStrImgMinThrCh(int value);
	void OnOriImgMaxThrCh(int value);
	void OnOriImgMinThrCh(int value);
	void OnSetLayers(int level);
	void OnSetZBrowser(int level);	
	void OnSetStrImgMaxThreshold(int value);
	void OnSetStrImgMinThreshold(int value);
	void OnSetOriImgMaxThreshold(int value);
	void OnSetOriImgMinThreshold(int value);
	void OnShowStrImg(QImage img);
	void OnShowOriImg(QImage img);
signals:
	void LayersChanged(int levels);
	void LayersIncreased();
	void LayersDecreased();
	void SendStrImg(QImage img);
	void SendOriImg(QImage img);
	void StrMinThresholdChanged(int);
	void StrMaxThresholdChanged(int);
	void OriMinThresholdChanged(int);
	void OriMaxThresholdChanged(int);
	void DestroyQtView();
private:
	QLabel *m_XLabel;
	QLabel *m_YLabel;
	QLabel *m_ZLabel;
	QTextBrowser *m_XBrowser;
	QTextBrowser *m_YBrowser;
	QTextBrowser *m_ZBrowser;
	QGroupBox    *m_CoordGroup;

	QLabel *m_OriImgLabel;
	QLabel *m_StrImgLabel;
	QTextBrowser *m_OriImgBrowser;
	QTextBrowser *m_StrImgBrowser;
	QGroupBox    *m_ImgShowGroup;

	QGroupBox *RightGroup;
	QGroupBox *TopGroup;
	QGroupBox *BelowGroup;

	QLabel    *m_LayerLabel;
	QSpinBox  *m_LayerSpinBox;

    QLabel *m_OriImgShowLabel;
	QLabel *m_StrImgShowLabel;
	vacGrayLevelHistogram  *m_OriGrayLevelHistogram;
	vacGrayLevelHistogram  *m_StrGrayLevelHistogram;

	QImage m_GetPixStrImg;
	QImage m_GetPixOriImg;
    
	QSlider *m_LayerSlider;

	QxtSpanSlider *m_StrSpanslider;
	QSpinBox *m_StrMinSpinBox;
	QSpinBox *m_StrMaxSpinBox;

	QxtSpanSlider *m_OriSpanslider;
	QSpinBox *m_OriMinSpinBox;
	QSpinBox *m_OriMaxSpinBox;

	vac3Dto2DTransformer *m_StrImgTransformer;
	vac3Dto2DTransformer *m_OriImgTransformer;

	QString m_QStrImgName;
	QString m_QOriImgName;
protected:
	void wheelEvent(QWheelEvent *e);
	void paintEvent (QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void closeEvent(QCloseEvent *e);
};

#endif  //VACQTVIEW_H