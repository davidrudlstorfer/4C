#!/bin/bash

# This file is part of 4C multiphysics licensed under the
# GNU Lesser General Public License v3.0 or later.
#
# See the LICENSE.md file in the top-level for license information.
#
# SPDX-License-Identifier: LGPL-3.0-or-later

# Define variables
fourc_exe="/home/rudlstorfer/01_work/01_workspace/4C_build_release/4C"  # Update this path
convert_dir="/home/rudlstorfer/01_work/01_workspace/4C/tests/input_files/"  # Update this path

# Exit immediately if a command fails
set -e

# Find and process .dat files
find "$convert_dir" -type f -name "*.dat" | while read -r file; do

    # Check if the corresponding .4C.yaml file already exists and skip it
    yaml_file="${file%.dat}.4C.yaml"
    if [[ -f "$yaml_file" ]]; then
        echo "Skipping $file (already converted: $yaml_file)"
        continue  # Skip this file
    fi

    echo "Processing $file..."
    "$fourc_exe" --to-yaml "$file"
done