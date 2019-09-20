#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Nov 29 16:32:54 2018

@author: quentin
"""

def create_node0(n, path_node, path_node0): 
    with open(path_node) as nodes:
        x = []
        y = []
        t = []
        lines = list(map(str.rstrip, nodes.readlines()))
        for i in range(1,n+1):
            metadatas = lines[i].split(';')
            x.append(float(metadatas[0]))
            y.append(float(metadatas[1]))
            t.append(metadatas[2])

    with open(path_node0,'w') as nodes0:
        for i in range(0,n):
            nodes0.write('%.12f %.12f %s \n' % (x[i],y[i],t[i]))
     
    
n = 13
path_node = "Data/grenoble/nodes.csv"
path_node0 = "Data/grenoble/nodes0.csv"
create_node0(n, path_node, path_node0)
