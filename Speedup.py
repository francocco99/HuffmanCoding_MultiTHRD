import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
#arr=np.loadtxt("thread.csv",delimiter=",",dtype=str)
#display
datat=pd.read_csv('thread.csv')
dataf=pd.read_csv('fast.csv')
datas=pd.read_csv('sequential.csv')

Scal=[{"nw","Speed"}]
dataSpeedt=pd.DataFrame([],columns=['nw','Speed'])
dataSpeedf=pd.DataFrame([],columns=['nw','Speed'])

Sequentime=datas.usec[0]

for index,row in datat.iterrows():
    Speed=Sequentime/row["usec"]
    newRow=pd.DataFrame({'nw':row["nw"],'Speed':Speed},index=[0])
    #dataScal=dataScal.append(newRow,ignore_index=True)
    dataSpeedt = pd.concat([newRow,dataSpeedt.loc[:]]).reset_index(drop=True)

for index,row in dataf.iterrows():
    Speed=Sequentime/row["usec"]
    newRow=pd.DataFrame({'nw':row["nw"],'Speed':Speed},index=[0])
    #dataScal=dataScal.append(newRow,ignore_index=True)
    dataSpeedf = pd.concat([newRow,dataSpeedf.loc[:]]).reset_index(drop=True)



xt=dataSpeedt.nw
yt=dataSpeedt.Speed


xf=dataSpeedf.nw
yf=dataSpeedf.Speed




plt.figure(figsize=(20,10))
plt.xlabel("number of workers")
plt.ylabel("SpeedUp(n)")


plt.plot(xt,yt,color="green",marker="o",label='Thread')
plt.plot(xf,yf,color="red",marker="*",label='Fast')
#plt.plot(x,x+0,color="red",linestyle='dashed')
plt.plot()



plt.legend()

plt.savefig("threadSpeed.png")

