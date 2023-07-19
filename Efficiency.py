import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
#arr=np.loadtxt("thread.csv",delimiter=",",dtype=str)
#display
datat=pd.read_csv('thread.csv')
dataf=pd.read_csv('fast.csv')
datas=pd.read_csv('sequential.csv')
Scal=[{"nw","Speed"}]
dataSpeed=pd.DataFrame([],columns=['nw','Speed'])

Sequentime=datas.usec[0]

for index,row in datat.iterrows():
    Speed=Sequentime/row["usec"]
    newRow=pd.DataFrame({'nw':row["nw"],'Speed':Speed},index=[0])
    #dataScal=dataScal.append(newRow,ignore_index=True)
    dataSpeed = pd.concat([newRow,dataSpeed.loc[:]]).reset_index(drop=True)
print(dataSpeed)


xt=dataSpeed.nw
yt=dataSpeed.Speed





plt.figure(figsize=(20,10))
plt.xlabel("number of workers")
plt.ylabel("SpeedUp(n)")


plt.plot(xt,yt,color="green",marker="o",label='Thread')
#plt.plot(x,x+0,color="red",linestyle='dashed')
plt.plot()



plt.legend()

plt.savefig("threadSpeed.png")

