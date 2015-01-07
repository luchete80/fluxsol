#include <QApplication>
#include <QTextEdit>

#include "MainWindow.h"
#include "ParamDialog.h"

#include "sortdialog.h"
#include "Tetrahedron.h"
#include "SimpleView.h"

using namespace std;

int main(int argc, char *argv[])
{
	//if (!QGLFormat::hasOpenGL()) {
	//cerr << "This system has no OpenGL support" << endl;
	//return 1;
	//}

	//TEST WITH DOCK WIDGETS!!!!

	QApplication app(argc, argv);

//	MainWindow *mainWin = new MainWindow;
//	mainWin->show();


    //Ui::GoToCellDialog ui;
  //  QDialog *dialog = new ParamDialog;
    //ui.setupUi(dialog);
  //  dialog->show();


//    SortDialog *dialog2 = new SortDialog;
    //dialog->setColumnRange(’C’, ’F’);
//    dialog2->show();

	SimpleView mySimpleView;
	mySimpleView.show();

	//Tetrahedron tetrahedron;
	//tetrahedron.setWindowTitle(QObject::tr("Tetrahedron"));
	//tetrahedron.resize(300, 300);
	//tetrahedron.show();
	//QTextEdit textEdit;
	//textEdit.show();
	return app.exec();


}
