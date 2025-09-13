import os
from .Source.Dependencies.Panshilar import buildutils

FOLDER_STRUCTURE = buildutils.getFolderStructure(os.path.dirname(os.path.abspath(__file__)))
