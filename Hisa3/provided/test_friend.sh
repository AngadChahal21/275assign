#!/bin/bash
# Complete testing script for A3: sequences, maze, writeval
# Run from: ~/275-Work/Assignments/a3/provided/

BASEDIR="$(cd "$(dirname "$0")" && pwd)"
GCC="gcc -Wvla -Wall -Werror"
PASS=0
FAIL=0

run_test() {
    local command="$1"
    local stem="$2"
    local tmpfile=$(mktemp)
    local vglo=$(mktemp)
    local v_fail=0

    # Execute and check memory
    if [ -f "$stem.args" ] && [ -s "$stem.args" ]; then
        $command $(cat "$stem.args") < "${stem}.in" > "$tmpfile" 2>&1
        if command -v valgrind > /dev/null 2>&1; then
            valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=100 $command $(cat "$stem.args") < "${stem}.in" > /dev/null 2> "$vglo"
            if [ $? -eq 100 ]; then v_fail=1; fi
        fi
    else
        $command < "${stem}.in" > "$tmpfile" 2>&1
        if command -v valgrind > /dev/null 2>&1; then
            valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=100 $command < "${stem}.in" > /dev/null 2> "$vglo"
            if [ $? -eq 100 ]; then v_fail=1; fi
        fi
    fi

    diff "$tmpfile" "${stem}.out" > /dev/null 2>&1
    local out_ok=$?
    
    if [[ $out_ok -eq 0 ]] && [[ $v_fail -eq 0 ]]; then
        echo "  PASS: ${stem} (Valgrind Clean)"
        PASS=$((PASS+1))
    else
        echo "  FAIL: ${stem}"
        if [[ $out_ok -ne 0 ]]; then
            echo "  --- Expected output ---"
            cat "${stem}.out"
            echo "  --- Actual output ---"
            cat "$tmpfile"
            echo "  ----------------------"
        fi
        if [[ $v_fail -ne 0 ]]; then
            echo "  --- Valgrind Memory Leak/Error ---"
            cat "$vglo"
            echo "  ----------------------------------"
        fi
        FAIL=$((FAIL+1))
    fi
    rm "$tmpfile" "$vglo"
}

########################################
# Q1 - SEQUENCES
########################################
echo "========================================"
echo "  A3 Complete Testing Script"
echo "  gcc flags: -Wvla -Wall -Werror"
echo "========================================"
echo ""
echo "======== Q1: SEQUENCES ========"
cd "$BASEDIR/q1"
chmod +x sequences_sample

# Generate expected .out files (stdout + stderr)
for t in test1 test2 test3 test4; do
    ./sequences_sample $(cat "$t.args") < "$t.in" > "$t.out" 2>&1
done

# Create additional error/edge cases for Q1
cat > seq_err1.in << 'EOF'
bla 10
add 5
foo 20
mul 2
n
EOF
echo "10" > seq_err1.args

cat > seq_empty.in << 'EOF'
n
EOF
echo "100" > seq_empty.args

cat > seq_noargs.in << 'EOF'
add 5
n
EOF
touch seq_noargs.args

# Generate .out for new tests
for t in seq_err1 seq_empty seq_noargs; do
    if [ -f "$t.args" ] && [ -s "$t.args" ]; then
        ./sequences_sample $(cat "$t.args") < "$t.in" > "$t.out" 2>&1
    else
        ./sequences_sample < "$t.in" > "$t.out" 2>&1
    fi
done

echo "Generated .out files from sequences_sample"

# Compile
$GCC sequences.c
if [[ $? -ne 0 ]]; then
    echo "  COMPILE FAILED for sequences.c"
else
    echo "Compiled sequences.c"
    for t in test1 test2 test3 test4 seq_err1 seq_empty seq_noargs; do
        run_test ./a.out "$t"
    done
fi

########################################
# Q2 - MAZE
########################################
echo ""
echo "======== Q2: MAZE ========"
cd "$BASEDIR/q2"
chmod +x mazeSample

# Generate expected .out files (stdout + stderr)
for t in test1 test2 test3 simpletest; do
    touch "$t.args"
    ./mazeSample < "$t.in" > "$t.out" 2>&1
done

# Create additional edge-case tests

# Test: malformed maze (no start tile)
cat > errortest1.in << 'EOF'
OOOXO
OXOOO
OXXXO
OOXXG

p
EOF
touch errortest1.args

# Test: malformed maze (no goal tile)
cat > errortest2.in << 'EOF'
SOOXO
OXOOO
OXXXO
OOXXO

p
EOF
touch errortest2.args

# Test: multiple starts
cat > errortest3.in << 'EOF'
SOOXO
SXOOO
OXXXO
OOXXG

p
EOF
touch errortest3.args

# Test: invalid tile character
cat > errortest4.in << 'EOF'
SOOXO
OXZOO
OXXXO
OOXXG

p
EOF
touch errortest4.args

# Test: reset and move commands
cat > movetest1.in << 'EOF'
SOOG
OOOX
OOOO

p
me p
me p
me p
r p
ms p
q
EOF
touch movetest1.args

# Test: invalid command and invalid move direction
cat > errortest5.in << 'EOF'
SOOG

p
z
ma
q
EOF
touch errortest5.args

# Test: walk into walls and edges
cat > walltest1.in << 'EOF'
XSXG
XOXX

mn p
mw p
ms p
me p
q
EOF
touch walltest1.args

# Test: teleport logic
cat > tptest1.in << 'EOF'
SO1O
OOOO
O21G
OOOO

p
me p
me p
EOF
touch tptest1.args

# Test: ice sliding
cat > icetest1.in << 'EOF'
SOIO
OXXO
OOOO
IXXG

p
me p
ms p
EOF
touch icetest1.args

# Generate .out for all new tests
for t in errortest1 errortest2 errortest3 errortest4 movetest1 errortest5 walltest1 tptest1 icetest1; do
    ./mazeSample < "$t.in" > "$t.out" 2>&1
done

echo "Generated .out files from mazeSample (including edge cases)"

# Compile
$GCC main.c maze.c
if [[ $? -ne 0 ]]; then
    echo "  COMPILE FAILED for maze"
else
    echo "Compiled maze"
    for t in simpletest test1 test2 test3 errortest1 errortest2 errortest3 errortest4 movetest1 errortest5 walltest1 tptest1 icetest1; do
        run_test ./a.out "$t"
    done
fi

########################################
# Q4 - WRITEVAL
########################################
echo ""
echo "======== Q4: WRITEVAL ========"
cd "$BASEDIR/q4"
chmod +x writeValSample

# Generate expected .out for existing test (stdout + stderr)
touch test1.args
./writeValSample < test1.in > test1.out 2>&1

# Create additional tests

# Test: char and ptr types
cat > test2.in << 'EOF'
char A
char z
int 42
EOF
touch test2.args

# Test: pointer type
cat > test3.in << 'EOF'
ptr 0
int 100
str hello
EOF
touch test3.args

# Test: empty input (no values written)
cat > test4.in << 'EOF'
EOF
touch test4.args

# Test: multiple strings
cat > test5.in << 'EOF'
str first
str second
str third
EOF
touch test5.args

# Test: mixed types
cat > test6.in << 'EOF'
int 0
int -1
char X
str test
int 2147483647
EOF
touch test6.args

# Test: edge case extreme numbers and string
cat > test7.in << 'EOF'
int -2147483648
str a_really_long_string_to_check_bounds_and_memory
char ~
ptr FFFFFFFFFFFFFFFF
EOF
touch test7.args

# Test: invalid command
cat > test8.in << 'EOF'
float 1.25
int 5
EOF
touch test8.args

# Generate .out for all tests
for t in test1 test2 test3 test4 test5 test6 test7 test8; do
    ./writeValSample < "$t.in" > "$t.out" 2>&1
done

echo "Generated .out files from writeValSample (including edge cases)"

# Compile
$GCC harness.c writeval.c raw.c
if [[ $? -ne 0 ]]; then
    echo "  COMPILE FAILED for writeVal"
else
    echo "Compiled writeval"
    for t in test1 test2 test3 test4 test5 test6 test7 test8; do
        run_test ./a.out "$t"
    done
fi

########################################
echo ""
echo "========================================"
echo "  Results: $PASS passed, $FAIL failed"
echo "========================================"
