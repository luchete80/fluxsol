#include "Tetrahedron.h"
using namespace std;
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	if (!QGLFormat::hasOpenGL()) {
	cerr << "This system has no OpenGL support" << endl;
	return 1;
	}
	Tetrahedron tetrahedron;
	tetrahedron.setWindowTitle(QObject::tr("Tetrahedron"));
	tetrahedron.resize(300, 300);
	tetrahedron.show();
	return app.exec();
}