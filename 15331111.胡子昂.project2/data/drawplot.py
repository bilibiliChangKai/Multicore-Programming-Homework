#!encoding=utf-8
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def getAvg(dfs):
    #rtndf = pd.DataFrame(index=dfs[0].index, columns=dfs[0].columns)
    rtndf = dfs[0]
    for df in dfs[1:]:
        #print df
        rtndf = rtndf + df
    rtndf = rtndf / len(dfs)
    return rtndf

def drawplot(dirname, columnname, begins=None, ends=None):
    # 读数据
    dfs = [] 
    for i in range(10): 
        dfs.append(pd.read_csv(dirname + "/B_" + str(i) + ".csv"))
    if begins != None and ends != None:
        dfB = getAvg(dfs)[begins[0]:ends[0]]
    else:
        dfB = getAvg(dfs)
    dfs = [] 
    for i in range(10): 
        dfs.append(pd.read_csv(dirname + "/KB_" + str(i) + ".csv"))
    if begins != None and ends != None:
        dfKB = getAvg(dfs)[begins[1]:ends[1]]
    else:
        dfKB = getAvg(dfs)
    dfs = [] 
    for i in range(10): 
        dfs.append(pd.read_csv(dirname + "/MB_" + str(i) + ".csv"))
    if begins != None and ends != None:
        dfMB = getAvg(dfs)[begins[2]:ends[2]]
    else:
        dfMB = getAvg(dfs)
        
    # 画图
    plt.figure(31)
    plt.subplot(311)
    plt.plot(dfB["B"], dfB[columnname])
    plt.subplot(312)
    plt.plot(dfKB["KB"], dfKB[columnname])
    plt.subplot(313)
    plt.plot(dfMB["MB"], dfMB[columnname])
    plt.show()

drawplot("readall", "Read Time (ns)", begins=[0, 0, 0], ends=[2048, 2048, 128])
drawplot("read1024", "Read Time (ns)", begins=[0, 0, 0], ends=[2048, 2048, 2048])
drawplot("readl", "Read Time (ns)", begins=[0, 0, 0], ends=[2048, 2048, 2048])
drawplot("read1024", "Read Time (ns)", begins=[1500, 0, 0], ends=[1600, 2048, 30])