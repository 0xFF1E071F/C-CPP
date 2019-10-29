#ifndef CMD_H
#define CMD_H

#include <QMainWindow>
#include <boost/unordered_map.hpp>
#include <qbytearray.h>

class CSession;

namespace Ui {
class Cmd;
}

class Cmd : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cmd(QWidget *parent = nullptr);
    ~Cmd();

	void ShowData(QByteArray);

	//设置socket
	/*void SetSocket(boost::unordered_map<int, std::shared_ptr<CSession>>::iterator& iter)
	{
		m_iter = iter;
	}*/
	void SetSocket(int connId)
	{
		m_nconnId = connId;
	}
	
private:
    Ui::Cmd *ui;
	//记录当前窗口是哪个客户端连接，因为服务端还要向客户端主动发消息并且有多个客户端，需要区分
	//boost::unordered_map<int, std::shared_ptr<CSession>>::iterator& m_iter;
	int m_nconnId;

signals:
	void SendCmdLineSig(int, QString);

public slots:
	void SendDataSlot();
};

#endif // CMD_H
