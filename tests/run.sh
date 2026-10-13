#!/bin/bash
# Copyright (c) 2026-2027, Qualcomm Innovation Center, Inc. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

nodes_root="/usr/share/urm"
TEST_NODES_DIR="$nodes_root/tests/nodes"

nodes_tmp_base="$(mktemp -d)"
echo "mktemp created: $nodes_tmp_base"
if [ -z "$nodes_tmp_base" ] || [ ! -d "$nodes_tmp_base" ]; then
    echo "mktemp -d failed — aborting"
    exit 1
fi
trap 'rm -rf "$nodes_tmp_base"' EXIT INT TERM

RUNTIME_NODES_DIR="$nodes_tmp_base/urm/tests/nodes"
if ! mkdir -p "$RUNTIME_NODES_DIR"; then
    echo "Failed to create staging directory $RUNTIME_NODES_DIR — suites requiring nodes will SKIP"
    exit 1
elif cp -r "$TEST_NODES_DIR/"* "$RUNTIME_NODES_DIR/"; then
    echo "Staged test nodes from $TEST_NODES_DIR to $RUNTIME_NODES_DIR"
else
    echo "Failed to stage test nodes into $RUNTIME_NODES_DIR — suites requiring nodes will SKIP"
    exit 1
fi

TESTS="
    /usr/bin/UrmComponentTests
    /usr/bin/UrmIntegrationTests
"

# ---------- Execute ----------
PASS=0
FAIL=0
SKIP=0

echo "Restarting URM service"
if ! sudo systemctl restart urm; then
    echo "ERROR: 'systemctl restart urm' failed — aborting" >&2
    exit 1
fi

echo "Waiting for URM service to become active..."
for i in $(seq 1 10); do
    if sudo systemctl is-active --quiet urm; then
        echo "URM service is active (attempt $i)"
        break
    fi
    if [ "$i" -eq 10 ]; then
        echo "ERROR: URM service did not become active after 10 attempts — aborting" >&2
        sudo systemctl status urm >&2
        exit 1
    fi
    sleep 1
done

echo "Proceeding with test-cases"
for t in $TESTS; do
    echo "Running: $t --npath $RUNTIME_NODES_DIR"
    $t --npath "$RUNTIME_NODES_DIR"
    rc=$?
    case $rc in
        0)
            PASS=$((PASS+1))
            ;;
        1)
            FAIL=$((FAIL+1))
            ;;
        2)
            SKIP=$((SKIP+1))
            ;;
    esac
done

echo "Overall Results [suite-level]: PASS=$PASS  FAIL=$FAIL  SKIP=$SKIP"

if [ "$FAIL" -gt 0 ]; then
  exit 1
fi

exit 0
