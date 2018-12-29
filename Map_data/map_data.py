#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt 

fichier = open("node_grenoble.txt", "r")
node = fichier.read()
fichier.close()

node = node.split("\n")
# ne marche peut etre pas pour grenoble et nice
del node[len(node)-1]
del node[len(node)-1]

for i in range(len(node)):
    node[i] = node[i].split(";")
    
X = [float(node[i][0]) for i in range(len(node))]
Y = [float(node[i][1]) for i in range(len(node))]
types = [node[i][2] for i in range(len(node))]
labels = [i for i in range(len(node))]
colors = []
for i in range(len(node)):
    if (types[i] == "distribution"):
        colors.append('yellow')
    else:
        colors.append('grey')



fig, ax = plt.subplots()
ax.scatter(X, Y, color=colors)

for i, txt in enumerate(labels):
    ax.annotate(txt, (X[i], Y[i]))
    
    
    
fichier2 = open("2112_grenoble.txt", "r")  #"26830_RS_Paris.txt"   "6703_nice.txt"    "25892_RS_cc_Paris.txt"   "6608_RS_cc_nice.txt"  "7100_nice.txt"
solution = fichier2.read()
fichier2.close()

solution = solution.split("\n")
del solution[len(solution)-1]
for i in range(len(solution)):
    solution[i] = solution[i].split(" ")
    
for i in range(len(solution)):
    if (solution[i][0] == "b"):
        for j in range(1, len(solution[i])-1):
            plt.plot([X[int(solution[i][j])], X[int(solution[i][j+1])]], [Y[int(solution[i][j])], Y[int(solution[i][j+1])]], c="y")
        plt.plot([X[int(solution[i][len(solution[i])-1])], X[int(solution[i][1])]], [Y[int(solution[i][len(solution[i])-1])], Y[int(solution[i][1])]], c="y")
    if (solution[i][0] == "c"):
        for j in range(1, len(solution[i])-1):
            plt.plot([X[int(solution[i][j])], X[int(solution[i][j+1])]], [Y[int(solution[i][j])], Y[int(solution[i][j+1])]], c="r")
    


plt.show()

    
