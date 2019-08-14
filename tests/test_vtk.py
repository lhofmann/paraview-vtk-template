import vtk
from my_vtk import ExampleFilter

ug = vtk.vtkUnstructuredGrid()
f = ExampleFilter()
f.SetInputDataObject(ug)
f.Update()
answer = f.GetOutput().GetFieldData().GetArray('Answer').GetValue(0)

print('The answer is {}.'.format(answer))
assert(answer == 42)
