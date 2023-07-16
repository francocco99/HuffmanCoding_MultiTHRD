import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
#arr=np.loadtxt("thread.csv",delimiter=",",dtype=str)
#display
datat=pd.read_csv('thread.csv')
dataf=pd.read_csv('fast.csv')
datas=pd.read_csv('sequential.csv')

xt=datat.nw
yt=datat.usec

xf=dataf.nw
yf=dataf.usec

xs=datas.nw
ys=datas.usec



plt.xlabel("number of workers")
plt.ylabel("usecs used")
plt.plot(xt,yt,color="green",marker="o")
plt.plot(xf,yf,color="red",marker="*")

plt.savefig("threadgraph.png")