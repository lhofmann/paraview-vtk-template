#ifndef EXAMPLEFILTER_H
#define EXAMPLEFILTER_H

#include <vtkUnstructuredGridAlgorithm.h>
#include "ExampleFilterModule.h"

class EXAMPLEFILTER_EXPORT ExampleFilter : public vtkUnstructuredGridAlgorithm
{
 public:
  vtkTypeMacro(ExampleFilter,vtkUnstructuredGridAlgorithm);
  static ExampleFilter *New();

 protected:
  ExampleFilter();
  ~ExampleFilter();

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

 private:
  ExampleFilter(const ExampleFilter&);
  void operator=(const ExampleFilter&);
};

#endif //EXAMPLEFILTER_H
