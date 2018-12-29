#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Nov 29 16:32:54 2018

@author: quentin
"""

x = []
y = []
t = []

n = 13

with open("/home/quentin/Bureau/KIRO/KIRO/Data/grenoble/nodes.csv") as nodes:
    lines = list(map(str.rstrip, nodes.readlines()))
    for i in range(1,n+1):
        metadatas = lines[i].split(';')
        x.append(float(metadatas[0]))
        y.append(float(metadatas[1]))
        t.append(metadatas[2])
    
with open("/home/quentin/Bureau/KIRO/KIRO/Data/grenoble/nodes0.csv",'w') as nodes0:
    for i in range(0,n):
        nodes0.write('%.12f %.12f %s \n' % (x[i],y[i],t[i]))

    
