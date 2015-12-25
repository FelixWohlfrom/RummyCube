#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>

#include "../game/stones/StoneManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	/**
	 * Listener for event when stone is dragged over main window
	 */
	void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

	/**
	 * Listener for event when stone is dropped on main window
	 */
	void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;

    StoneManager* manager;
};

#endif // MAINWINDOW_H
