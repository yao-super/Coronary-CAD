/**
 * @file vacCmprAlgorithm.h
 * @brief
 * 
 *
 * @see vacCmprAlgorithm
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef  VACCMPRALGORITHM_H
#define  VACCMPRALGORITHM_H

#include <iostream>
#include "itkBSplineInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include "itkVersorRigid3DTransform.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSeedPointFileIO.h"
#include "itkIdentityTransferFunction.h"
#include "vtkPoints.h"
#include "vtkLine.h"
#include "vtkPolyLine.h"

#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkSmartPointer.h"


#include "vtkUnstructuredGrid.h"

// for read
#include "vtkUnstructuredGridReader.h"
#include <time.h>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

typedef struct
{
	float x;
	float y;
	float z;
} PointCordTypeDef;

/** 
 * @class vacCmprAlgorithm
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacCmprAlgorithm 
{
public:
	vacCmprAlgorithm();
	~vacCmprAlgorithm();
	void CmprProcessImg();
	std::vector<float> NormalizeVector(const std::vector<float>& inputVec);
	float Length(const std::vector<float>& inputVec);
	std::vector<float> CrossPruduct(const std::vector<float>& inputVec1,const std::vector<float>& inputVec2);
	void SetInPutFileName(string& file);
	void SetOutputFileName(string& file);
	void SetCenterLineName(string& name);
private:
	std::vector<float> m_ParametersF;
	float m_CMPRResolution;
	std::string m_InitialImageN;
	const char* m_SeedPointN;
	std::string m_OutputImageN;
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
};

#endif  //VACCMPRALGORITHM_H