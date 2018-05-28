/*=========================================================================

Program:   basic_qtVTK
Module:    $RCSfile: mainWindows.cxx,v $
Creator:   Elvis C. S. Chen <chene@robarts.ca>
Language:  C++
Author:    $Author: Elvis Chen $
Date:      $Date: 2018/05/24 15:45:30 $
Version:   $Revision: 0.99 $

==========================================================================

Copyright (c) Elvis C. S. Chen, elvis.chen@gmail.com

Use, modification and redistribution of the software, in source or
binary forms, are permitted provided that the following terms and
conditions are met:

1) Redistribution of the source code, in verbatim or modified
form, must retain the above copyright notice, this license,
the following disclaimer, and any notices that refer to this
license and/or the following disclaimer.

2) Redistribution in binary form must include the above copyright
notice, a copy of this license and the following disclaimer
in the documentation or with other materials provided with the
distribution.

3) Modified copies of the source code must be clearly marked as such,
and must not be misrepresented as verbatim copies of the source code.

THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE SOFTWARE "AS IS"
WITHOUT EXPRESSED OR IMPLIED WARRANTY INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  IN NO EVENT SHALL ANY COPYRIGHT HOLDER OR OTHER PARTY WHO MAY
MODIFY AND/OR REDISTRIBUTE THE SOFTWARE UNDER THE TERMS OF THIS LICENSE
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, LOSS OF DATA OR DATA BECOMING INACCURATE
OR LOSS OF PROFIT OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF
THE USE OR INABILITY TO USE THE SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

=========================================================================*/

// local includes
#include "mainWindows.h"

// VTK includes
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageExtractComponents.h>
#include <vtkNew.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPNGWriter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkWindowToImageFilter.h>
#include <vtkXMLPolyDataReader.h>

// QT includes
#include <QColorDialog>
#include <QDebug>
#include <QErrorMessage>
#include <QFileDialog>
#include <QMessageBox>


template< class PReader > vtkPolyData *readAnPolyData(const char *fname) {
  vtkSmartPointer< PReader > reader =
    vtkSmartPointer< PReader >::New();
  reader->SetFileName(fname);
  reader->Update();
  reader->GetOutput()->Register(reader);
  return(vtkPolyData::SafeDownCast(reader->GetOutput()));
  }

basic_QtVTK::basic_QtVTK()
{
  this->setupUi(this);

  createVTKObjects();
  setupVTKObjects();
  setupQTObjects();
}


void basic_QtVTK::createVTKObjects()
{
  actor     = vtkSmartPointer<vtkActor>::New();
  ren       = vtkSmartPointer<vtkRenderer>::New();
  renWin    = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
}


void basic_QtVTK::cleanVTKObjects()
{
  // if needed
}


void basic_QtVTK::setupVTKObjects()
{
  // number of screenshot
  screenShotFileNumber = 0;

  this->openGLWidget->SetRenderWindow(renWin);

  // VTK Renderer
  ren->SetBackground(.1, .2, .4);

  // connect VTK with Qt
  this->openGLWidget->GetRenderWindow()->AddRenderer(ren);
}


void basic_QtVTK::setupQTObjects()
{
  connect(action_Quit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(actionLoad_Mesh, SIGNAL(triggered()), this, SLOT(loadMesh()));
  connect(actionMesh_Color, SIGNAL(triggered()), this, SLOT(editMeshColor()));
  connect(action_Background_Color, SIGNAL(triggered()), this, SLOT(editRendererBackgroundColor()));
  connect(actionScreen_Shot, SIGNAL(triggered()), this, SLOT(screenShot()));
  connect(actionthis_program, SIGNAL(triggered()), this, SLOT(aboutThisProgram()));
}


void basic_QtVTK::loadMesh()
  {
  QString fname = QFileDialog::getOpenFileName(this,
    tr("Open phantom mesh"),
    QDir::currentPath(),
    "PolyData File (*.vtk *.stl *.ply *.obj *.vtp)");

  // std::cerr << fname.toStdString().c_str() << std::endl;

  vtkPolyData *data;

  QFileInfo info(fname);
  bool knownFileType = true;
  if (info.suffix() == QString(tr("vtk")))
    {
    data = readAnPolyData<vtkPolyDataReader >(fname.toStdString().c_str());
    }
  else if (info.suffix() == QString(tr("stl")) || info.suffix() == QString(tr("stlb")))
    {
    data = readAnPolyData<vtkSTLReader>(fname.toStdString().c_str());
    }
  else if (info.suffix() == QString(tr("ply")))
    {
    data = readAnPolyData<vtkPLYReader>(fname.toStdString().c_str());
    }
  else if (info.suffix() == QString(tr("obj")))
    {
    data = readAnPolyData<vtkOBJReader>(fname.toStdString().c_str());
    }
  else if (info.suffix() == QString(tr("vtp")))
    {
    data = readAnPolyData<vtkXMLPolyDataReader>(fname.toStdString().c_str());
    }
  else
    {
    knownFileType = false;
    }

  if (knownFileType)
    {
    // do something only if we know the file type
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(data);

    actor->SetMapper(mapper);
    ren->AddActor(actor);

    // reset the camera according to visible actors
    ren->ResetCamera();
    ren->ResetCameraClippingRange();
    this->openGLWidget->GetRenderWindow()->Render();
    }
  else
    {
    QErrorMessage *em = new QErrorMessage(this);
    em->showMessage("Input file format not supported");
    }
}

void basic_QtVTK::screenShot()
{
  // output the screen to PNG files.
  //
  // the file names are 0.png, 1.png, ..., etc.
  //
  QString fname = QString::number(screenShotFileNumber) + QString(tr(".png"));
  screenShotFileNumber++;

  vtkNew<vtkWindowToImageFilter> w2i;
  w2i->SetInput(this->openGLWidget->GetRenderWindow());
  w2i->ReadFrontBufferOff();
  w2i->SetInputBufferTypeToRGBA();

  vtkNew<vtkImageExtractComponents> iec;
  iec->SetInputConnection(w2i->GetOutputPort());
  iec->SetComponents(0, 1, 2);

  vtkNew<vtkPNGWriter> writer;
  writer->SetFileName(fname.toStdString().c_str());
  writer->SetInputConnection(iec->GetOutputPort());
  writer->Write();
}


void basic_QtVTK::editRendererBackgroundColor()
{

  QColor color = QColorDialog::getColor(Qt::gray, this);

  if (color.isValid())
    {
    int r, g, b;
    color.getRgb(&r, &g, &b);
    ren->SetBackground((double)r / 255.0, (double)g / 255.0, (double)b / 255.0);
    this->openGLWidget->GetRenderWindow()->Render();
    }
}


void basic_QtVTK::editMeshColor()
{

  QColor color = QColorDialog::getColor(Qt::gray, this);

  if (color.isValid())
    {
    int r, g, b;
    color.getRgb(&r, &g, &b);
    actor->GetProperty()->SetColor((double)r / 255.0, (double)g / 255.0, (double)b / 255.0);
    this->openGLWidget->GetRenderWindow()->Render();
    }
  
}


void basic_QtVTK::slotExit()
{
  cleanVTKObjects(); // if needed
  qApp->exit();
}



void basic_QtVTK::aboutThisProgram()
{
  QMessageBox::about(this, tr("About basic_QtVTK"),
    tr("This is a demostration for Qt/VTK integration\n\n"
      "By: \n\n"
      "Elvis C.S. Chen\t\t"
      "chene@robarts.ca"));
}
