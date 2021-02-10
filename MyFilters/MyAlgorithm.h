#ifndef MYALGORITHM_H_
#define MYALGORITHM_H_

#include "TemplateMyFiltersModule.h"
#include <vtkDataObjectAlgorithm.h>

class TEMPLATEMYFILTERS_EXPORT MyAlgorithm : public vtkDataObjectAlgorithm {
 public:
  static MyAlgorithm *New();
  vtkTypeMacro(MyAlgorithm, vtkDataObjectAlgorithm);

  vtkSetMacro(Multiplier, double);
  vtkGetMacro(Multiplier, double);

  vtkSetMacro(DoMultiply, bool);
  vtkGetMacro(DoMultiply, bool);

 protected:
  double Multiplier {1.};
  bool DoMultiply {true};

  MyAlgorithm();
  ~MyAlgorithm() override;

  int FillInputPortInformation(int port, vtkInformation* info) override;
  int FillOutputPortInformation(int port, vtkInformation* info) override;

  int RequestInformation(
      vtkInformation* request,
      vtkInformationVector** inputVector,
      vtkInformationVector* outputVector) override;

  int RequestUpdateExtent(
      vtkInformation* request,
      vtkInformationVector** inputVector,
      vtkInformationVector* outputVector) override;

  int RequestDataObject(
      vtkInformation* request,
      vtkInformationVector** inputVector,
      vtkInformationVector* outputVector) override;

  int RequestData(
      vtkInformation* request,
      vtkInformationVector** inputVector,
      vtkInformationVector* outputVector) override;

 private:
  MyAlgorithm(const MyAlgorithm&); // Not implemented.
  void operator=(const MyAlgorithm&); // Not implemented.
};

#endif //MYALGORITHM_H_
