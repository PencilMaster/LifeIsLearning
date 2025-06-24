import os
import subprocess
import shutil
import sys

def run_command(command, cwd=None):
    result = subprocess.run(command, shell=True, cwd=cwd)
    if result.returncode != 0:
        raise RuntimeError(f"Command '{command}' failed with return code {result.returncode}")

def ensure_command_exists(command):
    if shutil.which(command) is None:
        raise RuntimeError(f"'{command}' is not installed or not found in PATH")

# Ensure cmake and make are installed
ensure_command_exists('cmake')
ensure_command_exists('make')

# Get the directory of this script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Check for debug argument
debug_mode = len(sys.argv) > 1 and sys.argv[1] == 'debug'
project_cmake_command = 'cmake .. -DCMAKE_BUILD_TYPE=Debug' if debug_mode else 'cmake ..'

# Build sockpp
third_party_dir = os.path.join(script_dir, '../lib/third-party/sockpp')
build_dir = os.path.join(third_party_dir, 'build')
os.makedirs(build_dir, exist_ok=True)
run_command('cmake ..', cwd=build_dir)
run_command('make', cwd=build_dir)

# Build rapidjson
third_party_dir = os.path.join(script_dir, '../lib/third-party/rapidjson')
build_dir = os.path.join(third_party_dir, 'build')
os.makedirs(build_dir, exist_ok=True)
run_command('cmake ..', cwd=build_dir)
run_command('make', cwd=build_dir)

# Build project
project_build_dir = os.path.join(script_dir, '../build')
os.makedirs(project_build_dir, exist_ok=True)
run_command(project_cmake_command, cwd=project_build_dir)
run_command('make', cwd=project_build_dir)
if(debug_mode): run_command('make tests', cwd=project_build_dir)