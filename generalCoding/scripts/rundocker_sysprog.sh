#!/bin/zsh

# Define the Docker command as an array
docker_command=("docker" "run" "--platform" "linux/amd64" "-v" "/Users/lennard/Desktop/Docker Template for Sysprog/home:/home/user" "--rm" "-it" "sysprog" "bash")

# Run the Docker command
"${docker_command[@]}"
