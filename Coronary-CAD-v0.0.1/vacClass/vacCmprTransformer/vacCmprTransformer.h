/**
 * @file vacCmprTransformer.h
 * @brief
 * 
 *
 * @see vacCmprTransformer
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef VACCMPRTRANSFORMER_H
#define VACCMPRTRANSFORMER_H

#include <iostream>
#include <QDialog>
#include <QThread>
#include <string>
using namespace std;

/** 
 * @class vacCmprTransformer
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacCmprAlgorithm;
class vacCmprTransformer : public QThread
{
	Q_OBJECT
public:
	vacCmprTransformer();
	~vacCmprTransformer();
	void GetInputFileName(QString& file);
	void GetOutputFileName(QString& file);
	void GetcenterLineName(QString& name);
signals:
	void CmprProcessSucceed(QString text);
	void CmprProcessComplete();
protected:
	void run();
private:
	vacCmprAlgorithm *m_CmprTransAlg;
	std::string m_InitialImageN;
	std::string m_InitialOutImageN;
	std::string m_SeedPointN;
};

#endif //VACCMPRTRANSFORMER_H