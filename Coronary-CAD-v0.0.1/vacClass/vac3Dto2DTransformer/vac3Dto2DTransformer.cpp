 /**
 * @file vac3Dto2DTransformer.cpp
 * @brief
 * 
 *
 * @see vac3Dto2DTransformer
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
 
#include "vac3Dto2DTransformer.h"

/**
 * @brief vac3Dto2DTransformer()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vac3Dto2DTransformer::vac3Dto2DTransformer():m_fLayer(0),m_nImWidth(100),m_nImHeight(100),m_fMinThreshold(-180),m_fMaxThreshold(300),m_nBytePerLine(0)
{
	//imagetype = type;
	m_nBytePerLine = (m_nImWidth*24 + 31) / 8;
	m_chImageData = new unsigned char[m_nBytePerLine * m_nImHeight];
	//m_vPointch.push_back(m_chImageData);
	m_fPixelData = new float [m_nImHeight * m_nImWidth]; 
	//m_vPointf.push_back(m_fPixelData);
}

int vac3Dto2DTransformer::sm_nMaxLayer = 0;
/**
 * @brief SetShowPictureName()
 *
 * set the 3D image name .
 *
 * @param . 
 *
 * @return None.
 */
 void vac3Dto2DTransformer::SetShowPictureName(QString qfilename)
{
	
   m_stShowImageN = qfilename.toStdString();
   Get3DPicture();
} 

/**
 * @brief Get3DPicture()
 *
 * Get the 3D image .
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::Get3DPicture()
{
	showReader = ReaderType::New();
	showReader->SetFileName(m_stShowImageN); 
	showReader->Update();
}

/**
 * @brief SetOriginalLayers()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::SetOriginalLayers(int z)
{
	m_fLayer = z;
}

/**
 * @brief GetlLayers()
 *
 *
 * @param . 
 *
 * @return the number of layer.
 */
int vac3Dto2DTransformer::GetlLayers()
{
	return m_fLayer;
}

/**
 * @brief ImageTransformer()
 *
 * transform the Image from 3D to 2D .
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::ImageTransformer()
{
	InputImageType::Pointer show = showReader->GetOutput();
	It=InputIteratorType(show, show->GetLargestPossibleRegion() ); //Use Image interator get the pixel of image
	It.GoToBegin();
	while (It.GetIndex()[2]!=m_fLayer)
	{
		++It;
		if(It.IsAtEnd())
		It.GoToBegin();
		continue;
	}
	GetImagePixel();
	for (int ix = 0; ix < m_nImHeight; ix++)
	{
		for (int iy = 0; iy < m_nImWidth; iy++ )
		{
			m_chImageData[ix*m_nBytePerLine + iy*3]     = m_fPixelData[iy + ix*m_nImWidth];  //r
			m_chImageData[ix*m_nBytePerLine + iy*3 + 1] = m_fPixelData[iy + ix*m_nImWidth];  //g
			m_chImageData[ix*m_nBytePerLine + iy*3 + 2] = m_fPixelData[iy + ix*m_nImWidth]; //b

		}
	}
	
	m_chImage = QImage(m_chImageData, m_nImWidth, m_nImHeight, m_nBytePerLine, QImage::Format_RGB888);
	emit ImgTransComplete(m_chImage);
}

/**
 * @brief run()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::run()
{
	ImageTransformer();
}

/**
 * @brief IncreaseLayers()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::IncreaseLayers()
{
	++m_fLayer;
	emit LayersChanged(m_fLayer);
}

/**
 * @brief ReduceLayers()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::ReduceLayers()
{
	--m_fLayer;
	emit LayersChanged(m_fLayer);
}

/**
 * @brief SetLayers()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::SetLayers(int levels)
{   
	if(levels < 1)
		m_fLayer = 0;
	else if(levels > sm_nMaxLayer)
		m_fLayer = sm_nMaxLayer;
	else
	    m_fLayer = levels;
}

/**
 * @brief SetMaxLayers()
 *
 * get the max number of layer .
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::SetMaxLayers()
{
	InputImageType::Pointer reader = showReader->GetOutput();
	InputIteratorType Itb(reader,reader->GetLargestPossibleRegion() );
	Itb.GoToBegin();
	while(!Itb.IsAtEnd())
	 ++Itb;
	sm_nMaxLayer =(--Itb).GetIndex()[2];
}

/**
 * @brief SetMinThreshold()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::SetMinThreshold(int value)
{

	m_fMinThreshold = value;
}

/**
 * @brief SetMaxThreshold()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::SetMaxThreshold(int value)
{
	m_fMaxThreshold = value;  
}

/**
 * @brief DestroyMemory()
 *
 *
 * @param . 
 *
 * @return None.
 */
void vac3Dto2DTransformer::DestroyMemory()
{
	delete[] m_chImageData;
	delete[] m_fPixelData;

}

/**
 * @brief ~vac3Dto2DTransformer()
 *
 *
 * @param . 
 *
 * @return None.
 */
vac3Dto2DTransformer::~vac3Dto2DTransformer()
{
	
}

void vacCMPR3Dto2DTransformer::GetImagePixel()
{
		//According the max or min threshold to set image pixel
   for (int i = 0; i < m_nImHeight; i++)
	   for (int j = 0; j < m_nImWidth; j++)
		   {
			   if(It.Get() < m_fMinThreshold)
			   m_fPixelData[j + i*m_nImWidth] = 0;
			   else if(It.Get() > m_fMaxThreshold)
				m_fPixelData[j + i*m_nImWidth] = 255;
			   else
			      m_fPixelData[j + i*m_nImWidth] = (It.Get() - m_fMinThreshold) * (255.0 / (m_fMaxThreshold - m_fMinThreshold));
				  ++It;
			}
}

void vacLevSet3Dto2DTransformer::GetImagePixel()
{
	for (int i = 0; i < m_nImHeight; i++)
		for (int j = 0; j < m_nImWidth; j++)
		{
				m_fPixelData[j + i*m_nImWidth] = It.Get();
			    ++It;
		}
}