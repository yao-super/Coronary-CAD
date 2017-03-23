/**
 * @file vacGrayLevelHistogram.cpp
 * @brief
 * 
 *
 * @see vacGrayLevelHistogram
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */

#include "vacGrayLevelHistogram.h"

/**
 * @brief vacGrayLevelHistogram()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacGrayLevelHistogram::vacGrayLevelHistogram (QWidget *parent):QLabel(parent)
{

}

/**
 * @brief OnGetImgGrayVal()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacGrayLevelHistogram::OnGetImgGrayVal(QImage img)
{
	m_GrayImage = img;
	repaint();
}

/**
 * @brief paintEvent()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacGrayLevelHistogram::paintEvent(QPaintEvent *e)
{

	QLabel::paintEvent(e);
	QPainter p(this);
    p.setBrush(QBrush(QColor(121,121,121)));
	p.drawRect(0, 0, this->width(), this->height());
	p.translate(0, this->height());
    // get the pixel of image
	if (m_GrayImage.bytesPerLine())
	{
	   QRgb val;
	   vector<int> hist(256);  
	   for (int i=0; i < m_GrayImage.width(); i++)  
		{  
		  for (int j=0; j < m_GrayImage.height(); j++)  
		   {  
			 val = m_GrayImage.pixel(i,j);  
			 int index = qRed(val);
			 hist[index] = hist[index] + 1;  
		   }  
		}  
        // sort the pixel
		vector<int> sort_count = hist;
		sort(sort_count.begin(), sort_count.end());
		int maxcount = sort_count[sort_count.size() - 1];

	    QPen pen;
	    QFont font("Arial", 3, QFont::Bold,true);
     	p.setFont(font);
	    pen.setColor(Qt::red);
	    pen.setWidth(5);
	    p.setPen(pen);
	    p.drawLine(0, 0, this->width(), 0);
	    p.drawLine(0, 0, 0, -this->height());
	    float x_step = float(this->width() - 10) / 256;
	    float y_step = float(this->height()) / maxcount;
       
	   for (int i=0; i!=256; i++)
	     {  
		   p.setBrush(QColor(i, 255-i, 0));
		   p.setPen(QColor(i, 255-i, 0));
           p.drawRect(5 + (i-1)*x_step, 0, x_step, -y_step*hist[i]);
	          /*if ( i%32 == 0 || i == 255)
	         {
			   p.drawText(QPointF(5+(i-0.5)*x_step,-2),QString::number(i));
		     }*/
	     }
	}
}
