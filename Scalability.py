import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
#arr=np.loadtxt("thread.csv",delimiter=",",dtype=str)
#display
datat=pd.read_csv('thread.csv')
dataf=pd.read_csv('fast.csv')
Scal=[{"nw","Scalability"}]

dataScalt=pd.DataFrame([],columns=['nw','Scalab'])
dataScalf=pd.DataFrame([],columns=['nw','Scalab'])

par1T=datat.usec[0]
par1F=dataf.usec[0]

for index,row in datat.iterrows():
    Scalab=par1T/row["usec"]
    newRow=pd.DataFrame({'nw':row["nw"],'Scalab':Scalab},index=[0])
    #dataScal=dataScal.append(newRow,ignore_index=True)
    dataScalt = pd.concat([newRow,dataScalt.loc[:]]).reset_index(drop=True)


for index,row in dataf.iterrows():
    Scalab=par1T/row["usec"]
    newRow=pd.DataFrame({'nw':row["nw"],'Scalab':Scalab},index=[0])
    #dataScal=dataScal.append(newRow,ignore_index=True)
    dataScalf = pd.concat([newRow,dataScalf.loc[:]]).reset_index(drop=True)



xt=dataScalt.nw
yt=dataScalt.Scalab


xf=dataScalf.nw
yf=dataScalf.Scalab



fig = plt.figure()
ax = plt.axes()

plt.figure(figsize=(20,10))
plt.xlabel("number of workers")
plt.ylabel("Scalability(n)")
plt.plot(xt,yt,color="green",marker="o",label='Thread')
plt.plot(xf,yf,color="red",marker="*",label='Fast')
#plt.plot(x,x+0,color="red",linestyle='dashed')
plt.plot()


plt.legend()

plt.savefig("threadScal.png")

