/**
 * @file vac3Dto2DTransformer.h
 * @brief
 * 
 *
 * @see vac3Dto2DTransformer
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef VAC3DTO2DTRANSFORMER_H
#define VAC3DTO2DTRANSFORMER_H

#include <iostream>
#include <vector>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include <QImage>
#include <QDialog>
#include <QFileDialog>
#include <QThread>
using namespace std;

/** 
 * @class vac3Dto2DTransformer
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vac3Dto2DTransformer : public QThread
{
	Q_OBJECT
public:
	vac3Dto2DTransformer();
    virtual ~vac3Dto2DTransformer();
    void ImageTransformer();
	void SetShowPictureName(QString qfilename);
	// increase the number of layer
	void IncreaseLayers();
	// reduce the number of layer
	void ReduceLayers();
	void SetMinThreshold(int value);
	void SetMaxThreshold(int value);
	void Get3DPicture();
	void SetLayers(int levels);
    void SetMaxLayers();
	void SetOriginalLayers(int z =1);
	int  GetlLayers();
	void DestroyMemory();
	virtual void GetImagePixel()=0;
signals:
	void ImgTransComplete(QImage img);
	void LayersChanged(int level);
private:
	//char imagetype;		
	vector<unsigned char*> m_vPointch;
	vector<float*> m_vPointf;
	static int sm_nMaxLayer;
	std::string m_stShowImageN;
	unsigned char *m_chImageData;
	QImage m_chImage;
	enum {Dimension=3};
	typedef float                                  InputPixelType;
	typedef itk::Image<InputPixelType, Dimension>  InputImageType;
	typedef itk::ImageRegionIteratorWithIndex<InputImageType>      InputIteratorType;
	typedef itk::ImageFileReader<InputImageType>   ReaderType;	
	ReaderType::Pointer showReader;
	float m_fLayer;
protected:
	int m_nImWidth;
	int m_nImHeight;
	int m_nBytePerLine;
	float  *m_fPixelData;
	float m_fMinThreshold;
	float m_fMaxThreshold;
	InputIteratorType It;
	void run();
};


/** 
 * @class vacCMPR3Dto2DTransformer
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacCMPR3Dto2DTransformer:public vac3Dto2DTransformer
{
public:
	void GetImagePixel();
};


/** 
 * @class vacLevSet3Dto2DTransformer
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacLevSet3Dto2DTransformer:public vac3Dto2DTransformer
{
public:
	void GetImagePixel();
};

#endif //VAC3DTO2DTRANSFORMER_H