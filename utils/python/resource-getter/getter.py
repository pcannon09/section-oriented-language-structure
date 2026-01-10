import sys
import tempfile
import os
import subprocess
import shutil

tmpDirInfo = tempfile.TemporaryDirectory()

def clone(url, dest):
	cmd = [ "git", "clone", url, dest ]

	subprocess.run(cmd, check=True);

def commandGet():
	tmpDir = os.path.join(tmpDirInfo.name + "_resource-getter-pcannon09")

	url = "https://github.com/pcannon09/resource-getter.git"
	out = os.path.join(tmpDir, "resource-getter")

	clone(url, out)

	return out 

def commandDistribute(start, _type, end):
	if _type.lower() in ("stylex-qt", "sqt"):
		src = os.path.join(start, "styleX-qt", "rsrc")

		os.makedirs(end, exist_ok=True)

		shutil.move(src, os.path.join(end, "rsrc"))
	else:
		shutil.move(start, end)

	tmpDirInfo.cleanup()

def main():
	try:
		if sys.argv[1] == "help" or sys.argv[1] == "h":
			print("> get   |   [ dest ]")
			print("        |   [ stylex-qt | none ]")

		elif sys.argv[1] == "get":
			res = commandGet()
			mvType = ""

			if len(sys.argv) > 3: mvType = sys.argv[3]

			commandDistribute(res, mvType, sys.argv[2])

		else: print("Provide a valid param: `help` for more information")

	except IndexError:
		print("Please provide at least one parameter: `help` for more information")

	return 0

if __name__ == "__main__":
	sys.exit(main())

