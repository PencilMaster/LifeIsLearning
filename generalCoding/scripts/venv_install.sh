#!/bin/bash

# Define the venv PATH
VENV_PATH="$HOME/.venvs/nvim"

if [ ! -d "$VENV_PATH" ]; then
  echo "Creating Neovim Python virtual environment at $VENV_PATH..."
  python3 -m venv "$VENV_PATH"
fi

# Now we install pynvim inside venv
"$VENV_PATH/bin/pip" install --upgrade pip pynvim neovim

# To use this python in nvim we have to do the following
echo "Insert the following into your nvim init.lua file (check it before):"
echo "vim.g.python3_host_prog = vim.fn/expand(\"$VENV_PATH/bin/python\")"
