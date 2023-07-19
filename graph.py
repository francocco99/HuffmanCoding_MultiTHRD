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


plt.figure(figsize=(10,10))
plt.xlabel("number of workers")
plt.ylabel("usecs used")
plt.plot(xt,yt,color="green",marker="o",label='Thread')
plt.plot(xf,yf,color="red",marker="*",label='Fast Flow')
plt.plot(xs,ys,color="blue",marker="*",label='Sequential')
plt.legend()

plt.savefig("threadgraph.png")