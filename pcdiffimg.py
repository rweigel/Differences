from paraview.simple import *

import math
import time
import os

def mySlice(do_var, file_in):
  aa = []
  for i in range (0,32):
    aa.append(file_in + "Result%d.vtk" % (i))
  #aa = file_in + "Result%d.vtk" % (i)
  reader = servermanager.sources.LegacyVTKReader( FileNames=aa )
  view = servermanager.CreateRenderView()
  #CREATE THE SLICE FOR DISPLAY
  sliceFilter = servermanager.filters.Slice(Input=reader)
  sliceFilter.SliceType = "Plane"
  sliceFilter.SliceType.Origin = [0,0,0]
  sliceFilter.SliceType.Normal= [0,1,0]
  if do_var == "Jx" or do_var == "Jx_Diff":
    sliceFilter.SliceType.Normal= [0,0,1]
  repre = servermanager.CreateRepresentation(sliceFilter,view)
  for i in range(0,32):
    #view.ViewTime = i
    reader.UpdatePipeline(i)
    datainfo = reader.GetDataInformation()
    pointDataInfo = datainfo.GetPointDataInformation()
    arrayInfo = pointDataInfo.GetArrayInformation(do_var)
    if arrayInfo:
      datarange = arrayInfo.GetComponentRange(-1)
      print "Iteration ",i,": ",datarange
  repre.ColorArrayName = do_var
  repre.ColorAttributeType='POINT_DATA'
  repre.CubeAxesVisibility = 1
  repre.CubeAxesColor = [0,0,0]

  #cc = bb.append('\n    
  #repre.CubeAxesXTitle = "X (Re) \n %d minutes" % (i*5)
  repre.CubeAxesXAxisVisibility = 1
  repre.CubeAxesXAxisTickVisibility = 1
  repre.CubeAxesXAxisMinorTickVisibility = 1
  repre.CubeAxesYTitle = "Y (Re)"
  repre.CubeAxesYAxisVisibility = 1
  repre.CubeAxesYAxisTickVisibility = 1
  repre.CubeAxesYAxisMinorTickVisibility = 0
  repre.CubeAxesZTitle = "Z (Re)"
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
  #LETS SET THE COLORBARS SPECIFICALLY FOR CERTAIN VARIABLES
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
  elif do_var == "B_Diff":
    bar.Title = "$\frac{\Delta B_z}{\bar{B_z}}$"
  elif do_var == "Jx_Diff":
    bar.Title = "$\frac{\Delta J_x}{\bar{J_x}}$"
  elif do_var == "rho_Diff":
    bar.Title = "$\frac{\Delta \rho}{\bar{\rho}}$"
  elif do_var == "Ux_Diff":
    bar.Title = "$\frac{\Delta U_x}{\bar{U_x}}$"
  

  bar.TitleFontSize=7
  bar.LabelFontSize=7
  bar.TitleColor = [0.0, 0.0, 0.0]
  bar.LabelColor = [0.0, 0.0, 0.0]
  bar.LookupTable=repre.LookupTable
  bar.Position=[0.88, 0.275]

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

  for i in range(0,32):
    view.ViewTime = i
    repre.CubeAxesXTitle = "Time %dmin \n\n    X (Re)" % (i*5)
    aa = file_in + "/images/"+do_var+"_File%d.png" % (i)
    #aa = "/home/bcurtis/Desktop/Test/"+do_var+"_File%d.png" % (i)
    view.WriteImage(aa,'vtkPNGWriter')
    print "Wrote Image "+do_var+"_File%d.png" % (i)
    

var_in = ["Bz_Diff","Jx_Diff","rho_Diff","Ux_Diff"]
folders = ["0_3","1_4","2_5"]
for folder in folders:
  file_in = "/mnt/Disk2/Precondition/Results/"+folder+"/"
  print "File-In: ",file_in
  for do_var in var_in:
    mySlice(do_var,file_in)
