from paraview.simple import *

import math
import time
import os

Nt     = 3
base   = "data/Brian_Curtis_04"
var_in = ["Bz","Jx","rho","Ux"]

#folders = ["2213_1","2213_2","2213_3","2213_5","2213_6","2213_7","2413_1","2413_2","2413_3","2413_5","2413_6","2413_7","102114_1","102114_2","102114_3"]
folders = ["2213_1"]

def mySlice(do_var, files, dir_png):

  reader = servermanager.sources.LegacyVTKReader(FileNames=files)
  view   = servermanager.CreateRenderView()

  # CREATE THE SLICE FOR DISPLAY
  sliceFilter                  = servermanager.filters.Slice(Input=reader)
  sliceFilter.SliceType        = "Plane"
  sliceFilter.SliceType.Origin = [0,0,0]
  sliceFilter.SliceType.Normal = [0,1,0]

  if do_var == "Jx" or do_var == "Jx_Diff":
    sliceFilter.SliceType.Normal= [0,0,1]

  repre = servermanager.CreateRepresentation(sliceFilter,view)

  for i in range(0,Nt):
    reader.UpdatePipeline(i)
    datainfo      = reader.GetDataInformation()
    pointDataInfo = datainfo.GetPointDataInformation()
    arrayInfo     = pointDataInfo.GetArrayInformation(do_var)
    if arrayInfo:
      datarange = arrayInfo.GetComponentRange(-1)
      print "Iteration ",i,": ",datarange

  repre.ColorArrayName     = do_var
  repre.ColorAttributeType = 'POINT_DATA'
  repre.CubeAxesVisibility = 1
  repre.CubeAxesColor      = [0,0,0]

  repre.CubeAxesXAxisVisibility = 1
  repre.CubeAxesXAxisTickVisibility = 1
  repre.CubeAxesXAxisMinorTickVisibility = 1
  repre.CubeAxesYTitle = "$Y (R_E)$"
  repre.CubeAxesYAxisVisibility = 1
  repre.CubeAxesYAxisTickVisibility = 1
  repre.CubeAxesYAxisMinorTickVisibility = 0
  repre.CubeAxesZTitle = "$Z (R_E)$"
  repre.CubeAxesZAxisVisibility = 1
  repre.CubeAxesZAxisTickVisibility = 1
  repre.CubeAxesZAxisMinorTickVisibility = 0

  repre.LookupTable = GetLookupTableForArray( do_var, 1)
  #repre.LookupTable.ColorSpace = 'Diverging'
  repre.LookupTable.VectorMode = 'Magnitude'
  #repre.LookupTable.ScalarRangeInitialized = 1.0
  #repre.LookupTable.LockScalarRange = 1
  #repre.LookupTable.AllowDuplicateScalars = 1
    
  bar = servermanager.rendering.ScalarBarWidgetRepresentation()

  # SET THE COLORBARS SPECIFICALLY FOR CERTAIN VARIABLES
  repre.LookupTable.ColorSpace = 'Diverging'
  repre.LookupTable.RGBPoints[0] = -100
  repre.LookupTable.RGBPoints[4] = 100
  repre.LookupTable.NumberOfTableValues = 20
  if do_var == "Bz":
    repre.LookupTable.RGBPoints[0] = -50
    repre.LookupTable.RGBPoints[4] = 50
    repre.LookupTable.NumberOfTableValues = 25
    bar.Title = "$B_z (nT)$"
  elif do_var == "Jx":
    repre.LookupTable.RGBPoints[0] = -.002
    repre.LookupTable.RGBPoints[4] = .002
    repre.LookupTable.NumberOfTableValues = 20
    bar.Title = "$J_x ({\mu A}/{m^2})$"
  elif do_var == "rho":
    repre.LookupTable.ColorSpace = 'HSV'
    repre.LookupTable.RGBPoints[0] = 0
    repre.LookupTable.RGBPoints[4] = 20
    repre.LookupTable.NumberOfTableValues = 20
    bar.Title = "$\rho (cm^{-3})$"
  elif do_var == "Ux":
    repre.LookupTable.RGBPoints[0] = -400
    repre.LookupTable.RGBPoints[4] = 400
    repre.LookupTable.NumberOfTableValues = 20
    bar.Title = "$U_x km/s$"
  
  bar.TitleFontSize = 7
  bar.LabelFontSize = 7
  bar.TitleColor  = [0.0, 0.0, 0.0]
  bar.LabelColor  = [0.0, 0.0, 0.0]
  bar.LookupTable = repre.LookupTable
  bar.Position    = [0.88, 0.275]

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

  if do_var == "Jx" or do_var == "Jx_Diff":
    view.CameraViewUp = [0.0,1.0,0.0]
    #view.CameraFocalPoint = [-82.51589842138664,0,0]
    #view.CameraPosition = [-82.51589842138664,  0, 468.91120534394526]
    view.CameraFocalPoint = [-82,0,0]
    view.CameraPosition = [-82,  0, 468]
    view.UseOffscreenRendering

  for i in range(0,Nt):# get active view
    view.ViewTime = i
    repre.CubeAxesXTitle = "Time %dmin \n\n    $X (R_E)$" % (i*5)
    file_png = dir_png+do_var+"_File%d.png" % (i)
    view.WriteImage(file_png,'vtkPNGWriter')
    print "Wrote "+file_png

for folder in folders:

  dir_vtk = base+folder+"/Results/"
  dir_png = dir_vtk + "/images/"
  print "Processing directory: ",dir_vtk
  files_vtk = []
  for i in range (0,Nt):
    files_vtk.append(dir_vtk + "Result%d.vtk" % (i))

  for do_var in var_in:
    mySlice(do_var,files_vtk,dir_png)
