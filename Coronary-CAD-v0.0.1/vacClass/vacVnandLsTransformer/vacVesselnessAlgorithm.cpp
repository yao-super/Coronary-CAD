 /**
 * @file vacVesselnessAlgorithm.cpp
 * @brief
 * 
 *
 * @see vacVesselnessAlgorithm
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */

#include "vacVesselnessAlgorithm.h"
#include "vacLevelSetAlgorithm.h"

/**
 * @brief vacVesselnessAlgorithm()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacVesselnessAlgorithm::vacVesselnessAlgorithm():m_InputImgSize(0)
{
	

}

/**
 * @brief ~vacVesselnessAlgorithm()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacVesselnessAlgorithm::~vacVesselnessAlgorithm()
{

}

float vacVesselnessAlgorithm::m_sigma = 2;
float vacVesselnessAlgorithm::m_alpha1=0.5;
float vacVesselnessAlgorithm::m_alpha2=0.5;

/**
 * @brief SetInputImgSize()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacVesselnessAlgorithm::SetInputImgSize(float size)
{
	m_InputImgSize = size;
}

/**
 * @brief SetInPut3DImgName()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacVesselnessAlgorithm::SetInPut3DImgName(string& file)
{
	m_InitialImageN = file;
}

/**
 * @brief SetOutput3DImgName()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacVesselnessAlgorithm::SetOutput3DImgName(string& file)
{
	m_OutputImageN = file;
}

/**
 * @brief Process3DImg()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacVesselnessAlgorithm::Process3DImg()
{
	
	ReaderType::Pointer m_InitialImgReader = ReaderType::New();
	m_InitialImgReader->SetFileName(m_InitialImageN);
	m_InitialImgReader->Update();

	InputImageType::Pointer oriImgPt = m_InitialImgReader->GetOutput();
	InputIteratorType OriIt(oriImgPt, oriImgPt->GetLargestPossibleRegion());
	OriIt.GoToBegin();

	InputImageType::Pointer EmptyPt = InputImageType::New();
	Create3DEmptyImg(m_InputImgSize,EmptyPt);
	InputIteratorType emptyit = InputIteratorType(EmptyPt,EmptyPt->GetLargestPossibleRegion());
	emptyit.GoToBegin();

	int last = (m_InputImgSize%100==0?100:m_InputImgSize%100);

	int num=(last==100?m_InputImgSize/100:(m_InputImgSize/100)+1); 


	vector<InputIteratorType> oriitvec;
	vector<InputIteratorType> emptyitvec;

	for(int i=0;i<m_InputImgSize*100*100;++i)
	{
		if(i%1000000==0)
		{
			oriitvec.push_back(OriIt);
			emptyitvec.push_back(emptyit);
		}
		++OriIt;
		++emptyit;
	}

	vector<StcUnion> StcUnionVec;
	StcUnion Unionpt;
	for(int i=0;i<num;++i)
	{
		Unionpt.empImg_it = emptyitvec[i];
		Unionpt.oriImg_it = oriitvec[i];
		Unionpt.nums = i;
		Unionpt.levs = (i==num-1?last:100);
		StcUnionVec.push_back(Unionpt);
	}

	HANDLE *handle = new HANDLE[num];
	for (int i = 0; i < num; i++)
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, vacVesselnessAlgorithm::ProcessImgThread, &StcUnionVec[i], 0, NULL);
	WaitForMultipleObjects(num, handle, TRUE, INFINITE);
	delete[] handle;

	WriterType::Pointer outputWriter = WriterType::New();
	outputWriter->SetFileName(m_OutputImageN);
	outputWriter->SetInput(EmptyPt);
	try
	{
		outputWriter->Update();
	}
	catch(itk::ExceptionObject& e)
	{
		std::cout<<"Error while writing output file !"<<std::endl;
		std::cout<<e<<std::endl;
	}
}

/**
 * @brief Create3DEmptyImg()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacVesselnessAlgorithm::Create3DEmptyImg(int levels,InputImageType::Pointer &emptyImage)
{
	InputImageType::IndexType index;
	index.Fill(0);
	InputImageType::SizeType size;
	size[0] = 100;
	size[1] = 100;
	size[2] = levels;

	InputImageType::RegionType region;
	region.SetIndex(index);
	region.SetSize(size);

	InputImageType::SpacingType spacing;
	spacing[0] = 10.0/100;
	spacing[1] = 10.0/100;
	spacing[2] = 1.0;

	InputImageType::PointType origin;
	origin[0]=0;
	origin[1]=0;
	origin[2]=0; 

	emptyImage->SetSpacing(spacing);
	emptyImage->SetRegions(region);
	emptyImage->SetOrigin(origin);
	emptyImage->Allocate();
	emptyImage->FillBuffer(itk::NumericTraits<PixelType>::One);
}

/**
 * @brief ProcessImgThread()
 *
 * 
 * @param . 
 *
 * @return None.
 */
unsigned int __stdcall vacVesselnessAlgorithm:: ProcessImgThread(PVOID iter)
{
	StcUnion pt=*((StcUnion*)iter);
	int num = pt.levs;
	InputIteratorType it = pt.oriImg_it;
	InputImageType::Pointer m_newIt = InputImageType::New();

	vacVesselnessAlgorithm::Create3DOriImg(num,it,m_newIt);

	HessianFilterType::Pointer hessianFilter = HessianFilterType::New();
	hessianFilter->SetInput( m_newIt );
	
	hessianFilter->SetSigma(m_sigma);

	VesselnessMeasureFilterType::Pointer vesselnessFilter = VesselnessMeasureFilterType::New();
	vesselnessFilter->SetInput( hessianFilter->GetOutput() );
	vesselnessFilter->SetAlpha1(m_alpha1);
	vesselnessFilter->SetAlpha2(m_alpha2);
	vesselnessFilter->Update();

	InputImageType::Pointer vesselImg = vesselnessFilter->GetOutput();
	InputIteratorType ImgIt = pt.empImg_it;
	int m = pt.nums;

	vacLevelSetAlgorithm vacLevelSetAlg;

	vacLevelSetAlg.LevelSetProcessImg(vesselImg,ImgIt,num,m);

	return 0;
}

/**
 * @brief Create3DOriImg()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacVesselnessAlgorithm::Create3DOriImg(int levels,InputIteratorType &oriIt,InputImageType::Pointer &m_newImage)
{
	InputImageType::IndexType index;
	index.Fill(0);
	InputImageType::SizeType size;
	size[0] = 100;
	size[1] = 100;
	size[2] = levels;

	InputImageType::RegionType region;
	region.SetIndex(index);
	region.SetSize(size);

	InputImageType::SpacingType spacing;
	spacing[0] = 10.0/100;
	spacing[1] = 10.0/100;
	spacing[2] = 1.0;

	InputImageType::PointType origin;
	origin[0]=0;
	origin[1]=0;
	origin[2]=0; 


	m_newImage->SetSpacing(spacing);
	m_newImage->SetRegions(region);
	m_newImage->SetOrigin(origin);
	m_newImage->Allocate();
	m_newImage->FillBuffer(itk::NumericTraits<PixelType>::One);

	InputIteratorType newImgIt(m_newImage, m_newImage->GetLargestPossibleRegion() );
	newImgIt.GoToBegin();

	for (int n=0;n<levels;++n)
	{
		for(int i=0;i<100;++i)
			for(int j=0;j<100;++j)
			{
				InputImageType::IndexType currentIndex;
				currentIndex[0] = oriIt.GetIndex()[0];
				currentIndex[1] = oriIt.GetIndex()[1];
				currentIndex[2] = n;
				newImgIt.SetIndex(currentIndex);
				newImgIt.Set(oriIt.Get());
				++oriIt;
			}
	}

}