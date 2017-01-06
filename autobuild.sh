#!/usr/bin/env bash

cmake -E make_directory build

#automatically runs cmake out-of-source
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/build" && cmake .. && make)"
