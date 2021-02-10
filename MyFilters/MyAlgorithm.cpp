#include "MyAlgorithm.h"

#include <vtkCommand.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkSmartPointer.h>
#include <vtkDataObject.h>
#include <vtkLogger.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>

vtkStandardNewMacro(MyAlgorithm);

MyAlgorithm::MyAlgorithm() {
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
}

MyAlgorithm::~MyAlgorithm() = default;

int MyAlgorithm::FillInputPortInformation(int port, vtkInformation *info) {
  if (port == 0) {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
    // optional:
    // info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
    // repeatable:
    // info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
  }
  return 1;
}

int MyAlgorithm::FillOutputPortInformation(int port, vtkInformation *info) {
  if (port == 0) {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
  }
  return 1;
}

namespace {

template <typename DataType>
void default_request_data_object(vtkAlgorithm *self, vtkInformationVector *outputVector, int port) {
  vtkInformation *outInfo = outputVector->GetInformationObject(port);
  auto output = DataType::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!output) {
    output = DataType::New();
    outInfo->Set(vtkDataObject::DATA_OBJECT(), output);
    output->FastDelete();
    self->GetOutputPortInformation(port)->Set(
        vtkDataObject::DATA_EXTENT_TYPE(), output->GetExtentType() );
  }
}

void pass_input_type_request_data_object(vtkInformationVector **inputVector, int input_port, vtkInformationVector *outputVector, int port) {
  vtkInformation *inInfo = inputVector[input_port]->GetInformationObject(0);
  if (!inInfo) {
    return;
  }
  vtkDataObject *input = inInfo->Get(vtkDataObject::DATA_OBJECT());
  if (input) {
    vtkInformation *info = outputVector->GetInformationObject(port);
    vtkDataObject *output = info->Get(vtkDataObject::DATA_OBJECT());

    if (!output || !output->IsA(input->GetClassName())) {
      vtkDataObject *newOutput = input->NewInstance();
      info->Set(vtkDataObject::DATA_OBJECT(), newOutput);
      newOutput->Delete();
    }
  }
}

}

int MyAlgorithm::RequestDataObject(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
  // create vtkPolyData on output port 0
  default_request_data_object<vtkPolyData>(this, outputVector, 0);
  // copy data type from input port 0 to output port 0
  // pass_input_type_request_data_object(inputVector, 0, outputVector, 0);
  return 1;
}

int MyAlgorithm::RequestInformation(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
  // nothing to do since output is not structured grid
  return 1;
}

int MyAlgorithm::RequestUpdateExtent(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
  // nothing to do since output is not structured grid
  return 1;
}

int MyAlgorithm::RequestData(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
  vtkImageData *input = vtkImageData::GetData(inputVector[0], 0);
  vtkPolyData *output = vtkPolyData::GetData(outputVector, 0);
  if (!input || !output)
    return 0;

  vtkDataArray *array = this->GetInputArrayToProcess(0, input);
  if (!array) {
    vtkLog(ERROR, "No input array.");
    return 0;
  }
  if (array->GetNumberOfTuples() < 1) {
    vtkLog(ERROR, "Input array has not enough tuples.");    
    return 0;
  }
  if (array->GetNumberOfComponents() != 1) {
    vtkLog(ERROR, "Input array needs to have exactly one component.");    
    return 0;
  }

  vtkNew<vtkPolyData> result;

  vtkNew<vtkPoints> points;
  result->SetPoints(points);
  result->Allocate();
  points->InsertPoint(0, 1., 1., 1.);
  points->InsertPoint(1, 2., 2., 2.);
  vtkIdType ids[2] {0, 1};
  result->InsertNextCell(VTK_POLY_LINE, 2, ids);

  vtkNew<vtkDoubleArray> data;
  data->SetName("Data");
  data->SetNumberOfComponents(1);
  data->SetNumberOfTuples(1);
  result->GetCellData()->AddArray(data);

  double value = array->GetTuple1(0);
  if (this->DoMultiply)
    value *= this->Multiplier;
  data->SetValue(0, value);

  output->ShallowCopy(result);

  return 1;
}
