/**
 * @file vacCmprTransformer.cpp
 * @brief
 * 
 *
 * @see vacCmprTransformer
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#include "vacCmprTransformer.h"
#include "vacCmprAlgorithm.h"

/**
 * @brief vacCmprTransformer()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacCmprTransformer::vacCmprTransformer()
{
     m_CmprTransAlg = new vacCmprAlgorithm;
}

/**
 * @brief GetInputFileName()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacCmprTransformer::GetInputFileName(QString& file)
{
	m_InitialImageN = file.toStdString();
	m_CmprTransAlg->SetInPutFileName(m_InitialImageN);
}

/**
 * @brief GetOutputFileName()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacCmprTransformer::GetOutputFileName(QString& file)
{
	m_InitialOutImageN = file.toStdString();
	m_CmprTransAlg->SetOutputFileName(m_InitialOutImageN);
}
/**
 * @brief GetcenterLineName()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacCmprTransformer::GetcenterLineName(QString& name)
{
	m_SeedPointN = name.toStdString(); 
	m_CmprTransAlg->SetCenterLineName(m_SeedPointN);
}

/**
 * @brief run()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacCmprTransformer::run()
{
	m_CmprTransAlg->CmprProcessImg();
	emit CmprProcessSucceed("原始图切片成功");
	emit CmprProcessComplete();
}

/**
 * @brief ~vacCmprTransformer()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacCmprTransformer::~vacCmprTransformer()
{
	delete m_CmprTransAlg;
}