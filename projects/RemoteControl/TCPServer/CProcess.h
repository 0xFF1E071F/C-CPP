#ifndef PROCESS_H
#define PROCESS_H

#include <QMainWindow>
#include <qbytearray.h>
#include <qmenu.h>
#include <qaction.h>
#include <qpoint.h>

namespace Ui {
class Process;
}

class Process : public QMainWindow
{
    Q_OBJECT

public:
    explicit Process(QWidget *parent = nullptr);
    ~Process();

	void SetSocket(size_t connId) { m_nconnId = connId; }
	size_t GetSocket() { return m_nconnId; }

	void ShowProcess(QByteArray baBuf);

	void CreateMenu();        //create menu
	void CreateAction();      //create action
	void CreateContextMenu(); //contextMenuPolicy
private:
    Ui::Process *ui;
	size_t m_nconnId;

	QMenu* m_pMenu;
	QAction* m_pActTerminate;

signals:
	void SendPidSig(QString pid);
public slots:
	//void OnCtxMenuSlot(const QPoint & pos); //槽函数名字和mainwindow里面的重复了
	void ProcessOnCtxMenuSlot(const QPoint & pos);
	void ProcessTerminateSlot();
};

#endif // PROCESS_H
