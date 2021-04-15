#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Visualise it with Paraview

@author: Chen Yongxin
"""

from paraview import vtr
import numpy as np

x = np.loadtxt("x");
y = np.loadtxt("y");
z = np.loadtxt("z");

nx, ny, nz = x.size, y.size, z.size

fname = "data"
data = np.loadtxt(fname)
data = np.reshape(data, (nx, ny, nz), order='F')

d = np.zeros((1, nx, ny, nz))
d[0,:,:,:] = data 

fields = {"Distance":d}
vtr("mystl", x, y, z, **fields)