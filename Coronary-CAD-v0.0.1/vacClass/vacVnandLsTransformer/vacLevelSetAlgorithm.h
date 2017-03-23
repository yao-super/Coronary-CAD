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
	int m_iterNum;      //迭代次数  
	float m_lambda1;    //全局项系数  
	float m_nu;     //长度约束系数ν  
	float m_mu;     //惩罚项系数μ  
	float m_timestep;   //演化步长δt  
	float m_epsilon;    //规则化参数ε 
	Mat m_mImage;       //源图像

	int m_iCol;     //图像宽度  
	int m_iRow;     //图像高度  
	int m_depth;        //水平集数据深度  
	float m_FGValue;    //前景值  
	float m_BKValue;    //背景值  
	Mat m_mPhi;     //水平集：φ 

	Mat m_mDirac;       //狄拉克处理后水平集：δ（φ）  
	Mat m_mHeaviside;   //海氏函数处理后水平集：Н（φ）  
	Mat m_mCurv;        //水平集曲率κ=div(▽φ/|▽φ|)  
	Mat m_mK;       //惩罚项卷积核  
	Mat m_mPenalize;    //惩罚项中的▽<sup>2</sup>φ 
	Mat m_cvResoutImg; //分割得到的图像

	int m_nImWidth;
	int m_nImHeight;
	int m_nBytePerLine;
	unsigned char *m_chImageData;
	float  *m_fPixelData;
	float m_fMinThreshold;
	float m_fMaxThreshold;
	QImage m_chcutImage;

	void Dirac();       //狄拉克函数  
	void Heaviside();   //海氏函数  
	void Curvature();   //曲率  
	void BinaryFit();   //计算前景与背景值 
	void initializePhi(Mat img, Rect boxPhi);//前景区域  
	void EVolution();   //演化 

	std::string m_InputImgN;
	std::string m_OutputImgN;
	std::vector<float> m_levsetParametersF;
	
};
#endif //VACLEVELSETALGOORITHM_H
