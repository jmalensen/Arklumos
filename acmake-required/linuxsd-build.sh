#!/bin/bash
# This is used because the build.ninja on the SD has issue with linking properly librt.a
echo "Replace '/x86_64-linux-gnu/' by '/' in build.ninja"

sed -i 's/\/x86_64-linux-gnu\//\//g' ../build/build.ninja

echo "replace done!"
