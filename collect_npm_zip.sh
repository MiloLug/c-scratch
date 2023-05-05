#!/bin/bash

origin=zip_npm_base
destination=zip_npm_tmp
package_name=c-scratch-base-project
package_version=`npm view c-scratch-base-project version`

rm -rf "$destination"
cp -r "$origin" "$destination"

sed -E \
  -e 's/\{\{package_version\}\}/'"$package_version"'/' \
  -e 's/\{\{package_name\}\}/'"$package_name"'/' \
  <"$origin/package.json" > "$destination/package.json"

mkdir -p "$destination/dist/project/assets/sprites"
mkdir -p "$destination/dist/project/assets/backdrop/costumes"
mkdir -p "$destination/dist/project/assets/backdrop/sounds"

cp -r runtime .gitignore CMakeLists.txt config.h source.cpp "$destination/dist/project/"
cp -r "$origin/stage" "$origin/assets" "$destination/"

cd "$destination/dist/project"
zip -r ../project.zip *
