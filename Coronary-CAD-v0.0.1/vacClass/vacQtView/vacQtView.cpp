/**
 * @file vacQtView.cpp
 * @brief
 * 
 *
 * @see vacQtView
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#include <QtGui>
#include "vacQtView.h"
#include "vacGrayLevelHistogram.h"
#include "vac3Dto2DTransformer.h"
/**
 * @brief vacQtView()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacQtView::vacQtView(QWidget *parent):QDialog(parent)
{
	setFixedSize(640,480);
	m_StrImgTransformer = 0;
	m_OriImgTransformer = 0;
	QFont ft;  
	ft.setPointSize(14);
	m_XLabel = new QLabel(tr("x:"), this);
	m_XLabel->setFont(ft);
	m_XLabel->setGeometry(20,80,20,20);
	m_XBrowser = new QTextBrowser(this);
	m_XBrowser->setPlainText(tr("0.00"));
	m_XBrowser->setGeometry(40,80,50,25);

	m_YLabel = new QLabel(tr("y:"), this);  
	m_YLabel->setFont(ft);   
	m_YLabel->setGeometry(20,105,20,20);  
	m_YBrowser = new QTextBrowser(this);  
	m_YBrowser->setPlainText(tr("0.00"));  
	m_YBrowser->setGeometry(40,105,50,25);  

	m_ZLabel = new QLabel(tr("z:"), this);  
	m_ZLabel->setFont(ft);  
	m_ZLabel->setGeometry(20,130,20,20);  
	m_ZBrowser = new QTextBrowser(this);
	m_ZBrowser->setPlainText(tr("0"));
	m_ZBrowser->setGeometry(40,130,50,25);

	m_CoordGroup = new QGroupBox(tr("坐标"), this);
	m_CoordGroup->setGeometry(10,60,120,110);

	m_OriImgLabel = new QLabel(tr("原始图："), this);
	m_OriImgLabel->setGeometry(20,200,40,20);
	m_OriImgBrowser = new QTextBrowser(this);
	m_OriImgBrowser->setPlainText(tr("00.00"));
	m_OriImgBrowser->setGeometry(60,200,50,25);

	m_StrImgLabel = new QLabel(tr("增强图："), this);
	m_StrImgLabel->setGeometry(20,225,40,20);
	m_StrImgBrowser = new QTextBrowser(this);
	m_StrImgBrowser->setPlainText(tr("00.00"));
	m_StrImgBrowser->setGeometry(60,225,50,25);

	m_ImgShowGroup = new QGroupBox(tr("灰度"), this);
	m_ImgShowGroup->setGeometry(10,180,120,80);

	RightGroup = new QGroupBox(this);
	RightGroup->setGeometry(5,20,130,440);
	TopGroup = new QGroupBox(tr("原始图"), this);
	TopGroup->setFont(ft);
	TopGroup->setGeometry(180,20,450,200);
	BelowGroup = new QGroupBox(tr("增强图"), this);
	BelowGroup->setFont(ft);
	BelowGroup->setGeometry(180,260,450,200);

	m_LayerLabel = new QLabel(tr("层数"), this);
	m_LayerLabel->setGeometry(145,110,35,20);
	m_LayerSpinBox = new QSpinBox(this);
	m_LayerSpinBox->setGeometry(136,130,43,20);
	m_LayerSpinBox->setRange(0,750);
	m_LayerSpinBox->setValue(0);
	m_LayerSlider = new QSlider(this);
	m_LayerSlider->setGeometry(148,155,19,150);
	m_LayerSlider->setRange(0,750);
	m_LayerSlider->setValue(0);
	connect(m_LayerSpinBox, SIGNAL(valueChanged(int)),
		m_LayerSlider, SLOT(setValue(int)));
	connect(m_LayerSlider, SIGNAL(valueChanged(int)),
		m_LayerSpinBox, SLOT(setValue(int)));
	connect(m_LayerSlider, SIGNAL(valueChanged(int)),
		this, SLOT(OnSetLayers(int)));
	connect(m_LayerSlider, SIGNAL(valueChanged(int)),
		this, SLOT(OnSetZBrowser(int)));

	m_OriImgShowLabel = new QLabel(this);
	m_OriImgShowLabel->setGeometry(190,70,100,100);
	m_OriImgShowLabel->setStyleSheet("border:2px solid red;");

	m_StrImgShowLabel = new QLabel(this);
	m_StrImgShowLabel->setGeometry(190,310,100,100);
	m_StrImgShowLabel->setStyleSheet("border:2px solid red;");

	m_OriGrayLevelHistogram = new vacGrayLevelHistogram(this);
	m_OriGrayLevelHistogram->setGeometry(320,70,100,70);
	m_OriGrayLevelHistogram->setStyleSheet("border:2px solid red;");
	connect(this, SIGNAL(SendOriImg(QImage)),
	m_OriGrayLevelHistogram, SLOT(OnGetImgGrayVal(QImage)));

	m_StrGrayLevelHistogram = new vacGrayLevelHistogram(this);
	m_StrGrayLevelHistogram->setGeometry(320,310,100,70);
	m_StrGrayLevelHistogram->setStyleSheet("border:2px solid red;");
	connect(this, SIGNAL(SendStrImg(QImage)),
	m_StrGrayLevelHistogram, SLOT(OnGetImgGrayVal(QImage)));

	m_StrSpanslider = new QxtSpanSlider(Qt::Horizontal, this);
    m_StrSpanslider->setGeometry(300,405,140,15);
	m_StrSpanslider->setHandleMovementMode(QxtSpanSlider::NoCrossing);
	m_StrSpanslider->setRange(0,10);
	m_StrMinSpinBox = new QSpinBox(this);
	m_StrMinSpinBox->setRange(0,10);
	m_StrMinSpinBox->setGeometry(300,382,43,20);
	m_StrMaxSpinBox = new QSpinBox(this);
	m_StrMaxSpinBox->setRange(0,10);
	m_StrMaxSpinBox->setGeometry(397,382,43,20);

	connect(m_StrSpanslider, SIGNAL(lowerValueChanged(int)),
		m_StrMinSpinBox, SLOT(setValue(int)));
	connect(m_StrSpanslider, SIGNAL(upperValueChanged(int)),
		m_StrMaxSpinBox, SLOT(setValue(int)));
	connect(m_StrMinSpinBox, SIGNAL(valueChanged(int)),
		m_StrSpanslider, SLOT(setLowerValue(int)));
    connect(m_StrMaxSpinBox, SIGNAL(valueChanged(int)),
		m_StrSpanslider, SLOT(setUpperValue(int)));
	connect(m_StrMinSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(OnSetStrImgMinThreshold(int)));
	connect(m_StrMaxSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(OnSetStrImgMaxThreshold(int)));
	m_StrMinSpinBox->setValue(0);
	m_StrMaxSpinBox->setValue(10);

	m_OriSpanslider = new QxtSpanSlider(Qt::Horizontal, this);
	m_OriSpanslider->setGeometry(300,165,140,15);
	m_OriSpanslider->setHandleMovementMode(QxtSpanSlider::NoCrossing);
	m_OriSpanslider->setRange(0,10);
	m_OriMinSpinBox = new QSpinBox(this);
	m_OriMinSpinBox->setRange(0,10);
	m_OriMinSpinBox->setGeometry(300,142,43,20);
	m_OriMaxSpinBox = new QSpinBox(this);
	m_OriMaxSpinBox->setRange(0,10);
	m_OriMaxSpinBox->setGeometry(397,142,43,20);

	connect(m_OriSpanslider, SIGNAL(lowerValueChanged(int)),
		m_OriMinSpinBox, SLOT(setValue(int)));
	connect(m_OriSpanslider, SIGNAL(upperValueChanged(int)),
		m_OriMaxSpinBox, SLOT(setValue(int)));
	connect(m_OriMinSpinBox, SIGNAL(valueChanged(int)),
		m_OriSpanslider, SLOT(setLowerValue(int)));
	connect(m_OriMaxSpinBox, SIGNAL(valueChanged(int)),
		m_OriSpanslider, SLOT(setUpperValue(int)));
	connect(m_OriMinSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(OnSetOriImgMinThreshold(int)));                 
	connect(m_OriMaxSpinBox, SIGNAL(valueChanged(int)),
		this, SLOT(OnSetOriImgMaxThreshold(int)));           
	m_OriMinSpinBox->setValue(0);    
	m_OriMaxSpinBox->setValue(10);   
}

/**
 * @brief SetStrImgGrayVal()
 *
 * show the gray value of strengthen image on the browser
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::SetStrImgGrayVal(int val)
{
	QString str = QString("%1").arg(val);
	m_StrImgBrowser->setPlainText(str);
}

/**
 * @brief SetOriImgGrayVal()
 *
 * show the gray value of original image on the browser 
 *
 * @param . 
 *
 * @return None.
 */
void vacQtView::SetOriImgGrayVal(int val)
{
	QString str = QString("%1").arg(val);
	m_OriImgBrowser->setPlainText(str);
}

/**
 * @brief SetXYBrowser()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::SetXYBrowser(int x,int y)
{
	QString str = QString("%1").arg(x);
	m_XBrowser->setPlainText(str);
	str = QString("%1").arg(y);
	m_YBrowser->setPlainText(str);
}

/**
 * @brief OnSetZBrowser()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnSetZBrowser(int level)
{
	QString str = QString("%1").arg(level);  
	m_ZBrowser->setPlainText(str);   
}

/**
 * @brief OnSetLayers()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnSetLayers(int level)
{
	emit LayersChanged(level); 
}

/**
 * @brief wheelEvent()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::wheelEvent(QWheelEvent *e)
{
	e->delta() > 0 ? emit LayersIncreased() : emit LayersDecreased();	
}

/**
 * @brief mousePressEvent()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::mousePressEvent(QMouseEvent *e)
{
	int x = e->x();
	int y = e->y();
	if((x>190 && x<290 && y>310 && y<410) || (x>190 && x<290 && y>70 && y<170))
	{

		if (e->button() == Qt::LeftButton && y>310)
		{
			int mouse_x = e->x()-190;  
			int mouse_y = e->y()-310; 
			SetXYBrowser(mouse_x, mouse_y);
			QRgb val_str = m_GetPixStrImg.pixel(mouse_x, mouse_y);
			int gra_str = qRed(val_str);
			SetStrImgGrayVal(gra_str);
			QRgb val_ori = m_GetPixOriImg.pixel(mouse_x, mouse_y);
			int gra_ori = qRed(val_ori);
			SetOriImgGrayVal(gra_ori);
		
		}

		else if (e->button() == Qt::LeftButton)
		{
			int mouse_x = e->x() - 190;
			int mouse_y = e->y() - 70;
			SetXYBrowser(mouse_x, mouse_y);
			QRgb val_str = m_GetPixStrImg.pixel(mouse_x, mouse_y);
			int gra_str = qRed(val_str);
			SetStrImgGrayVal(gra_str);
			QRgb val_ori = m_GetPixOriImg.pixel(mouse_x, mouse_y);
			int gra_ori = qRed(val_ori);
			SetOriImgGrayVal(gra_ori);
			
		}
	}
}

/**
 * @brief OnSetStrImgMaxThreshold()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnSetStrImgMaxThreshold(int value)
{
	emit StrMaxThresholdChanged(value);
}

/**
 * @brief OnSetStrImgMinThreshold()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnSetStrImgMinThreshold(int value)
{
	emit StrMinThresholdChanged(value);
}

/**
 * @brief OnSetOriImgMaxThreshold()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnSetOriImgMaxThreshold(int value)
{
	emit OriMaxThresholdChanged(value);
}

/**
 * @brief OnSetOriImgMinThreshold()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnSetOriImgMinThreshold(int value)
{
	emit OriMinThresholdChanged(value);
}

/**
 * @brief paintEvent()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::paintEvent( QPaintEvent * e )
{
	QPainter p(this);
	p.drawPixmap(0,0,width(),height(),QPixmap("D:/image/2.jpg"));
}

/**
 * @brief ShowStrImgFun()
 *
 * 
 * @param . 
 *
 * @return None.
 */
bool vacQtView::ShowStrImgFun()
{
	m_StrImgTransformer = new vacLevSet3Dto2DTransformer;
	connect(m_StrImgTransformer, SIGNAL(ImgTransComplete(QImage)),
		this, SLOT(OnShowStrImg(QImage)), Qt::BlockingQueuedConnection);    
	m_QStrImgName = QFileDialog::getOpenFileName(this,
		tr("OnOpen Processed Image"), "/",
		tr("Image files (*.nii.gz)"));
	if (m_QStrImgName.isEmpty())
	{
		return  false;
	}
	else
		m_StrImgTransformer-> SetShowPictureName(m_QStrImgName);  
		m_StrImgTransformer->SetOriginalLayers(m_OriImgTransformer->GetlLayers());
		//m_StrImgTransformer->SetMaxLayers();
		m_StrImgTransformer->start();
		connect(m_StrImgTransformer, SIGNAL(LayersChanged(int)),
			m_LayerSlider, SLOT(setValue(int)));
		// when layer changed, reshow the image
	    connect(this, SIGNAL(LayersChanged(int)),
		   this, SLOT(OnReShowImg(int)));   
		connect(this, SIGNAL(StrMinThresholdChanged(int)),
			this, SLOT(OnStrImgMinThrCh(int))); 
		// show the change of threshold
		connect(this, SIGNAL(StrMaxThresholdChanged(int)),
			this, SLOT(OnStrImgMaxThrCh(int)));   
	
	    return true;
}

/**
 * @brief ShowOriImgFun()
 *
 * 
 * @param . 
 *
 * @return None.
 */
bool vacQtView::ShowOriImgFun()
{
    m_OriImgTransformer = new vacCMPR3Dto2DTransformer;  
	connect(m_OriImgTransformer, SIGNAL(ImgTransComplete(QImage)),
		this, SLOT(OnShowOriImg(QImage)), Qt::BlockingQueuedConnection);  
	m_QOriImgName = QFileDialog::getOpenFileName(this,
		tr("OnOpen Processed Image"), "/",
		tr("Image files (*.nii.gz)"));
	if (m_QOriImgName.isEmpty())
	{
		return false;
	}
	else
		m_OriImgTransformer->SetShowPictureName(m_QOriImgName);
	    // set the layer of the original image
	    m_OriImgTransformer->SetOriginalLayers();
		m_OriImgTransformer->SetMaxLayers();
	    m_OriImgTransformer->start();
	connect(this, SIGNAL(LayersChanged(int)),
		this, SLOT(OnReShowImg(int))); 
	connect(this, SIGNAL(LayersIncreased()),
		this, SLOT(OnIncreaseLayers()));
	connect(this, SIGNAL(LayersDecreased()),
		this, SLOT(OnDecreaseLayers()));
	connect(this, SIGNAL(OriMinThresholdChanged(int)),
		this, SLOT(OnOriImgMinThrCh(int))); 
	connect(this, SIGNAL(OriMaxThresholdChanged(int)),
		this, SLOT(OnOriImgMaxThrCh(int))); 
	return true;  
}

/**
 * @brief OnReShowImg()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnReShowImg(int level)
{   
	m_OriImgTransformer->SetLayers(level);
	m_OriImgTransformer->start();
	if (m_StrImgTransformer)
	{
		m_StrImgTransformer->SetLayers(level);
		m_StrImgTransformer->start();
	}
}

/**
 * @brief OnIncreaseLayers()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnIncreaseLayers()
{
	m_OriImgTransformer->IncreaseLayers();
	m_OriImgTransformer->start();
	if (m_StrImgTransformer)
	{
		m_StrImgTransformer->IncreaseLayers();
		m_StrImgTransformer->start();
	}
}

/**
 * @brief OnDecreaseLayers()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnDecreaseLayers()
{
	m_OriImgTransformer->ReduceLayers();
	m_OriImgTransformer->start();
	if (m_StrImgTransformer)
	{
		m_StrImgTransformer->ReduceLayers();
		m_StrImgTransformer->start();
	}
}

/**
 * @brief OnStrImgMinThrCh()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnStrImgMinThrCh(int value)
{   
	m_StrImgTransformer->SetMinThreshold(value);
	m_StrImgTransformer->start();
}

/**
 * @brief OnStrImgMaxThrCh()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnStrImgMaxThrCh(int value)
{
	m_StrImgTransformer->SetMaxThreshold(value);
	m_StrImgTransformer->start();
}

/**
 * @brief OnOriImgMinThrCh()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnOriImgMinThrCh(int value)
{
	m_OriImgTransformer->SetMinThreshold(value);
	m_OriImgTransformer->start();
}

/**
 * @brief OnOriImgMaxThrCh()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnOriImgMaxThrCh(int value)
{
	m_OriImgTransformer->SetMaxThreshold(value);
	m_OriImgTransformer->start();	
}

/**
 * @brief OnShowStrImg()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnShowStrImg(QImage img)
{
	m_StrImgShowLabel->setPixmap(QPixmap::fromImage(img));
	m_GetPixStrImg = img; 
	emit SendStrImg(m_GetPixStrImg);
}

/**
 * @brief OnShowOriImg()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::OnShowOriImg(QImage img)
{
	m_OriImgShowLabel->setPixmap(QPixmap::fromImage(img));
	m_GetPixOriImg = img;
	emit SendOriImg(m_GetPixOriImg); 
}

/**
 * @brief closeEvent()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacQtView::closeEvent(QCloseEvent *e)
{   if(m_OriImgTransformer)
	m_OriImgTransformer->DestroyMemory();
    
    if(m_StrImgTransformer)
	m_StrImgTransformer->DestroyMemory();
	emit DestroyQtView();
}

/**
 * @brief ~vacQtView()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacQtView::~vacQtView()
{
	if (m_StrImgTransformer)
	delete m_StrImgTransformer;
	if(m_OriImgTransformer)
	delete m_OriImgTransformer;
}

