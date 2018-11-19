#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Nov  6 18:55:29 2018

@author: weigel
"""
import numpy as np
from matplotlib import pyplot as plt

#table = np.genfromtxt('points.txt', dtype=None, delimiter=',', encoding='utf-8')
table = np.genfromtxt('points_00.txt', dtype=None, delimiter=',', encoding='utf-8')

x = table[:,0]
y = table[:,1]                                                                                                          
z = table[:,2]
Ro = table[:,3]
thetaod = table[:,4]
phiod = table[:,5]
R = np.sqrt(x*x+y*y+z*z)
r = np.sqrt(x*x+y*y)
#theta = np.arctan2(r,z)
#phi = np.arctan2(x,y)
#phi[np.logical_and(x==0,y==0)] = 0.0
thetad = 180.0*theta/np.pi
phid = 180.0*phi/np.pi

varnames = ['x','y','z','R',r'\theta',r'\phi','B_x','B_y','B_z','J_x','J_y','J_z','U_x','U_y','U_z','P','\rho']
vari = 7
var = table[:,vari]
phi = np.unique(phiod)
theta = np.unique(thetaod)
var = np.reshape(var,(len(phi),len(theta)))
#plt.plot(thetaod,phiod,'.')
print(var)
plt.rc('font', size=16)
plt.pcolor(phi,theta,np.transpose(var))
plt.colorbar(label="$"+varnames[vari]+"$")
plt.xlabel(r'$\phi$')
plt.ylabel(r'$\theta$')
