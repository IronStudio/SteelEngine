import subprocess
import sys

shared = "--shared"

if len(sys.argv) == 2:
	if sys.argv[1] == "--static":
		shared = ""


subprocess.run(["premake5", "gmake2", shared])
subprocess.run(["make", "engine"])
subprocess.run(["make", "test"])