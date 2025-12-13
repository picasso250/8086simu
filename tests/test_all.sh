#!/bin/bash

# Get the directory where the script is located, and fall back to a simpler method if readlink isn't available.
root=$(cd "$(dirname "$0")" && pwd)
cd "$root" || exit

passed_tests=0

# Find all .asm files
files=$(find "$root" -iname "*.asm" | sort)

for file in $files
do
    echo "-----------------------------------------"
    echo "Running test: $(basename "$file")"
    
    # Run the individual test script. Suppress its output for a cleaner summary.
    # The individual test will still print asserts.
    bash test_file.sh "$file" > /dev/null
    
    if [ $? -eq 0 ]; then
        echo "Result: PASSED"
        ((passed_tests++))
    else
        echo "Result: FAILED for $(basename "$file")"
		echo "Stopping on first failure."
        exit 1
    fi
done

echo "========================================="
echo "Success! All $passed_tests tests passed."
echo "========================================="

exit 0