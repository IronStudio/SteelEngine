import subprocess
import os
from pathlib import Path


steel_engine_directory = os.getcwd()

# download submodules
subprocess.run(["git", "submodule", "init"])
subprocess.run(["git", "submodule", "update"])

# create binaries folder to put shared object
binaries_folder = [
	"bin/debug/sandbox",
	"bin/release/sandbox",
	"bin/dist/sandbox"
]
for folder in binaries_folder:
	Path(steel_engine_directory + "/" + folder).mkdir(exist_ok=True, parents=True)


# build and prepare SDL2
sdl2_directory = steel_engine_directory + "/vendors/SDL2"
Path(sdl2_directory + "/build").mkdir(exist_ok=True, parents=True)
os.chdir(sdl2_directory + "/build")
os.system("cmake -G \"MinGW Makefiles\" ..")
subprocess.run(["make", "all"])
Path(sdl2_directory + "/build/lib").mkdir(exist_ok=True, parents=True)
os.system("move lib*.a lib")
os.system("move sdl2.pc lib")
os.system("move SDL2.dll lib")
for folder in binaries_folder:
	os.system("copy lib/SDL2.dll " + steel_engine_directory + "/" + folder)
os.chdir(steel_engine_directory)
print(os.getcwd())