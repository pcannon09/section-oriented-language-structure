import os
import sys
import subprocess

def clone(url, dest):
	cmd = [ "git", "clone", url, dest ]

	subprocess.run(cmd, check=True);

def main():
	clone("https://www.github.com/pcannon09/python-requirements.git", "python-requirements")

	return 0;

if __name__ == "__main__":
	sys.exit(main())
	
