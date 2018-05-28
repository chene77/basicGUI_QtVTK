/*=========================================================================

Program:   basic_qtVTK
Module:    $RCSfile: mainWindows.h,v $
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


#ifndef __MAINWIDGET_H__
#define __MAINWIDGET_H__

#pragma once

#include <vtkSmartPointer.h>
#include <QMainWindow>
#include "ui_basic_QtVTK.h"

// VTK forward declaration
class vtkActor;
class vtkGenericOpenGLRenderWindow;
class vtkRenderer;

class basic_QtVTK : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT

public:
  // constructor/destructor
  basic_QtVTK();
  ~basic_QtVTK() {};

public slots:

  virtual void slotExit();

  void loadMesh();
  void editMeshColor();
  void editRendererBackgroundColor();
  void screenShot();
  
public:
  // centralized location to initiate all vtk objects
  void createVTKObjects();

  // connect VTK pipelines
  void setupVTKObjects();

  // connect Qt pipelines (slots/signals/actions)
  void setupQTObjects();

  // clean up
  void cleanVTKObjects();

private:

  //vtkNew<vtkRenderer> ren;
  vtkSmartPointer<vtkActor>                       actor;
  vtkSmartPointer<vtkRenderer>                    ren;
  vtkSmartPointer<vtkGenericOpenGLRenderWindow>   renWin;

  int                                             screenShotFileNumber;

};

#endif // of __MAINWIDGET_H__