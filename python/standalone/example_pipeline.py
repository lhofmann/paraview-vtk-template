from my_python_module.MyImageSource import MyImageSource
from my_vtk_module import MyAlgorithm
from vtkmodules.vtkIOXML import vtkXMLImageDataReader, vtkXMLPolyDataWriter
from vtkmodules.vtkCommonDataModel import vtkDataObject
import sys

def main():
    reader = vtkXMLImageDataReader()
    source = MyImageSource()
    algorithm = MyAlgorithm()
    writer = vtkXMLPolyDataWriter()

    if len(sys.argv) == 5:
        reader.SetFileName(sys.argv[1])
        algorithm.SetInputConnection(reader.GetOutputPort())
        algorithm.SetInputArrayToProcess(0, 0, 0, vtkDataObject.FIELD_ASSOCIATION_POINTS, sys.argv[2])
        algorithm.SetMultiplier(float(sys.argv[3]))
        writer.SetFileName(sys.argv[4])
    elif len(sys.argv) == 3:
        algorithm.SetInputConnection(source.GetOutputPort())
        algorithm.SetInputArrayToProcess(0, 0, 0, vtkDataObject.FIELD_ASSOCIATION_POINTS, 'v_mag')
        algorithm.SetMultiplier(float(sys.argv[1]))     
        writer.SetFileName(sys.argv[2])
    else:
        print('Usage: {} [IN_FILE IN_ARRAY] MULTIPLIER OUT_FILE'.format(sys.argv[0]))
        sys.exit(1)

    writer.SetInputConnection(algorithm.GetOutputPort())
    writer.Update()

if __name__ == '__main__':
    main()
