/**
 * @file vacMainInterface.h
 * @brief
 * 
 *
 * @see vacMainInterface
 *
 * @author Y. Y. 
 * @version 0.0.1
 * @date 2016/10/17
 */
#ifndef VACMAININTERFACE_H
#define VACMAININTERFACE_H
#include <QMainWindow>

class QAction;
class vacQtView;
class QLabel;
class vacCmprTransformer;
class vacMessageBox;
class vacProgressBar;
class vacVnandLvTransformer;
/** 
 * @class vacMainInterface
 * @brief . 
 * 
 *
 * @ingroup CoronaryCAD
 */
class vacMainInterface : public QMainWindow
{
	Q_OBJECT 
public:
	vacMainInterface();
signals: 
	void SuccessfulMsg(QString text);
	void ReadCenterLineMsg(QString text);
	void NoCenterLineMsg(QString text);
	void NoStrImgMsg(QString text);
private slots:
	bool OnOpenOriImg();
	bool OnOpenCutImg();
	void OnAbout();
	void OnCreateMsgBox(QString text);
	void OnGetCenterLine();
	bool OnCreateStrImgView();
	bool OnCreateOriImgView();
	void OnDestroyQtView();
	void OnDestroyTransverter();
private:
	void __CreateActions();
	void __CreateMenus();
	void __CreateStatusBar();
	void __CreateCmprThread();
	void __CreateCutImgThread();
	
    vacQtView *m_QtView;
	QLabel *m_FormulaLabel;
	QString m_QFilename; 
	QString m_QFileOutname;

	QString m_QCutFileName;
	QString m_QCutFileOutName;

	vacCmprTransformer *m_CmprTransverter;
	QString m_QCenterLineName;
	vacVnandLvTransformer *m_VnandLsTransverter;
	QMenu *m_QFileMenu;
	QMenu *m_QProgressMenu;
	QMenu *m_QHelpMenu;

	QAction *m_QImportCenterLineAction;
	QAction *m_QImportOriImgAction;
	QAction *m_QImportStrImgAction;
	QAction *m_QGetOriImg;
	QAction *m_QGetStrImg;
	QAction *m_QExitAction;
	QAction *m_QAboutAction;

	vacMessageBox *m_MsgBox;
	vacProgressBar *m_ProgressBar;
protected:
    void paintEvent(QPaintEvent * e );

};

#endif // VACMAININTERFACE_H