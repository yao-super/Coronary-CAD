 /**
 * @file vacLevelSetAlgorithm.cpp
 * @brief
 * 
 *
 * @see vacLevelSetAlgorithm
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */

#include "vacLevelSetAlgorithm.h"

/**
 * @brief vacLevelSetAlgorithm()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacLevelSetAlgorithm::vacLevelSetAlgorithm():m_nImWidth(100),m_nImHeight(100),m_fMinThreshold(0),m_fMaxThreshold(12),m_nBytePerLine(0),
	m_iterNum(100),m_lambda1(1),m_nu(0.001*255*255),m_mu(1),m_timestep(0.1),m_epsilon(1.0)
{
	
}

/**
 * @brief LevelSetProcessImg()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacLevelSetAlgorithm::LevelSetProcessImg(InputImageType::Pointer &cutImgReader,InputIteratorType &empIt,int levels,int num)
{
	
	InputIteratorType It(cutImgReader, cutImgReader->GetLargestPossibleRegion());
	InputIteratorType It_t(cutImgReader, cutImgReader->GetLargestPossibleRegion());
	It_t.GoToBegin();
	It.GoToBegin();
	m_nBytePerLine = (m_nImWidth*24 + 31) / 8;
	m_chImageData = new unsigned char[m_nBytePerLine * m_nImHeight];
	m_fPixelData = new float [m_nImHeight * m_nImWidth];
	Rect a(35,35,25,25);
	for(int n =0; n<levels;++n)
	{
		
		for (int i = 0; i < m_nImHeight; i++)
			for (int j = 0; j < m_nImWidth; j++)
			{
				if(It.Get() < m_fMinThreshold)
					m_fPixelData[j + i*m_nImWidth] = 0;
				else if(It.Get() > m_fMaxThreshold)
					m_fPixelData[j + i*m_nImWidth] = 255;
				else
					m_fPixelData[j + i*m_nImWidth] = (It.Get() - m_fMinThreshold) * (255.0 / (m_fMaxThreshold - m_fMinThreshold));
				++It;
			}

			for (int ix = 0; ix < m_nImHeight; ix++)
			{
				for (int iy = 0; iy < m_nImWidth; iy++ )
				{
					m_chImageData[ix*m_nBytePerLine + iy*3]     = m_fPixelData[iy + ix*m_nImWidth];  //r
					m_chImageData[ix*m_nBytePerLine + iy*3 + 1] = m_fPixelData[iy + ix*m_nImWidth];  //g
					m_chImageData[ix*m_nBytePerLine + iy*3 + 2] = m_fPixelData[iy + ix*m_nImWidth]; //b
				}
			}
			m_chcutImage = QImage(m_chImageData, m_nImWidth, m_nImHeight, m_nBytePerLine, QImage::Format_RGB888);
			Mat img = cv::Mat(m_chcutImage.height(), m_chcutImage.width(), CV_8UC3, (void*)m_chcutImage.constBits(), m_chcutImage.bytesPerLine()); 
			cv::cvtColor(img, img, CV_RGB2BGR);
			initializePhi(img,a);
			EVolution();
			
			for (int nx = 0; nx < m_nImHeight; nx++)
		    {
				  for (int ny = 0; ny < m_nImWidth; ny++)
				  { 
					   InputImageType::IndexType currentIndex;
				       currentIndex[0] = It_t.GetIndex()[0];
				       currentIndex[1] = It_t.GetIndex()[1];
				       currentIndex[2] = 100*num+n;
				       empIt.SetIndex(currentIndex);
				       empIt.Set(m_cvResoutImg.at<Vec3b>(nx, ny)[0]);
					   ++It_t;
				  }
			}
			
	 }								
}

/**
 * @brief initializePhi()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacLevelSetAlgorithm::initializePhi(Mat img, Rect boxPhi)
{
	//boxPhi是前景区域  
	//m_iterNum = iterNum;  
	cvtColor(img, m_mImage, CV_BGR2GRAY);  

	m_iCol = img.cols;  
	m_iRow = img.rows;  
	m_depth = CV_32FC1;  

	//显式分配内存  
	m_mPhi = Mat::zeros(m_iRow, m_iCol, m_depth);  
	m_mDirac = Mat::zeros(m_iRow, m_iCol, m_depth);  
	m_mHeaviside = Mat::zeros(m_iRow, m_iCol, m_depth);  

	//初始化惩罚性卷积核  
	m_mK = (Mat_<float>(3, 3) << 0.5, 1, 0.5,  
		1, -6, 1,  
		0.5, 1, 0.5);  

	int c = 2;  
	for (int i = 0; i < m_iRow; i++)  
	{  
		for (int j = 0; j < m_iCol; j++)  
		{  
			if (i<boxPhi.y || i>boxPhi.y + boxPhi.height || j<boxPhi.x || j>boxPhi.x + boxPhi.width)  
			{  
				m_mPhi.at<float>(i, j) = -c;  
			}  
			else  
			{  
				m_mPhi.at<float>(i, j) = c;  
			}  
		}  
	}  
}

/**
 * @brief Dirac()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacLevelSetAlgorithm::Dirac()  
{  
	//狄拉克函数  
	float k1 = m_epsilon / CV_PI;  
	float k2 = m_epsilon*m_epsilon;  
	for (int i = 0; i < m_iRow; i++)  
	{  
		float *prtDirac = &(m_mDirac.at<float>(i, 0));  
		float *prtPhi = &(m_mPhi.at<float>(i, 0));  

		for (int j = 0; j < m_iCol; j++)  
		{  
			float *prtPhi = &(m_mPhi.at<float>(i, 0));  
			prtDirac[j] = k1 / (k2 + prtPhi[j] * prtPhi[j]);  
		}  
	}  
}  

/**
 * @brief Heaviside()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacLevelSetAlgorithm::Heaviside()  
{  
	//海氏函数  
	float k3 = 2 / CV_PI;  
	for (int i = 0; i < m_iRow; i++)  
	{  
		float *prtHeaviside = (float *)m_mHeaviside.ptr(i);  
		float *prtPhi = (float *)m_mPhi.ptr(i);  

		for (int j = 0; j < m_iCol; j++)  
		{  
			prtHeaviside[j] = 0.5 * (1 + k3 * atan(prtPhi[j] / m_epsilon));  
		}  
	}  
}  

/**
 * @brief Curvature()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacLevelSetAlgorithm::Curvature()  
{  
	//计算曲率  
	Mat dx, dy;  
	Sobel(m_mPhi, dx, m_mPhi.depth(), 1, 0, 1);  
	Sobel(m_mPhi, dy, m_mPhi.depth(), 0, 1, 1);  

	for (int i = 0; i < m_iRow; i++)  
	{  
		float *prtdx = (float *)dx.ptr(i);  
		float *prtdy = (float *)dy.ptr(i);  
		for (int j = 0; j < m_iCol; j++)  
		{  
			float val = sqrtf(prtdx[j] * prtdx[j] + prtdy[j] * prtdy[j] + 1e-10);  
			prtdx[j] = prtdx[j] / val;  
			prtdy[j] = prtdy[j] / val;  
		}  
	}  
	Mat ddx, ddy;  
	Sobel(dx, ddy, m_mPhi.depth(), 0, 1, 1);  
	Sobel(dy, ddx, m_mPhi.depth(), 1, 0, 1);  
	m_mCurv = ddx + ddy;  
}  

/**
 * @brief BinaryFit()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void  vacLevelSetAlgorithm::BinaryFit()  
{  
	//先计算海氏函数  
	Heaviside();  

	//计算前景与背景灰度均值  
	float sumFG = 0;  
	float sumBK = 0;  
	float sumH = 0;  
	//float sumFH = 0;  
	Mat temp = m_mHeaviside;  
	Mat temp2 = m_mImage;  
	float fHeaviside;  
	float fFHeaviside;  
	float fImgValue;  
	for (int i = 1; i < m_iRow; i++)  
	{  
		float *prtHeaviside = &(m_mHeaviside.at<float>(i, 0));  
		uchar *prtImgValue = &(m_mImage.at<uchar>(i, 0));  
		for (int j = 1; j < m_iCol; j++)  
		{  
			fImgValue = prtImgValue[j];  
			fHeaviside = prtHeaviside[j];  
			fFHeaviside = 1 - fHeaviside;  

			sumFG += fImgValue*fHeaviside;  
			sumBK += fImgValue*fFHeaviside;  
			sumH += fHeaviside;  
		}  
	}  
	m_FGValue = sumFG / (sumH + 1e-10);         //前景灰度均值  
	m_BKValue = sumBK / (m_iRow*m_iCol - sumH + 1e-10); //背景灰度均值  
} 

/**
 * @brief EVolution()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacLevelSetAlgorithm::EVolution()  
{  
	float fCurv;  
	float fDirac;  
	float fPenalize;  
	float fImgValue;  
	for (int i = 0; i < m_iterNum; i++)  
	{  
		Dirac();  
		Curvature();  
		BinaryFit();  
		filter2D(m_mPhi, m_mPenalize, m_depth, m_mK, Point(1, 1));//惩罚项的△φ  
		for (int i = 0; i < m_iRow; i++)  
		{  
			float *prtCurv = &(m_mCurv.at<float>(i, 0));  
			float *prtDirac = &(m_mDirac.at<float>(i, 0));  
			float *prtPenalize = &(m_mPenalize.at<float>(i, 0));  
			uchar *prtImgValue = &(m_mImage.at<uchar>(i, 0));  
			for (int j = 0; j < m_iCol; j++)  
			{  
				fCurv = prtCurv[j];  //惩罚项卷积
				fDirac = prtDirac[j];  //狄拉克处理后水平集
				fPenalize = prtPenalize[j];  //惩罚欧
				fImgValue = prtImgValue[j];  

				float lengthTerm = m_nu* fDirac * fCurv;                    //长度约束  
				float penalizeTerm = m_mu*(fPenalize - fCurv);                  //惩罚项  
				float areaTerm = fDirac * m_lambda1 *                       //全局项  
					(-((fImgValue - m_FGValue)*(fImgValue - m_FGValue))  
					+ ((fImgValue - m_BKValue)*(fImgValue - m_BKValue)));  

				m_mPhi.at<float>(i, j)=m_mPhi.at<float>(i, j)+m_timestep*(lengthTerm + penalizeTerm + areaTerm);  
			}  
		}  
     }  
	//显示每一次演化的结果  
		Mat showIMG;  
		//IplImage showIMG;
		cvtColor(m_mImage, showIMG, CV_GRAY2BGR);  
		Mat Mask = m_mPhi >= 0;   //findContours的输入是二值图像  
		dilate(Mask, Mask, Mat(), Point(-1, -1), 3);  
		erode(Mask, Mask, Mat(), Point(-1, -1), 3);  
		vector<vector<Point> > contours;  
		findContours(Mask,  
			contours,// 轮廓点  
			RETR_EXTERNAL,// 只检测外轮廓  
			CHAIN_APPROX_NONE);// 提取轮廓所有点  
		//drawContours(showIMG, contours, -1, Scalar(0, 0, 255), 1);
	    m_cvResoutImg = Mat::zeros(showIMG.rows, showIMG.cols, CV_8UC3);
		drawContours(m_cvResoutImg, contours, -1, Scalar(255,255,255), CV_FILLED);
		bitwise_and(m_cvResoutImg, showIMG, m_cvResoutImg);	

}

/**
 * @brief ~vacLevelSetAlgorithm()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacLevelSetAlgorithm::~vacLevelSetAlgorithm()
{
	delete[] m_chImageData;
	delete[] m_fPixelData;
}