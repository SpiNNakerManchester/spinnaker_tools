#!/bin/sh

# Build and package up a release of the SpiNNaker low-level tools. Builds SC&MP
# and all documentation in a clean check-out of the repository and packages
# a subset of build artefacts (e.g. PDFs and SC&MP binary).

if [ -z "$SPINN_DIRS" ]; then
  echo "ERROR: \$SPINN_DIRS not defined. Did you source setup?"
  exit 1
fi
cd "$SPINN_DIRS"

# Version low-level tools version number
VER=`grep SLLT_VER_STR include/version.h | cut -f3 | sed s/\"//g`

# Release name
NAME=spinnaker_tools_$VER

# Location of released file
RELEASE_DIR=/tmp
RELEASE_FILE="$RELEASE_DIR/$NAME.tar.gz"


# Warn if there are uncommitted changes
if [ -n "$(git status --porcelain | grep -vx " M setup")" ]; then
  echo "WARNING: There are some uncommitted changes."
  git status --short
  echo "Press <return> to build anyway or Ctrl+C to stop."
  read
fi

# Warn if we're not in the master branch
if [ -z "$(git branch | grep -x "* master")" ]; then
  echo "WARNING: Not in master branch."
  git branch
  echo "Press <return> to build anyway or Ctrl+C to stop."
  read
fi

# Use a temporary working directory to produce the build
WORKING_DIR="$(mktemp -d)"
echo "Building in temporary dir: $WORKING_DIR"
cd "$WORKING_DIR"
git clone --quiet --depth 1 "file://$SPINN_DIRS" .

# Build SARK
echo "Building SARK"
(cd "$WORKING_DIR/sark" && make GNU=0 SPINN_DIRS=$WORKING_DIR) || exit 2

# Build SC&MP
echo "Building SC&MP"
(cd "$WORKING_DIR/scamp" && make GNU=0 install SPINN_DIRS=$WORKING_DIR) || exit 3

# Build documentation
for document_makefile in $(find docs/ -name Makefile); do
  document_dir="$(dirname "$document_makefile")"
  echo "Building $document_dir"
  (cd "$document_dir" && make < /dev/null) || exit 4
done

# Package just the git-tracked files for the release
echo "Building release"
git archive --output "$NAME.tar" --prefix "$NAME/" HEAD \
  README.md release.txt setup Makefile \
  build include sark spin1_api lib make tools apps \
  || exit 5

# Add selected build artefacts
echo "Adding build artefacts"
(
    # Directories for documentation
    find docs -type d

    # Documentation (excluding PDFs of figures)
    for document_makefile in $(find docs/ -name Makefile); do
        document_dir="$(dirname "$document_makefile")"
        echo "$document_dir/$(basename "$document_dir").pdf"
    done

    # SC&MP binary
    echo "tools/boot/scamp.boot"
) | xargs tar fr "$NAME.tar" --transform='s:.*:'"$NAME"'/\0:' --no-recursion \
  || exit 6


# Gzip the tar file and place this in the output directory
echo "Compressing archive"
mkdir -p "$RELEASE_DIR"
gzip -c "$NAME.tar" > "$RELEASE_FILE" || exit 7

echo "Removing temporary files"
rm -rf "$WORKING_DIR" || exit 8

echo "Release created: $RELEASE_FILE"
