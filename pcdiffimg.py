from paraview.simple import *

import math
import time
import os
import sys

#Nt     = 32
Nt     = 1
var_in = ["Bz","Jx","rho","Ux"]
pcdir  = sys.argv[1]

def mySlice(do_var, file_in):
  aa = []
  for i in range (0,Nt):
    aa.append(file_in + "/pcdiff%d.vtk" % (i))

  reader = servermanager.sources.LegacyVTKReader(FileNames=aa)
  view = servermanager.CreateRenderView()

  #CREATE THE SLICE FOR DISPLAY
  sliceFilter = servermanager.filters.Slice(Input=reader)
  sliceFilter.SliceType = "Plane"
  sliceFilter.SliceType.Origin = [0,0,0]
  sliceFilter.SliceType.Normal= [0,1,0]

  if do_var == "Jx":
    sliceFilter.SliceType.Normal= [0,0,1]

  repre = servermanager.CreateRepresentation(sliceFilter,view)
  repre.Specular = 1.0
  repre.ColorArrayName = do_var
#  repre.ColorAttributeType = 'POINT_DATA'
  repre.CubeAxesVisibility = 1
  repre.CubeAxesColor = [0,0,0]

  repre.CubeAxesXAxisVisibility = 1
  repre.CubeAxesXAxisTickVisibility = 1
  repre.CubeAxesXAxisMinorTickVisibility = 1
#  repre.CubeAxesYTitle = "$Y (R_E)$"
  repre.CubeAxesYTitle = ""
  repre.CubeAxesYAxisVisibility = 1
  repre.CubeAxesYAxisTickVisibility = 1
  repre.CubeAxesYAxisMinorTickVisibility = 0
#  repre.CubeAxesZTitle = "$Z (R_E)$"
  repre.CubeAxesZTitle = ""
  repre.CubeAxesZAxisVisibility = 1
  repre.CubeAxesZAxisTickVisibility = 1
  repre.CubeAxesZAxisMinorTickVisibility = 0

  repre.CubeAxesXTitle = "" 
#    repre.CubeAxesXTitle = "\n\n\n\nTime: %d min" % (i*5)

  repre.LookupTable = GetLookupTableForArray( do_var, 1)
  repre.LookupTable.VectorMode = 'Magnitude'

  bar = servermanager.rendering.ScalarBarWidgetRepresentation()

  bar.RangeLabelFormat = ''

  # SET THE COLORBARS SPECIFICALLY FOR CERTAIN VARIABLES
  repre.LookupTable.ColorSpace = 'Diverging'
#  print repre.LookupTable.RGBPoints
#  repre.LookupTable.RGBPoints = [-100,0,0,1,100,1,0,0]
  repre.LookupTable.RGBPoints = [-100.01,0,0,1, 0,1,1,1, 100.01,1,0,0]
#  repre.LookupTable.RGBPoints[0] = -100
#  repre.LookupTable.RGBPoints[4] = 0
#  repre.LookupTable.RGBPoints[4] = 100
  repre.LookupTable.NumberOfTableValues = 20

  if do_var == "Bz":
    bar.Title = r"$\Delta B_z/\bar{B}_z$"
  elif do_var == "Jx":
    bar.Title = r"$\Delta J_x/\bar{J}_x$"
  elif do_var == "rho":
    bar.Title = r"$\Delta \rho/\bar{\rho}$"
  elif do_var == "Ux":
    bar.Title = r"$\Delta U_x/\bar{U}_x$"

  bar.ComponentTitle = ''
  bar.NumberOfLabels = 13
  bar.TitleFontSize=7
  bar.LabelFontSize=7
  bar.TitleColor = [0.0, 0.0, 0.0]
  bar.LabelColor = [0.0, 0.0, 0.0]
  bar.LookupTable=repre.LookupTable
  bar.Position=[0.88, 0.31]

  text = Text()
  text.Text = r"$X (R_E)$"
  textDisplay = Show(text,view)
  textDisplay.Position = [0.43,0.23]
  textDisplay.Color = [0,0,0]
  textDisplay.FontSize = 7

  text2 = Text()
  text2.Text = r"$Z (R_E)$"
  text2Display = Show(text2,view)
  text2Display.Position = [0.0,0.50]
  text2Display.Color = [0,0,0]
  text2Display.FontSize = 7

  view.Representations.append(bar)
  view.Background = [1.0, 1.0, 1.0]
  view.OrientationAxesVisibility=0
  view.CenterAxesVisibility=0
  view.ViewSize = [1024,768]

  view.CameraViewUp = [0.0,0.0,1.0]
  #view.CameraFocalPoint = [-82.51589842138664,0,0]
  #view.CameraPosition = [-82.51589842138664, -468.91120534394526, 0]
  view.CameraFocalPoint = [-82,0,0]
  view.CameraPosition = [-82, -468, 0]
#  view.CameraViewAngle = 20.0
  if do_var == "Jx" or do_var == "Jx_Diff":
    view.CameraViewUp = [0.0,1.0,0.0]
    #view.CameraFocalPoint = [-82.51589842138664,0,0]
    #view.CameraPosition = [-82.51589842138664,  0, 468.91120534394526]
    view.CameraFocalPoint = [-82,0,0]
    view.CameraPosition = [-82,  0, 468]
#   view.UseOffscreenRendering

  for i in range(0,Nt):
    text3 = Text()
    text3.Text = "Time: %d min" % (i*5)
    text3Display = Show(text3,view)
    text3Display.Position = [0.45,0.70]
    text3Display.Color = [0,0,0]
    text3Display.FontSize = 7

    view.ViewTime = i
    file_png = file_in + "/pcdiff_"+do_var+"_%d.png" % (i)
    view.WriteImage(file_png,'vtkPNGWriter')
    print "Wrote "+file_png
    file_pdf = file_in + "/pcdiff_"+do_var+"_%d.pdf" % (i)
    ExportView(file_pdf,view=view)
    print "Wrote "+file_pdf

for do_var in var_in:
  mySlice(do_var,pcdir)
