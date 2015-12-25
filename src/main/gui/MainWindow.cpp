#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMimeData>

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// TODO Remove following code after testing
	manager = new StoneManager();
	manager->createStones(ui->widget);

	QVector<Gamestone*>& stones = manager->getStones();

	for (int i(0); i < stones.size(); i++)
	{
		stones.at(i)->move((i * 40) % 800, (int) (i / 20) * 65);
		// stones.at(i)->storeStone();
		stones.at(i)->setParent(Gamestone::HOLDER);

		// This makes the stones appear one after another
		stones.at(i)->show();
		update();
	}

	setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete manager;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	// We need to check here for ignore action since a gamestone
	// might already have handled the event
	if (event->mimeData()->hasFormat(GAMESTONE_MIMETYPE)
			&& !event->dropAction() == Qt::IgnoreAction)
	{
		event->acceptProposedAction();
	}
}

void MainWindow::dropEvent(QDropEvent *event)
{
	event->setDropAction(Qt::CopyAction);
	event->accept();
}
