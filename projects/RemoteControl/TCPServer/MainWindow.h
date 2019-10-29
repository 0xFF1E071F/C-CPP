#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qobject.h>
#include "CAsio.h"
#include <qmenu.h>
#include "CScreen.h"
#include "CCmd.h"
#include "CProcess.h"
#include <qbytearray.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	void CreateAction();      //create action
	void CreateMenu();        //create menu
	void CreateContextMenu(); //contextMenuPolicy

    void SetAttr();//tableWidget属性设置
    void SetConnect(); //设置信号槽

	//std::shared_ptr<CAsio> m_pAsio;
private:
    Ui::MainWindow *ui;

	QMenu* m_pMenu;
	QAction* m_pActScreen;
	QAction* m_pActFile;
	QAction* m_pActCmd;
	QAction* m_pActProcess;

	Screen* m_pScreen;
	Cmd* m_pCmd;
	Process* m_pProcess;

	//CSession m_session;

signals:
    void ShowConnSig();
	void SendConnSig();
public slots:
	void HeartBeat(); //心跳
    void GetConnSlot();
	void OnCtxMenuSlot(const QPoint & pos);
	void GetScreenSlot();
	//void GetDataSlot(int msgType);
	void ShowScreenSlot(const char* pData, int nlen);
	void GetCmdSlot();
	void ShowCmdSlot(QByteArray);

	void SendCmdLineSlot(int connId, QString cmdline);
	void CloseScreenSlot();//关闭发送屏幕数据

	void GetPorcessSlot();//进程获取
	void ShowProcessSlot(QByteArray);//进程显示
	void TerminatePidSlot(QString); //结束进程
	

};

#endif // MAINWINDOW_H
