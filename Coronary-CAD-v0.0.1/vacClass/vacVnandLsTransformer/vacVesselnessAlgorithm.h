/**
 * @file vacVesselnessAlgorithm.h
 * @brief
 * 
 *
 * @see vacVesselnessAlgorithm
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */

#ifndef  VACVESSELNESSALGORITHM_H
#define  VACVESSELNESSALGORITHM_H
#include <iostream>
#include <vector>
#include <process.h>
#include <windows.h>
#include "itkHessian3DToVesselnessMeasureImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <string>
using namespace std;
enum {Dimension = 3};
typedef float                                  InputPixelType;
typedef itk::Image< InputPixelType, Dimension >   InputImageType;
typedef itk::HessianRecursiveGaussianImageFilter< InputImageType >  HessianFilterType;
typedef itk::Hessian3DToVesselnessMeasureImageFilter< InputPixelType > VesselnessMeasureFilterType;	
typedef InputImageType::PixelType   PixelType;
typedef itk::ImageFileReader<InputImageType>   ReaderType;
typedef itk::ImageFileWriter<InputImageType>   WriterType;
typedef itk::ImageRegionIteratorWithIndex<InputImageType>      InputIteratorType;
class vacLevelSetAlgorithm;

class StcUnion
{
public:
	InputIteratorType oriImg_it;
	InputIteratorType empImg_it;
	int nums;
	int levs;
};

/** 
 * @class vacVesselnessAlgorithm
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */

class vacVesselnessAlgorithm
{
public:
	vacVesselnessAlgorithm();
	~vacVesselnessAlgorithm();
	
	void SetInPut3DImgName(string& file);
	void SetOutput3DImgName(string& file);
	void SetInputImgSize(float size);
	void Process3DImg();
private:
	static void Create3DOriImg(int levels,InputIteratorType &oriIt,InputImageType::Pointer &m_newImage);
	void Create3DEmptyImg(int levels,InputImageType::Pointer &emptyImage);
	static unsigned int __stdcall ProcessImgThread(PVOID m);
	string m_InitialImageN;
	string m_OutputImageN;
	vacLevelSetAlgorithm   *m_vacLevelSetAlg;
	int m_InputImgSize;
	static float m_sigma;
	static float m_alpha1;
	static float m_alpha2;
};

#endif  //VACVESSELNESSALGORITHM_H