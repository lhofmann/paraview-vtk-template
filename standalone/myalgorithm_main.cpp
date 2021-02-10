#include "MyAlgorithm.h"

#include <vtkNew.h>
#include <vtkXMLImageDataReader.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkImageData.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>

#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  vtkNew<vtkXMLImageDataReader> reader;
  vtkNew<vtkImageData> sample_data;
  vtkNew<MyAlgorithm> my_algorithm;
  vtkNew<vtkXMLPolyDataWriter> writer;
  if (argc == 5) {
    reader->SetFileName(argv[1]);
    writer->SetFileName(argv[4]);
    my_algorithm->SetInputConnection(reader->GetOutputPort());
    my_algorithm->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, argv[2]);
    my_algorithm->SetMultiplier(std::stod(argv[3]));
  } else if (argc == 3) {
    sample_data->SetDimensions(3,3,3);    
    vtkNew<vtkDoubleArray> data;
    data->SetName("SampleData");
    data->SetNumberOfComponents(1);
    data->SetNumberOfTuples(3*3*3);
    data->Fill(5.);
    sample_data->GetPointData()->AddArray(data);

    writer->SetFileName(argv[2]);
    my_algorithm->SetInputData(sample_data);
    my_algorithm->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "SampleData");
    my_algorithm->SetMultiplier(std::stod(argv[1]));
  } else {
    std::cout << "Usage: " << argv[0] << " [IN_FILE IN_ARRAY] MULTIPLIER OUT_FILE" << std::endl;
    return 1;
  }
  writer->SetInputConnection(my_algorithm->GetOutputPort());
  writer->Update();
  return 0;
}
