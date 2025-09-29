import os
from Source.Dependencies.Panshilar import buildutils

FOLDER_STRUCTURE = buildutils.getFolderStructure(os.path.dirname(os.path.abspath(__file__)))
MAIN_FILE_C   = FOLDER_STRUCTURE.srcDir + 'zzzz_Unity.c'
MAIN_FILE_CXX = FOLDER_STRUCTURE.srcDir + 'zzzz_Unity.cpp'
