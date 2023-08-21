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
plt.plot(xt,yt,color="black",marker="o",label='Thread')
plt.plot(xf,yf,color="black",marker="*",label='Fast Flow',linestyle='dashed')
plt.plot(xs,ys,color="black",marker="+",label='Sequential')
plt.legend()

plt.savefig("threadgraph.png")

#EFFICIENTCY GRAPH
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
plt.plot(xt,yt,color="black",marker="o",label='Thread')
plt.plot(xf,yf,color="black",marker="*",label='Fast',linestyle='dashed')
#plt.plot(x,x+0,color="red",linestyle='dashed')
plt.plot()



plt.legend()

plt.savefig("threadEff.png")

#SCALABILITY GRAPH

Scal=[{"nw","Scalability"}]

dataScalt=pd.DataFrame([],columns=['nw','Scalab'])
dataScalf=pd.DataFrame([],columns=['nw','Scalab'])

par1T=datat.usec[0]
par1F=dataf.usec[0]
xt =[]
yt=[]
xt.append(0)
yt.append(0)
xf=[]
yf=[]
xf.append(0)
yf.append(0)
for index,row in datat.iterrows():
    Scalab=par1T/row["usec"]
    xt.append(row["nw"])
    yt.append(Scalab)
    


for index,row in dataf.iterrows():
    Scalab=par1T/row["usec"]
    xf.append(row["nw"])
    yf.append(Scalab)






fig = plt.figure()
ax = plt.axes()

plt.figure(figsize=(20,10))
plt.xlabel("number of workers")
plt.ylabel("Scalability(n)")
values=range(len(xt))

plt.plot(values,yt,color="black",marker="o",label='Thread')
plt.plot(values,yf,color="black",marker="*",label='Fast',linestyle='dashed')

plt.xticks(values,xt)
plt.ylim(0,7)

plt.plot()


plt.legend()

plt.savefig("threadScal.png")


#SPEED UP GRAPH

Scal=[{"nw","Speed"}]
dataSpeedt=pd.DataFrame([],columns=['nw','Speed'])
dataSpeedf=pd.DataFrame([],columns=['nw','Speed'])

Sequentime=datas.usec[0]
xt =[]
yt=[]
xt.append(0)
yt.append(0)
xf=[]
yf=[]
xf.append(0)
yf.append(0)
for index,row in datat.iterrows():
    Speed=Sequentime/row["usec"]
    xt.append(row["nw"])
    yt.append(Speed)
   
    
for index,row in dataf.iterrows():
    Speed=Sequentime/row["usec"]
    newRow=pd.DataFrame({'nw':row["nw"],'Speed':Speed},index=[0])
    xf.append(row["nw"])
    yf.append(Speed)

   




print(xt)
print(yt)
id=yt
values=range(len(xt))
print(values)
plt.figure(figsize=(20,10))
plt.xlabel("number of workers")
plt.ylabel("SpeedUp(n)")




plt.plot(values,yt,color="black",marker="o",label='Thread')
plt.plot(values,yf,color="black",marker="*",label='Fast',linestyle='dashed')
plt.xticks(values,xt)
plt.ylim(0,7)


plt.plot()



plt.legend()

plt.savefig("threadSpeed.png")




