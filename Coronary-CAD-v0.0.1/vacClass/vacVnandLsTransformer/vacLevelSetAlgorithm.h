/**
 * @file vacLevelSetAlgorithm.h
 * @brief
 * 
 *
 * @see vacLevelSetAlgorithm
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */

#ifndef VACLEVELSETALGOORITHM_H
#define VACLEVELSETALGOORITHM_H
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include "itkVersorRigid3DTransform.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkIdentityTransferFunction.h"
#include <QtGui>
#include <QApplication>
#include <string>
using namespace std;
using namespace cv;
/** 
 * @class vacLevelSetAlgorithm
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacLevelSetAlgorithm
{
public:
	vacLevelSetAlgorithm();
	~vacLevelSetAlgorithm();
	enum {Dimension = 3};
	typedef float                                  InputPixelType;
	typedef itk::Image<InputPixelType, Dimension>  InputImageType;
	typedef itk::Image<InputPixelType, 2>          OutputImageType;
	typedef itk::ImageFileReader<InputImageType>   ReaderType;
	typedef itk::BSplineInterpolateImageFunction<InputImageType>  T_Interpolator;
	typedef InputImageType::PixelType   PixelType;
	typedef itk::Rigid3DTransform<double>   TransformType;
	typedef itk::ResampleImageFilter<InputImageType,InputImageType>  ResampleFilterType;
	typedef itk::ImageFileWriter<InputImageType>         WriterType;
	typedef itk::ImageRegionIteratorWithIndex<InputImageType>      InputIteratorType;
	void LevelSetProcessImg(InputImageType::Pointer &cutImgReader,InputIteratorType &empIt,int levels,int num);
private:
	int m_iterNum;      //��������  
	float m_lambda1;    //ȫ����ϵ��  
	float m_nu;     //����Լ��ϵ����  
	float m_mu;     //�ͷ���ϵ����  
	float m_timestep;   //�ݻ�������t  
	float m_epsilon;    //���򻯲����� 
	Mat m_mImage;       //Դͼ��

	int m_iCol;     //ͼ����  
	int m_iRow;     //ͼ��߶�  
	int m_depth;        //ˮƽ���������  
	float m_FGValue;    //ǰ��ֵ  
	float m_BKValue;    //����ֵ  
	Mat m_mPhi;     //ˮƽ������ 

	Mat m_mDirac;       //�����˴����ˮƽ�����ģ��գ�  
	Mat m_mHeaviside;   //���Ϻ��������ˮƽ���������գ�  
	Mat m_mCurv;        //ˮƽ�����ʦ�=div(����/|����|)  
	Mat m_mK;       //�ͷ�������  
	Mat m_mPenalize;    //�ͷ����еĨ�<sup>2</sup>�� 
	Mat m_cvResoutImg; //�ָ�õ���ͼ��

	int m_nImWidth;
	int m_nImHeight;
	int m_nBytePerLine;
	unsigned char *m_chImageData;
	float  *m_fPixelData;
	float m_fMinThreshold;
	float m_fMaxThreshold;
	QImage m_chcutImage;

	void Dirac();       //�����˺���  
	void Heaviside();   //���Ϻ���  
	void Curvature();   //����  
	void BinaryFit();   //����ǰ���뱳��ֵ 
	void initializePhi(Mat img, Rect boxPhi);//ǰ������  
	void EVolution();   //�ݻ� 

	std::string m_InputImgN;
	std::string m_OutputImgN;
	std::vector<float> m_levsetParametersF;
	
};
#endif //VACLEVELSETALGOORITHM_H
