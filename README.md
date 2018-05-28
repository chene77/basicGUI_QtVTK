# basicGUI_QtVTK

This is a basic, and yet complete, VTK polydata viewer written using the following libraries:
* VTK 9,
* QT 5.10.1

This program was built and tested on 
* Windows 10/Visual Studio 2015 Enterprise with update 3

but it contains no OS-specific function calls. 

The main purpose of this program is to demonstrate how to integrate Qt (GUI) with VTK, with appropriate SIGNAL/SLOT mechanism for user interactions. The main GUI was designed using Qt Designer:
* The base Widget is a QMainWindow, 
  * The centralwidget is a QWidget,
* A QOpenGLWidget was laid on top of the centralwidget,
  * and the QOpenGLWidget was promoted to QVTKOpenGLWidget.
* PolyData file types supported are:
  * [Legacy VTK file format (.VTK)](https://www.vtk.org/doc/nightly/html/classvtkPolyDataReader.html),
  * [ASCII (.STL) or binary (.STLB) Stereo Lithography file format](https://www.vtk.org/doc/nightly/html/classvtkSTLReader.html),
  * [Stanford University (.PLY) polygonal file format](https://www.vtk.org/doc/nightly/html/classvtkPLYReader.html),
  * [Wavefront (.OBJ) file](https://www.vtk.org/doc/nightly/html/classvtkOBJReader.html), and
  * [VTK XML (.VTP) PolyData file](https://www.vtk.org/doc/nightly/html/classvtkXMLPolyDataReader.html).

The following short-cuts are supported:
* Load Mesh (Ctrl+M), 
* Quit (Ctrl+Q),
* Change mesh color (Ctrl+C),
* Change background color (Ctrl+B), and
* Screen shot (Ctrl+S).

The screen shorts are saves sequentially using numbers (0.png, 1.png, ..., etc).
