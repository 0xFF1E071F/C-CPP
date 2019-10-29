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

	//����socket
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
	//��¼��ǰ�������ĸ��ͻ������ӣ���Ϊ����˻�Ҫ��ͻ�����������Ϣ�����ж���ͻ��ˣ���Ҫ����
	//boost::unordered_map<int, std::shared_ptr<CSession>>::iterator& m_iter;
	int m_nconnId;

signals:
	void SendCmdLineSig(int, QString);

public slots:
	void SendDataSlot();
};

#endif // CMD_H
