import numpy as np
import pandas as pd
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt
#arr=np.loadtxt("thread.csv",delimiter=",",dtype=str)
#display
datat=pd.read_csv('thread.csv')
dataf=pd.read_csv('fast.csv')
datas=pd.read_csv('sequential.csv')
Effic=[{"nw","Effc"}]
dataEffict=pd.DataFrame([],columns=['nw','Effc'])
dataEfficf=pd.DataFrame([],columns=['nw','Effc'])

Sequentime=datas.usec[0]

for index,row in datat.iterrows():
    time=Sequentime/row["nw"]
    Effc=time/row['usec']
    newRow=pd.DataFrame({'nw':row["nw"],'Effc':Effc},index=[0])
    #dataScal=dataScal.append(newRow,ignore_index=True)
    dataEffict = pd.concat([newRow,dataEffict.loc[:]]).reset_index(drop=True)

for index,row in dataf.iterrows():
    time=Sequentime/row["nw"]
    Effc=time/row['usec']
    newRow=pd.DataFrame({'nw':row["nw"],'Effc':Effc},index=[0])
    #dataScal=dataScal.append(newRow,ignore_index=True)
    dataEfficf = pd.concat([newRow,dataEfficf.loc[:]]).reset_index(drop=True)



xt=dataEffict.nw
yt=dataEffict.Effc

xf=dataEfficf.nw
yf=dataEfficf.Effc





plt.figure(figsize=(20,10))
plt.xlabel("number of workers")
plt.ylabel("Efficiency(n)")
plt.plot(xt,yt,color="green",marker="o",label='Thread')
plt.plot(xf,yf,color="red",marker="*",label='Fast')
#plt.plot(x,x+0,color="red",linestyle='dashed')
plt.plot()



plt.legend()

plt.savefig("threadEff.png")

