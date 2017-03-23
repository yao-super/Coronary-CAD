/**
 * @file vacVnandLvTransformer.cpp
 * @brief
 * 
 *
 * @see vacVnandLvTransformer
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#include "vacVnandLsTransformer.h"
#include "vacVesselnessAlgorithm.h"
#include "vacLevelSetAlgorithm.h"

/**
 * @brief vacVnandLvTransformer()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacVnandLvTransformer::vacVnandLvTransformer()
{
	m_VesselnessAlg = new vacVesselnessAlgorithm;
}

/**
 * @brief GetInput3DImageFileName()
 *
 * 
 * @param . 
 *
 * @return None.
 */

void vacVnandLvTransformer::GetInput3DImageFileName(QString& file)
{
	m_InputImageN = file.toStdString();
	m_VesselnessAlg->SetInPut3DImgName(m_InputImageN);
}

/**
 * @brief GetOutput3DImageFileName()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacVnandLvTransformer::GetOutput3DImageFileName(QString& file)
{
	m_OutputImageN = file.toStdString();
	m_VesselnessAlg->SetOutput3DImgName(m_OutputImageN);
}

/**
 * @brief Get3DImageSize()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacVnandLvTransformer::Get3DImageSize()
{
	ReaderType::Pointer ImageReader=ReaderType::New();
	ImageReader->SetFileName(m_InputImageN);
	ImageReader->Update();
	InputImageType::Pointer reader = ImageReader->GetOutput();
	InputIteratorType Itb(reader,reader->GetLargestPossibleRegion() );
	Itb.GoToBegin();
	while(!Itb.IsAtEnd())
		++Itb;

	float m_3DImageSize = (--Itb).GetIndex()[2];
	m_VesselnessAlg->SetInputImgSize(m_3DImageSize);

}

/**
 * @brief run()
 *
 * Strat Process the 3D Image
 * 
 * @param . 
 *
 * @return None.
 */
void vacVnandLvTransformer::run()
{
	m_VesselnessAlg->Process3DImg();
	emit ImgProcessSucceed("Í¼Æ¬ÇÐ¸îÍê³É");
	emit ImgProcessComplete();
}

/**
 * @brief ~vacVnandLvTransformer()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacVnandLvTransformer::~vacVnandLvTransformer()
{
	delete m_VesselnessAlg;
}