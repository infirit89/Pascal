from asyncore import write
import sys
import subprocess

packages = [ "requests", "numpy" ]

def InstallPackages():
    for package in packages:
        sys.stdout.write("\rInstalling package: {}\n".format(package))
        subprocess.check_call([sys.executable, "-m", "pip", "install", package])
        sys.stdout.write('\n')

def UpdateSubmodules():
    sys.stdout.write("\rUpdating submodules: \n")
    subprocess.check_call(["git", "submodule", "update", "--init", "--recursive"])
    sys.stdout.write('\n')

if __name__ == "__main__":
    UpdateSubmodules()
    InstallPackages()

    print('test')

    from SetupPremake import PremakeSetup
    PremakeSetup.Setup()
