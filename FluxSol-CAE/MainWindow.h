#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Tetrahedron.h"
#include <QMainWindow>
class QAction;
class QLabel;
class FindDialog;
class Spreadsheet;
class MainWindow : public QMainWindow
{
Q_OBJECT
public:
MainWindow();
protected:
//void closeEvent(QCloseEvent *event);
// We define the class MainWindow as a subclass of QMainWindow. It contains the Q_
// OBJECT macro since it provides its own signals and slots.
// The closeEvent() function is a virtual function in QWidget that is automatically
// called when the user closes the window. It is reimplemented in MainWindow
// so that we can ask the user the standard question “Do you want to save your
// changes?” and to save user preferences to disk.
private slots:
//void newFile();
void open();
//bool save();
bool saveAs();
//void find();
//void goToCell();
//void sort();
//void about();

//void openRecentFile();
//void updateStatusBar();
//void spreadsheetModified();
private:
 void createActions();
 void createMenus();
 void createContextMenu();
 void createToolBars();
// void createStatusBar();
// void readSettings();
// void writeSettings();
// bool okToContinue();
// bool loadFile(const QString &fileName);
// bool saveFile(const QString &fileName);
// void setCurrentFile(const QString &fileName);
// void updateRecentFileActions();
QString strippedName(const QString &fullFileName);
// The main window needs some more private slots and several private functions
// to support the user interface.
Spreadsheet *spreadsheet;

Tetrahedron *tetrahedron;

FindDialog *findDialog;
QLabel *locationLabel;
QLabel *formulaLabel;
QStringList recentFiles;
QString curFile;
enum { MaxRecentFiles = 5 };
QAction *recentFileActions[MaxRecentFiles];
QAction *separatorAction;
QMenu *fileMenu;
QMenu *editMenu;
QMenu *viewMenu;

QToolBar *fileToolBar;
QToolBar *editToolBar;

QToolBar *toolbar;
QLayout *layout;

QAction *newAction;
QAction *openAction;
QAction *importAction;
QAction *exitAction;

QLabel *label;

QAction *aboutQtAction;
};
#endif
