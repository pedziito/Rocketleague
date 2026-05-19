#!/usr/bin/env bash
set -euo pipefail

OUT=rocketleague-source.zip
rm -f "$OUT"
zip -r "$OUT" . -x "*/.git/*" -x "*/build/cmake-out/*" -x "*published/*" -x "*.vs/*" -x "*.vscode/*"

echo "Created $OUT"
