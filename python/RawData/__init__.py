import ROOT
from ROOT import gROOT
from RawData import RawData
gROOT.ProcessLine(".L /home/mage/Local/lib/libRawData.so")
gROOT.ProcessLine(".L /home/mage/Local/include/RawData.h")
__all__=['RawData']
