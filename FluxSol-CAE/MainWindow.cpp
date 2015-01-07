#include <QtGui>
#include <qmenubar>
#include <qtoolbar>
#include <qfiledialog>
#include <qtoolbutton>
#include <qlabel>
#include <qlayout>
//#include "finddialog.h"
//#include "gotocelldialog.h"
#include "MainWindow.h"
//#include "sortdialog.h"
//#include "spreadsheet.h"

#include <iostream>
using namespace std;

MainWindow::MainWindow()
{

//openAction   = new QAction(tr("&Open"), this);
//importAction = new QAction(tr("&Import"), this);
//saveAction = new QAction(tr("&Save"), this);
//exitAction = new QAction(tr("E&xit"), this);

	//spreadsheet = new Spreadsheet;
	createActions();
	createMenus();
	//createContextMenu();
    createToolBars();
	//createStatusBar();
	//readSettings();
	//findDialog = 0;
	//setWindowIcon(QIcon(":/images/icon.png"));
	//setCurrentFile("");

	//cout << "Creating tetra"<<endl;
	tetrahedron=new Tetrahedron();
	//cout<<"placing tetra"<<endl;
	this->resize(600, 400);
	//tetrahedron->resize(300, 300);
	setCentralWidget(tetrahedron);
    tetrahedron->show();
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&Archivo"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	//fileMenu->addAction(importAction);
	//fileMenu->addAction(saveAction);
	//fileMenu->addAction(saveAsAction);
	//separatorAction = fileMenu->addSeparator();
	//for (int i = 0; i < MaxRecentFiles; ++i)
	//fileMenu->addAction(recentFileActions[i]);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	// In Qt, menus are instances of QMenu. The addMenu() function creates a QMenu
	// widget with the specified text and adds it to the menu bar. The QMainWindow::
	// menuBar() function returns a pointer to a QMenuBar. The menu bar is created the
	// first time menuBar() is called.
	// We start by creating the File menu and then add the New, Open, Save, and
	// Save As actions to it. We insert a separator to visually group closely related
	// items together. We use a for loop to add the (initially hidden) actions from the
	// recentFileActions array, and then add the exitAction action at the end.
	// We have kept a pointer to one of the separators. This will allow us to hide the
	// separator (if there are no recent files) or to show it, since we do not want to
	// show two separators with nothing in between.
	viewMenu = menuBar()->addMenu(tr("&Ver"));
	//editMenu->addAction(cutAction);
	//editMenu->addAction(copyAction);

	//editMenu->addAction(pasteAction);
	//editMenu->addAction(deleteAction);
	//selectSubMenu = editMenu->addMenu(tr("&Select"));
	//selectSubMenu->addAction(selectRowAction);
	//selectSubMenu->addAction(selectColumnAction);
	//selectSubMenu->addAction(selectAllAction);
	//editMenu->addSeparator();
	//editMenu->addAction(findAction);
	//editMenu->addAction(goToCellAction);
	// Now we create the Edit menu, adding actions with QMenu::addAction() as we
	// did for the File menu, and adding the submenu with QMenu::addMenu() at the
	// position where we want it to appear. The submenu, like the menu it belongs
	// to, is a QMenu.
	//toolsMenu = menuBar()->addMenu(tr("&Tools"));
	//toolsMenu->addAction(recalculateAction);
	//toolsMenu->addAction(sortAction);
	//optionsMenu = menuBar()->addMenu(tr("&Options"));
	//optionsMenu->addAction(showGridAction);
	//optionsMenu->addAction(autoRecalcAction);
	menuBar()->addSeparator();
	//helpMenu = menuBar()->addMenu(tr("&Help"));
	//helpMenu->addAction(aboutAction);
	//helpMenu->addAction(aboutQtAction);

	//connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
}

void MainWindow::createContextMenu()
{
	//spreadsheet->addAction(cutAction);
	//spreadsheet->addAction(copyAction);
	//spreadsheet->addAction(pasteAction);
	//spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

// A more sophisticated way of providing context menus is to reimplement the
// QWidget::contextMenuEvent() function, create a QMenu widget, populate it with
// the desired actions, and call exec() on it.

void MainWindow::createToolBars()
{
    //label= new QLabel(this);
    QToolButton *newm=new QToolButton();
    QToolButton *load=new QToolButton();
    newm->setIcon(QIcon("./images/new.png"));
    load->setIcon(QIcon("./images/open.png"));
    //label->setPixmap(QPixmap::fromImage((*image)));
    //label->setScaledContents(false);
    //label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toolbar=new QToolBar(this);
    layout=new QVBoxLayout(this);
    //layout->addWidget(label);
    layout->addWidget(toolbar);
    //addToolBar(Qt::BottomToolBarArea, toolbar);
    addToolBar(toolbar);

    toolbar->addWidget(newm);
    toolbar->addWidget(load);

	//fileToolBar = addToolBar(tr("&File"));
	//fileToolBar->addAction(newAction);
	//fileToolBar->addAction(openAction);
	//fileToolBar->addAction(saveAction);
	//editToolBar = addToolBar(tr("&Edit"));
	//editToolBar->addAction(cutAction);
	//editToolBar->addAction(copyAction);
	//editToolBar->addAction(pasteAction);
	//editToolBar->addSeparator();
	//editToolBar->addAction(findAction);
	//editToolBar->addAction(goToCellAction);

	//setScaledComponents(false);
}

bool MainWindow::saveAs()
{
QString fileName = QFileDialog::getSaveFileName(this,
tr("Save Spreadsheet"), ".",
tr("Spreadsheet files (*.sp)"));
if (fileName.isEmpty())
return false;
//return saveFile(fileName);
}

void MainWindow::open()
{
	//if (okToContinue())
	{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("Open Spreadsheet"), ".",
	tr("Spreadsheet files (*.sp)"));
	if (!fileName.isEmpty())
	{}
	//loadFile(fileName);
	}
}

void MainWindow::createActions()
{
	newAction = new QAction(tr("&Nuevo"), this);
	newAction->setIcon(QIcon("./images/new.png"));
	//newAction->setIcon(QIcon("new.png"));
	newAction->setShortcut(tr("Ctrl+N"));
	newAction->setStatusTip(tr("Create a new spreadsheet file"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

	openAction = new QAction(tr("&Abrir"), this);
	openAction->setIcon(QIcon("./images/open.png"));
	//newAction->setIcon(QIcon("new.png"));
	openAction->setShortcut(tr("Ctrl+A"));
	openAction->setStatusTip(tr("Abrir un archivo existente"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

	exitAction = new QAction(tr("S&alir"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Salir de la aplicación"));
    connect(exitAction, SIGNAL(triggered()),qApp, SLOT(closeAllWindows()));
}
