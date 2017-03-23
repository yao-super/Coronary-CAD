/**
 * @file vacCmprAlgorithm.cpp
 * @brief
 * 
 *
 * @see vacCmprAlgorithm
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#include "vacCmprAlgorithm.h"

/**
 * @brief vacCmprAlgorithm()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacCmprAlgorithm::vacCmprAlgorithm() : m_CMPRResolution(0)
{
	m_ParametersF.push_back(0);
	m_ParametersF.push_back(10);
	m_ParametersF.push_back(100);
	m_SeedPointN = "";
	m_InitialImageN = "";
}

/**
 * @brief SetInPutFileName()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacCmprAlgorithm::SetInPutFileName(string& file)
{
	m_InitialImageN = file;
}

void vacCmprAlgorithm::SetOutputFileName(string& file)
{
	m_OutputImageN = file;
}
/**
 * @brief SetCenterLineName()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacCmprAlgorithm::SetCenterLineName(string& name)
{

	m_SeedPointN = name.c_str();
}

/**
 * @brief vac3Dto2DTransformer()
 *
 * 
 * @param . 
 *
 * @return the result of normalizevector.
 */		
std::vector<float> vacCmprAlgorithm::NormalizeVector(const std::vector<float>& inputVec)
{
	float Length=sqrt(pow(inputVec[0],2) + pow(inputVec[1],2) + pow(inputVec[2],2));
	std::vector<float> outputVec(3,0);
	outputVec[0] = inputVec[0] / Length; 
	outputVec[1] = inputVec[1] / Length;
	outputVec[2] = inputVec[2] / Length;
	return outputVec;
}

/**
 * @brief Length()
 *
 * 
 * @param . 
 *
 * @return the length of vector.
 */
float vacCmprAlgorithm::Length(const std::vector<float>& inputVec)
{
	float Length = sqrt(pow(inputVec[0],2) + pow(inputVec[1],2) + pow(inputVec[2],2));
	return Length;
}

/**
 * @brief CrossPruduct()
 *
 * 
 * @param . 
 *
 * @return the crosspruduct of vector.
 */
std::vector<float> vacCmprAlgorithm::CrossPruduct(const std::vector<float>& inputVec1, const std::vector<float>& inputVec2)
{
	std::vector<float> outputVec(3,0);
	outputVec[0] = (inputVec1[1] * inputVec2[2]) - (inputVec1[2] * inputVec2[1]);
	outputVec[1] = (inputVec1[2] * inputVec2[0]) - (inputVec1[0] * inputVec2[2]);
	outputVec[2] = (inputVec1[0] * inputVec2[1]) - (inputVec1[1] * inputVec2[0]);
	return outputVec;
}

/**
 * @brief CmprProcessImg()
 *
 * The CMPR algorithm .
 * 
 * @param . 
 *
 * @return None.
 */
void vacCmprAlgorithm::CmprProcessImg()
{
	m_CMPRResolution = m_ParametersF[1] / m_ParametersF[2];
	ReaderType::Pointer initialReader = ReaderType::New();
	initialReader->SetFileName(m_InitialImageN);
	initialReader->Update();
	InputImageType::Pointer inital = initialReader->GetOutput();
	std::vector<float> imgExt(3,0);
	imgExt[0] = inital->GetBufferedRegion().GetSize()[0];
	imgExt[1] = inital->GetBufferedRegion().GetSize()[1];
	imgExt[2] = inital->GetBufferedRegion().GetSize()[2];

	InputImageType::PointType originIn;
	originIn = inital->GetOrigin();
	std::cout<<"Origin: "<<originIn[0]<<" "<<originIn[1]<<" "<<originIn[2]<<std::endl;
	std::cout<<"Already read original image "<<m_InitialImageN<<"done"<<endl;
	std::cout<<"Size: "<<imgExt[0]<<" "<<imgExt[1]<<" "<<imgExt[2]<<std::endl;

	//itk::SeedPointFileIO::Pointer seedPointReader = itk::SeedPointFileIO::New();
	//seedPointReader->SetFileName(m_SeedPointN);
	//seedPointReader->SetVerbose(false);
	//itk::SeedPointFileIO::PointListType m_seedPoints;
	//m_seedPoints = seedPointReader->GetPoints();
	//emit level_num(m_seedPoints.size());
	//std::cout<<"Centerline contains "<<m_seedPoints.size()<<"points"<<std::endl;
	vector<PointCordTypeDef> m_seedPoints;
	if (!m_seedPoints.empty())
	{
		m_seedPoints.clear();
	}
	vtkSmartPointer<vtkUnstructuredGridReader> iVtkReader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	iVtkReader->SetFileName(m_SeedPointN);
	iVtkReader->Update();
	vtkSmartPointer<vtkUnstructuredGrid> iGridRead = iVtkReader->GetOutput();

	int nPointNum = iGridRead->GetMaxCellSize();

	double dCord[3];
	PointCordTypeDef strctTempPoint;

	//for (int i = 0; i < nPointNum; i++)
	std::cout<<nPointNum<<endl;
	for (int i=0; i<nPointNum; i = i+10)
	{
		iGridRead->GetPoint(i, dCord);
		strctTempPoint.x = dCord[0];
		strctTempPoint.y = dCord[1];
		strctTempPoint.z = dCord[2];
		m_seedPoints.push_back(strctTempPoint);
	}
	std::cout<<"Centerline contains "<<m_seedPoints.size()<<"points"<<std::endl;

	if (m_seedPoints.size()<1)
	{
		std::cout<<"At least one seed point are needed"<<std::endl;
		exit(-1);
	}

	T_Interpolator::Pointer interpolator = T_Interpolator::New();
	interpolator->SetSplineOrder(1);

	InputImageType::IndexType index;
	index.Fill(0);
	InputImageType::SizeType size;
	size[0] = m_ParametersF[2];
	size[1] = m_ParametersF[2];
	size[2] = m_seedPoints.size();

	InputImageType::RegionType region;
	region.SetIndex(index);
	region.SetSize(size);

	InputImageType::PointType origin;
	origin[0]=0;
	origin[1]=0;
	origin[2]=0;

	InputImageType::SpacingType spacing;
	spacing[0] = m_ParametersF[1]/m_ParametersF[2];
	spacing[1] = m_ParametersF[1]/m_ParametersF[2];
	spacing[2] = 1;

	InputImageType::Pointer m_cmprImage = InputImageType::New();
	m_cmprImage->SetSpacing(spacing);

	m_cmprImage->SetRegions(region);
	m_cmprImage->SetOrigin(origin);
	m_cmprImage->Allocate();
	m_cmprImage->FillBuffer(itk::NumericTraits<PixelType>::One);

	InputIteratorType cmprIt(m_cmprImage, m_cmprImage->GetLargestPossibleRegion() );
	cmprIt.GoToBegin();
	std::vector<float> localy(3,0);
	std::vector<float> localx(3,0);

	std::vector<float> tangential1(3,0);
	//tangential1[0] = m_seedPoints[2][0] - m_seedPoints[0][0];
	//tangential1[1] = m_seedPoints[2][1] - m_seedPoints[0][1];
	//tangential1[2] = m_seedPoints[2][2] - m_seedPoints[0][2];
	tangential1[0] = m_seedPoints[2].x - m_seedPoints[0].x;
	tangential1[1] = m_seedPoints[2].y - m_seedPoints[0].y;
	tangential1[2] = m_seedPoints[2].z - m_seedPoints[0].z;
	tangential1 = NormalizeVector(tangential1);
	for (int i = 0; i < m_seedPoints.size(); ++i)
	{
        int fore = min(i+1, (int)(m_seedPoints.size()-1));
		int back = max(i-1, 0);
		std::vector<float> tangential(3,0);
		tangential[0] = m_seedPoints[fore].x - m_seedPoints[back].x;
		tangential[1] = m_seedPoints[fore].y - m_seedPoints[back].y;
		tangential[2] = m_seedPoints[fore].z - m_seedPoints[back].z;
		tangential = NormalizeVector(tangential);

		if (i==0)
		{
			localy = CrossPruduct(tangential, tangential1);
			if (Length(localy) > 0)
			{
				localy = NormalizeVector(localy);
			}

			else
			{
				std::cout<<"The first two tangentials are parallel!!!"<<std::endl;
				localy[0] = tangential[0] + 0.5;
				localy[1] = tangential[1] + 0.5;
				localy[2] = tangential[2] + 0.5;
				localy = NormalizeVector(localy);
			}

			localx = CrossPruduct(localy, tangential);
			localx = NormalizeVector(localx);
		}

		else
		{
			localy = CrossPruduct(tangential, localx);
			localy = NormalizeVector(localy);
			localx = CrossPruduct(localy, tangential);
			localx = NormalizeVector(localx);
		}

		ResampleFilterType::Pointer resampler = ResampleFilterType::New();
		resampler->SetTransform(TransformType::New());
		resampler->SetInput(inital);

		InputImageType::SizeType sizeCMPR;
		sizeCMPR[0] = m_ParametersF[2];
		sizeCMPR[1] = m_ParametersF[2];
		sizeCMPR[2] = 1;
		resampler->SetSize(sizeCMPR);

		std::vector<float> shift2D(2,0);
		shift2D[0] = (0.5 - m_ParametersF[2]/2) * m_CMPRResolution;
		shift2D[1] = (0.5 - m_ParametersF[2]/2) * m_CMPRResolution;
		std::vector<float> shift(3,0);
		shift[0] = shift2D[0]*localx[0] + shift2D[1]*localy[0];
		shift[1] = shift2D[0]*localx[1] + shift2D[1]*localy[1];
		shift[2] = shift2D[0]*localx[2] + shift2D[1]*localy[2];

		itk::SeedPointFileIO::PrecisionPointType origin;
		origin[0] = m_seedPoints[i].x + shift[0];
		origin[1] = m_seedPoints[i].y + shift[1];
		origin[2] = m_seedPoints[i].z + shift[2];
		resampler->SetOutputOrigin(origin);

		InputImageType::SpacingType spacing;
		spacing[0] = m_ParametersF[1] / m_ParametersF[2];
		spacing[1] = m_ParametersF[1] / m_ParametersF[2];
		spacing[2] = 1;
		resampler->SetOutputSpacing(spacing);

		InputImageType::DirectionType direction;
		direction(0,0) = localx[0];
		direction(1,0) = localx[1];
		direction(2,0) = localx[2];
		direction(0,1) = localy[0];
		direction(1,1) = localy[1];
		direction(2,1) = localy[2];
		direction(0,2) = tangential[0];
		direction(1,2) = tangential[1];
		direction(2,2) = tangential[2];
		resampler->SetOutputDirection(direction);
		resampler->SetInterpolator(interpolator);
		resampler->Update();

		InputIteratorType crossIt(resampler->GetOutput(), resampler->GetOutput()->GetLargestPossibleRegion() );
		crossIt.GoToBegin();
		while (!crossIt.IsAtEnd())
		{
			InputImageType::IndexType currentIndex;
			currentIndex[0] = crossIt.GetIndex()[0];
			currentIndex[1] = crossIt.GetIndex()[1];
			currentIndex[2] = i;
			cmprIt.SetIndex(currentIndex);
			cmprIt.Set(crossIt.Get());
			++crossIt;
		}
	}

    std::cout<<"CMPRimage: "<<m_OutputImageN<<std::endl;
	WriterType::Pointer outputWriter = WriterType::New();
	std::cout<<"Writing output image file..."<<std::endl;
	outputWriter->SetFileName(m_OutputImageN);
	outputWriter->SetInput( m_cmprImage);
	try
	{
		outputWriter->Update();
	}
	catch(itk::ExceptionObject& e)
	{
		std::cout<<"Error while writing output file !"<<std::endl;
		std::cout<<e<<std::endl;
		return exit(0);
	}

}

/**
 * @brief ~vacCmprAlgorithm()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacCmprAlgorithm::~vacCmprAlgorithm()
{
	
}