import ROOT
from ROOT import *
import matplotlib.pyplot as plt
import numpy as np


class RawData:
    def __init__(self,filename):
	self.rd=ROOT.RawData(filename)
##############Get Channel 0################
    def getCh0(self):
	ch0=[]
	size= self.rd.GetCh0().size()
	for i in xrange(0,size):
	    ch0.append(self.rd.GetCh0()[i])
	return ch0
##############Show Trace###################
    def showTrace(self,root=False,num=None):
	if num is not None:
	    self.setEvent(num)
	if root:
	    self.rd.GetTrace().Draw()
	else:
	    ch=self.getCh0()
	    plt.plot(ch)
	    plt.show()
##############Save Trace###################
    def saveTrace(self,name="defaultName.png",num=None,show=False):
	if num is not None:
	    print num
	    self.setEvent(num)
    
	ch=self.getCh0()
	plt.plot(ch)
	plt.title("event "+str(self.getEventNum()))
	plt.xlabel("time ns")
	plt.savefig(name,bbox_inches="tight")
	if show:
	    plt.show()


##############Get Trace###################
    def getTrace(self,num=None):
	if num is not None:
	    self.setEvent(num)
	return  self.rd.GetTrace()

##############Set Event###################
    def setEvent(self,num):
	self.rd.SetEntry(num)
	self.rd.ch0=[]
 
##############Get Event###################
    def getEventNum(self):
	return int(self.rd.GetEntry())
###############Get Channel Size#####
    def getChSize(self):
	return int(self.rd.GetCh0().size())
##################Get Total Event#########
    def getTotalEvents(self):
	return self.rd.GetTotalEvents()
