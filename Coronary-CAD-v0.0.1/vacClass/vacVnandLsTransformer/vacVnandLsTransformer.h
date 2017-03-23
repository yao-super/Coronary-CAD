/**
 * @file vacVnandLvTransformer.h
 * @brief
 * 
 *
 * @see vacVnandLvTransformer
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef VACVNANDLVTRANSFORMER_H
#define VACVNANDLVTRANSFORMER_H

#include <iostream>
#include <QDialog>
#include <QThread>
#include <string>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"
using namespace std;

/** 
 * @class vacVnandLvTransformer
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacVesselnessAlgorithm;
class vacVnandLvTransformer:public QThread
{
	Q_OBJECT
public:
	vacVnandLvTransformer();
	~vacVnandLvTransformer();
	void GetInput3DImageFileName(QString& file);
	void GetOutput3DImageFileName(QString& file);
	void Get3DImageSize();
signals:
	void ImgProcessSucceed(QString text);
	void ImgProcessComplete();
protected:
	void run();
private:
	std::string m_InputImageN;
	std::string m_OutputImageN;
	vacVesselnessAlgorithm* m_VesselnessAlg;
	enum {Dimension=3};
	typedef float                                  InputPixelType;
	typedef itk::Image<InputPixelType, Dimension>  InputImageType;
	typedef itk::ImageRegionIteratorWithIndex<InputImageType>      InputIteratorType;
	typedef itk::ImageFileReader<InputImageType>   ReaderType;
};

#endif //VACVNANDLVTRANSFORMER_H