__all__ = ['MyImageSource']
from vtkmodules.util.vtkAlgorithm import VTKPythonAlgorithmBase
from vtkmodules.numpy_interface import dataset_adapter as dsa
from vtkmodules.vtkCommonDataModel import vtkImageData
import numpy as np

from .paraview_util import smproxy, smproperty, smdomain, smhint
# equivalent to
# from paraview.util.vtkAlgorithm import smproxy, smproperty, smdomain, smhint
# but also supports pure VTK
 
 
@smproxy.source(label="My Image Source")
@smhint.xml('''<ShowInMenu category="Template" />''')
class MyImageSource(VTKPythonAlgorithmBase):
    def __init__(self):
        self.Dimensions = [101, 101, 101]
        self.T = 10.0
        self.A = 0.1
        self.eps = 0.1
        VTKPythonAlgorithmBase.__init__(self, nInputPorts=0, nOutputPorts=1, outputType='vtkImageData')
 
    @smproperty.intvector(name='Dimensions', default_values=[101, 101, 101])
    def SetDimensions(self, x, y, z):
        self.Dimensions = [x, y, z]
        self.Modified()
 
    @smproperty.doublevector(name='T', default_values=10.0)
    def SetT(self, T):
        self.T = T
        self.Modified()
 
    @smproperty.doublevector(name='A', default_values=0.1)
    def SetA(self, A):
        self.A = A
        self.Modified()
 
    @smproperty.doublevector(name='eps', default_values=0.1)
    def Seteps(self, eps):
        self.eps = eps
        self.Modified()
 
    def RequestInformation(self, request, inInfo, outInfo):
        # set extent information for image data output
        executive = self.GetExecutive()
        info = outInfo.GetInformationObject(0)
        info.Set(executive.WHOLE_EXTENT(),
                 [0, self.Dimensions[0] - 1, 0, self.Dimensions[1] - 1, 0, self.Dimensions[2] - 1], 6)
        return 1
  
    def RequestData(self, request, inInfo, outInfo):
        output = vtkImageData.GetData(outInfo, 0)
        if output is None:
            return 0

        # create 3D coordinate grid
        xs = np.linspace(0.0, 2.0, self.Dimensions[0])
        ys = np.linspace(0.0, 1.0, self.Dimensions[1])
        ts = np.linspace(0.0, 10.0, self.Dimensions[2])
        x, y, t = np.meshgrid(xs, ys, ts, indexing='ij')
 
        # allocate image data output
        output.SetDimensions(self.Dimensions)
        output.SetSpacing(abs(xs[1] - xs[0]), abs(ys[1] - ys[0]), abs(ts[1] - ts[0]))
        output.SetOrigin(xs[0], ys[0], ts[0])
        output = dsa.WrapDataObject(output)
 
        # double gyre (https://shaddenlab.berkeley.edu/uploads/LCS-tutorial/examples.html)
        a = self.eps * np.sin(2.0 * np.pi / self.T * t)
        b = 1.0 - 2.0 * self.eps * np.sin(2.0 * np.pi / self.T * t)
        f = a * x**2 + b * x
        df = 2.0 * a * x + b
        dx = -np.pi * self.A * np.sin(np.pi * f) * np.cos(np.pi * y)
        dy = np.pi * self.A * np.cos(np.pi * f) * np.sin(np.pi * y) * df
        dt = np.full_like(t, 1.0)

        # stack and flatten such that layout matches VTK
        v = np.stack([dx, dy, dt], axis=-1)
        v = v.reshape((-1, 3), order='F')
        output.PointData.append(v, 'v')

        # compute and output velocity magnitude
        v_mag = np.linalg.norm(v[..., :2], axis=-1)
        output.PointData.append(v_mag, 'v_mag')
 
        return 1

