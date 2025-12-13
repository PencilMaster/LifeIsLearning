import os
import sys

def tryFinally():
    try:
        return 42
    finally:
        os.system('echo "Finally was executed"') 

def main():
    print(tryFinally())

if __name__ == "__main__":
    main()
