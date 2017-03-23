/**
 * @file vacMainInterface.cpp
 * @brief
 * 
 *
 * @see vacMainInterface
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#include <QtGui>
#include <QApplication>
#include "vacQtView.h"
#include "vacMainInterface.h"
#include "vacMessageBox.h"
#include "vacProgressBar.h"
#include "vacCmprTransformer.h"
#include "vacVnandLsTransformer.h"
/**
 * @brief vacMainInterface()
 *
 * 
 * @param . 
 *
 * @return None.
 */
vacMainInterface::vacMainInterface()
{
	resize(640,480);
	m_QtView = 0;
	m_MsgBox = 0;
	m_ProgressBar = 0;
	m_CmprTransverter = 0;
	m_VnandLsTransverter = 0;
	m_QCenterLineName = "";
	m_QFilename = "";
	m_QCutFileName = "";
	m_QCutFileOutName = "";
	//setWindowOpacity(0.7);
	setWindowTitle("ITK_IMG_SHOW");
	__CreateActions();
	__CreateMenus();
	__CreateStatusBar();
}

/**
 * @brief __CreateActions()
 *
 * Create all need QActions . 
 *
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::__CreateActions()
{
	m_QImportCenterLineAction = new QAction(QIcon("D:/image/open.png"), tr("&导入中心线"), this);
	m_QImportCenterLineAction->setShortcut(QKeySequence::Open); 
	m_QImportCenterLineAction->setStatusTip(tr("导入中心线"));
	connect(m_QImportCenterLineAction, SIGNAL(triggered()), this, SLOT(OnGetCenterLine()));
	
	m_QExitAction = new QAction(tr("退出"), this); 
	m_QExitAction->setShortcut(tr("Ctrl+Q")); 
	m_QExitAction->setStatusTip(tr("退出此应用")); 
	connect(m_QExitAction, SIGNAL(triggered()), this, SLOT(close())); 

	m_QImportOriImgAction = new QAction(QIcon("D:/image/show.png"), tr("导入原始图"), this);
	m_QImportOriImgAction->setShortcut(tr("Ctrl+S"));
	m_QImportOriImgAction->setStatusTip(tr("正在导入原始图")); 
	connect(m_QImportOriImgAction, SIGNAL(triggered()), this, SLOT(OnOpenOriImg()));

	m_QAboutAction = new QAction(QIcon("D:/image/aboutqt.png"), tr("&关于"), this);
	m_QAboutAction->setStatusTip(tr("关于此应用"));
	connect(m_QAboutAction, SIGNAL(triggered()), this, SLOT(OnAbout()));

	m_QImportStrImgAction = new QAction(QIcon("D:/image/qt.png"), tr("导入增强图"), this);
	m_QImportStrImgAction->setStatusTip(tr("正在导入增强图"));
	connect(m_QImportStrImgAction, SIGNAL(triggered()), this, SLOT(OnOpenCutImg()));

	m_QGetOriImg = new QAction(tr("提取原始图截图"), this);
	m_QGetOriImg->setStatusTip(tr("正在提取原始图截图"));
	connect(m_QGetOriImg, SIGNAL(triggered()), this, SLOT(OnCreateOriImgView()));
	m_QGetStrImg = new QAction(tr("提取增强图截图"), this);
	m_QGetStrImg->setStatusTip(tr("正在提取增强图截图"));
	connect(m_QGetStrImg, SIGNAL(triggered()), this, SLOT(OnCreateStrImgView()));
}

/**
 * @brief __CreateMenus()
 *
 * Create all need QMenus .
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::__CreateMenus()
{
	m_QFileMenu = menuBar()->addMenu(tr("&文件"));
	m_QFileMenu->addAction(m_QImportCenterLineAction);
	m_QFileMenu->addAction(m_QImportOriImgAction);
	m_QFileMenu->addAction(m_QImportStrImgAction);
    m_QFileMenu->addAction(m_QExitAction);

	m_QProgressMenu = menuBar()->addMenu(tr("&处理"));
	m_QProgressMenu->addAction(m_QGetOriImg);
	m_QProgressMenu->addAction(m_QGetStrImg);

	menuBar()->addSeparator();

	m_QHelpMenu = menuBar()->addMenu(tr("&帮助"));
	m_QHelpMenu->addAction(m_QAboutAction);	
}

/**
 * @brief __CreateStatusBar()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::__CreateStatusBar()
{
	m_FormulaLabel = new QLabel;;
	m_FormulaLabel->setIndent(3);

	statusBar()->addWidget(m_FormulaLabel, 1);
}

/**
 * @brief OnOpen()
 *
 * 
 * @param . 
 *
 * @return None.
 */
bool vacMainInterface::OnOpenOriImg()
{
	  if(m_QCenterLineName.isEmpty())
	  {
		  connect(this, SIGNAL(NoCenterLineMsg(QString)),
			  this, SLOT(OnCreateMsgBox(QString)));
		  emit NoCenterLineMsg("请先导入中心线！");
		  return false;
	  }
	  m_ProgressBar = new vacProgressBar;
	  m_QFilename = QFileDialog::getOpenFileName(this,
			tr("OnOpen ITK"), "/",
			tr("ITK files (*.nii.gz)"));
	  if (m_QFilename.isEmpty())
		  return false;
	  else
	  {
		  m_QFileOutname = QFileDialog::getSaveFileName(this,
			  tr("Save directions"), "/out",
			  tr("Output files (*.nii.gz)"));
		  if (m_QFileOutname.isEmpty())
		  {
			  return false;
		  }
	     __CreateCmprThread();
		 return true;
	  }
}

bool vacMainInterface::OnOpenCutImg()
{
	m_VnandLsTransverter = new vacVnandLvTransformer;
	m_ProgressBar = new vacProgressBar;
	m_QCutFileName = QFileDialog::getOpenFileName(this,
		tr("OnOpen ITK"), "/",
		tr("ITK files (*.nii.gz)"));
	if (m_QCutFileName.isEmpty())
		return false;
	else
	{
		m_QCutFileOutName = QFileDialog::getSaveFileName(this,
			tr("Save directions"), "/cut",
			tr("Output files (*.nii.gz)"));
		if (m_QCutFileOutName.isEmpty())
		{
			return false;
		}
		__CreateCutImgThread();
		return true;
	}
}

/**
 * @brief OnGetCenterLine()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::OnGetCenterLine()
{
	m_CmprTransverter = new vacCmprTransformer;
	m_QCenterLineName = QFileDialog::getOpenFileName(this,
		tr("OnOpen CenterLine"), "/",
		tr("CenterLine files (*.vtk)"));
	if (m_QCenterLineName.isEmpty())
	{
		exit(1);
	}
	else
	{
		m_CmprTransverter->GetcenterLineName(m_QCenterLineName);
		connect(this, SIGNAL(ReadCenterLineMsg(QString)),
			this, SLOT(OnCreateMsgBox(QString)));
		emit ReadCenterLineMsg("中心线导入成功");
	}
}

/**
 * @brief __CreateCmprThread()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::__CreateCmprThread()
{  
    // when process compete, show messagebox
	connect(m_CmprTransverter, SIGNAL(CmprProcessSucceed(QString)),
		this, SLOT(OnCreateMsgBox(QString)),Qt::BlockingQueuedConnection);  
	connect(m_CmprTransverter, SIGNAL(CmprProcessComplete()),
		m_ProgressBar, SLOT(OnStopProgressBar()));
	connect(m_CmprTransverter, SIGNAL(CmprProcessComplete()),
		this, SLOT(OnDestroyTransverter()));
	// read the file		
	m_CmprTransverter->GetInputFileName(m_QFilename); 
	m_CmprTransverter->GetOutputFileName(m_QFileOutname);

	// strat the process thread
	m_CmprTransverter->start(); 
	m_ProgressBar->show();
	m_ProgressBar->raise();
	m_ProgressBar->activateWindow();
}


void vacMainInterface::__CreateCutImgThread()
{
	connect(m_VnandLsTransverter, SIGNAL(ImgProcessSucceed(QString)),
		this, SLOT(OnCreateMsgBox(QString)),Qt::BlockingQueuedConnection);  
	connect(m_VnandLsTransverter, SIGNAL(ImgProcessComplete()),
		m_ProgressBar, SLOT(OnStopProgressBar()));
	connect(m_VnandLsTransverter, SIGNAL(ImgProcessComplete()),
		this, SLOT(OnDestroyTransverter()));
	// read the file		
	m_VnandLsTransverter->GetInput3DImageFileName(m_QCutFileName); 
	m_VnandLsTransverter->GetOutput3DImageFileName(m_QCutFileOutName);
	m_VnandLsTransverter->Get3DImageSize();
	// strat the process thread
	m_VnandLsTransverter->start(); 
	m_ProgressBar->show();
	m_ProgressBar->raise();
	m_ProgressBar->activateWindow();
}

/**
 * @brief OnDestroyTransverter()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::OnDestroyTransverter()
{
	if(m_CmprTransverter)
	{
		delete m_CmprTransverter;
	    m_CmprTransverter = 0;
	}
	
	if(m_VnandLsTransverter)
	{
		delete m_VnandLsTransverter;
		m_VnandLsTransverter = 0;
	}
	delete m_ProgressBar;
	m_ProgressBar = 0;
}

/**
 * @brief OnCreateStrImgView()
 *
 * 
 * @param . 
 *
 * @return None.
 */
bool vacMainInterface::OnCreateStrImgView()
{
		if(!m_QtView)
		{
			connect(this, SIGNAL(NoStrImgMsg(QString)),
				this, SLOT(OnCreateMsgBox(QString)));
			emit NoStrImgMsg("请先提取原始图截图！");
			return false;
		}
		m_QtView->ShowStrImgFun();
	    return true;
}

/**
 * @brief OnDestroyQtView()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::OnDestroyQtView()
{
	delete m_QtView;
	m_QtView = 0;
}
/**
 * @brief OnCreateOriImgView()
 *
 * 
 * @param . 
 *
 * @return None.
 */
bool vacMainInterface::OnCreateOriImgView()
{

	if(!m_QtView)
	{
		m_QtView = new vacQtView(this);
	}
	connect(m_QtView,SIGNAL(DestroyQtView()),
		this,SLOT(OnDestroyQtView()));
	m_QtView->show();
	m_QtView->raise();
	m_QtView->activateWindow();
    m_QtView->ShowOriImgFun();
	return true;  
}

/**
 * @brief OnAbout()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::OnAbout()
{
	QMessageBox::about(this, tr("OnAbout ITKSHOW"),
		tr("<h2>ITKSHOW 1.1</h2>"
		"<p>Copyright &copy; 2016 Software Inc."
		"<p>Spreadsheet is a small application that "
		"demonstrates QAction, QMainWindow, QMenuBar, "
		"QStatusBar, QTableWidget, QToolBar, and many other "
		"Qt classes."));
}

/**
 * @brief paintEvent()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::paintEvent( QPaintEvent * e )
{
	QPainter p(this);
    p.drawPixmap(0, 0, width(), height(), QPixmap("D:/image/4.jpg"));
	
}

/**
 * @brief OnCreateMsgBox()
 *
 * 
 * @param . 
 *
 * @return None.
 */
void vacMainInterface::OnCreateMsgBox(QString text)
{
	if(!m_MsgBox)
	{
		m_MsgBox = new vacMessageBox;
	}
	connect(this, SIGNAL(SuccessfulMsg(QString )),
		m_MsgBox, SLOT(OnShowMessage(QString)));
	emit SuccessfulMsg(text);
	m_MsgBox->show(); 
	m_MsgBox->raise();
	m_MsgBox->activateWindow();
}

