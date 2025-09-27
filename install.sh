#!/bin/bash

sudo mkdir -p /etc/awcc/

if [ ! -e /etc/awcc/database.json ]; then
    sudo ln -sf "$PWD/database.json" /etc/awcc/database.json
fi
