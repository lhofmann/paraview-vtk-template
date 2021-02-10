#include "MyAlgorithm.h"

#include <vtkCommand.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkSmartPointer.h>
#include <vtkDataObject.h>
#include <vtkLogger.h>
#include <vtkPolyData.h>

vtkStandardNewMacro(MyAlgorithm);

MyAlgorithm::MyAlgorithm() {
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
}

MyAlgorithm::~MyAlgorithm() = default;

int MyAlgorithm::FillInputPortInformation(int port, vtkInformation *info) {
  if (port == 0) {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
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
  default_request_data_object<vtkPolyData>(this, outputVector, 0);
  return 1;
}

int MyAlgorithm::RequestInformation(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
  return 1;
}

int MyAlgorithm::RequestUpdateExtent(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
  return 1;
}

int MyAlgorithm::RequestData(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
  vtkPolyData *input = vtkPolyData::GetData(inputVector[0], 0);
  vtkPolyData *output = vtkPolyData::GetData(outputVector, 0);
  if (!input || !output)
    return 0;

  output->ShallowCopy(input);

  return 1;
}
