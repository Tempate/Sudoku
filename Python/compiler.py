from os import path
from time import clock

directory = path.dirname(__file__)
import pyximport; pyximport.install(pyimport= True, build_dir=directory)

import Sudoku
print("Finished compiling\n\t---===---")

"""
The first time it is run it may take about 30s to set up everything
I only guarantee it works for Cython using python2.7
"""

beg = clock()

Sudoku.main()

print(clock() - beg)