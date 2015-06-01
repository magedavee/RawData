RawDate
=========

	This is a library that is designed for easy interfacing with raw data root files.
Depenedecies 
==========
	Root
	Cmake
	
Building the C++ libray
=========== 

	make a build directory using 
		mkdir /path_to_build/RawData-build
	change to build directory 
		cd /path_to_build/RawData-buil
	run cmake and supply it with the CMakeList.txt location
		cmake /path_to_this_directory/.
	run make, adding the -jN option with tell it to use more cores for faster complitation.
		make -j4
	you should be able to see libRawData.so(on linux and mac,.dll on windows) libPmtData.so
	libRawData_static.a and libPmtRata_static.a.
	
	you can check with 
		ls

	if you want an example built with the code uncoment the two lines in CMakeList.txt by deleting the # in the front
		#add_executable(example example.cpp) 
		#target_link_libraries(example ${ROOT_LIBRARIES} RawData)

	you will need to add Root file made from the raw data but you can test compliation with 
		./example 11111-raw-data.root
	this example can process any number of root file given to it, and it should output the size of CH0 vector array
	
	The PmtData library is an example of how to build an inheirted libray
	based on Rawdata so that more concertated analaysis can be done.

Runnng the Python Libraries
===========================

	Just include the RawData.py in your anaylis directory and add the following line to your python script
	
		from RawData import RawData
	You can then create a RawData objects with the following line
		rd=RawData("111111-rawdata.root")

TODO
====

	-More channels need to be added to both python and c++ libraries
	-Make a c++ implementation python library for performance
	-Add a setup.py script so RawData can included with having to copy over the .py file
	-There is a bug with RawData.GetTrace that sometimes in does not always display the graph of the trace. 	
