import os, sys
# add this directory to PYTHONPATH
cwd = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(cwd, '..'))
# import ParaView plugins
from my_python_module.all_plugins import *
