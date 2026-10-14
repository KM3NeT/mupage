#!/bin/bash
set -e

MUPAGE_DAT_DIR=${MUPAGE_DAT_DIR:-dat}
MUPAGE_BENCHMARK_DIR=${MUPAGE_BENCHMARK_DIR:-benchmark}

mupage.exe -i 1 -n 10 -s 12345678 -o mupage.txt -p "${MUPAGE_DAT_DIR}/parameters.dat" -f native
if [ -s "mupage.txt" ]; then
	echo Output file contains `cat mupage.txt | wc -l` lines.
else
	echo ERROR: Output file is not produced >&2
 	exit 1
fi

if diff -q "mupage.txt" "${MUPAGE_BENCHMARK_DIR}/mupage.txt" > /dev/null 2>&1; then
    echo Files are identical
else
    echo ERROR: Output file differs from the benchmark one >&2
    exit 1
fi
