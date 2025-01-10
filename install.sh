#!/bin/bash

INSTALL_DIR="$HOME/.local/bin"

cd src
make || exit 1
cd ..

mkdir -p "$INSTALL_DIR"
cp src/gcal "$INSTALL_DIR/"

if ! grep -q "$INSTALL_DIR" ~/.bashrc; then
  echo "export PATH=\$PATH:$INSTALL_DIR" >> ~/.bashrc
fi

echo "Installation complete. Run 'source ~/.bashrc' to update PATH."

