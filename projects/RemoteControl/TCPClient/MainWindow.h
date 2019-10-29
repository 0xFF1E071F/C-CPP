#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	bool Start();

private:
    Ui::MainWindow *ui;

signals:
	
public slots:
	void HeartBeat();
};