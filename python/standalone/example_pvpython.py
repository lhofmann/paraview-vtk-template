# this file is modified from ParaView's Save State (as .py)
from paraview.simple import *

# create a new 'My Image Source'
myImageSource1 = MyImageSource()

# create a new 'My Algorithm'
myAlgorithm1 = MyAlgorithm(Input=myImageSource1)
myAlgorithm1.InputArray = ['POINTS', 'v_mag']
myAlgorithm1.Multiplier = 3.0

# run the pipeline and get the result:
#   myAlgorithm1.UpdatePipeline()
#   output = servermanager.Fetch(myAlgorithm1)

# run pipeline and save output
SaveData('output.vtp', myAlgorithm1)
