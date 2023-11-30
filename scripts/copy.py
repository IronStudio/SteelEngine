import shutil
import sys
from pathlib import Path

if len(sys.argv) != 3:
	print("This script take exactly 2 arguments : the file to copy and the destination")

Path(sys.argv[2]).mkdir(parents=True, exist_ok=True)

shutil.copy(sys.argv[1], sys.argv[2])