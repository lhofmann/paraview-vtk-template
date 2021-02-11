__all__ = ['smproxy', 'smproperty', 'smdomain', 'smhint']
try:
    from paraview.util.vtkAlgorithm import smproxy, smproperty, smdomain, smhint
except ImportError:
    # we are being called from VTK
    # create dummy decorators smproxy, smproperty, smdomain, smhint

    from functools import wraps

    def _mock_method(*args, **kwargs):
        def _mock_decorator(func):
            @wraps(func)
            def wrapper(*args, **kwargs):
                return func(*args, **kwargs)
            return wrapper
        return _mock_decorator

    class _mock_class:
        def __getattr__(self, name):
            return _mock_method

    smproxy = smproperty = smhint = smdomain = _mock_class()
