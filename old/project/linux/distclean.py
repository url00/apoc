from shutil import rmtree
import sys
from time import sleep


def main(out):
    rmtree(out)


if __name__ == "__main__":
    sleep(1)
    main(sys.argv[1])
