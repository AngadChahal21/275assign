#!/bin/bash
# Complete testing script for A4 Bonus: conways, intSet
# Compares stdout AND stderr against sample executables.
# Run from: ~/275-Work/Assignments/a4_bonus/provided/

BASEDIR="$(cd "$(dirname "$0")" && pwd)"
GPP="g++ -std=c++17 -Wall -Werror"
PASS=0
FAIL=0

# ──────────────────────────────────────────────────────────────
# run_test <binary> <stem>
#   Compares student stdout vs stem.out  (required)
#   Compares student stderr vs stem.err  (if stem.err exists)
#   Runs valgrind for memory leaks
# ──────────────────────────────────────────────────────────────
run_test() {
    local command="$1"
    local stem="$2"
    local tmp_out=$(mktemp)
    local tmp_err=$(mktemp)
    local vglo=$(mktemp)
    local v_fail=0

    $command < "${stem}.in" > "$tmp_out" 2> "$tmp_err"

    if command -v valgrind > /dev/null 2>&1 && [ "$NO_VALGRIND" != "1" ]; then
        valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=100 \
            $command < "${stem}.in" > /dev/null 2> "$vglo"
        if [ $? -eq 100 ]; then v_fail=1; fi
    fi

    diff "$tmp_out" "${stem}.out" > /dev/null 2>&1
    local stdout_ok=$?

    local stderr_ok=0
    if [ -f "${stem}.err" ]; then
        diff "$tmp_err" "${stem}.err" > /dev/null 2>&1
        stderr_ok=$?
    fi

    local all_ok=0
    if [[ $stdout_ok -ne 0 ]] || [[ $stderr_ok -ne 0 ]] || [[ $v_fail -ne 0 ]]; then
        all_ok=1
    fi

    if [[ $all_ok -eq 0 ]]; then
        if [ "$NO_VALGRIND" == "1" ]; then
            echo "  PASS: ${stem}"
        else
            echo "  PASS: ${stem} (Valgrind Clean)"
        fi
        PASS=$((PASS+1))
    else
        echo "  FAIL: ${stem}"
        if [[ $stdout_ok -ne 0 ]]; then
            echo "    [stdout mismatch]"
            echo "    --- Expected stdout ---"
            cat "${stem}.out" | sed 's/^/    /'
            echo "    --- Actual stdout ---"
            cat "$tmp_out" | sed 's/^/    /'
            echo "    ----------------------"
        fi
        if [[ $stderr_ok -ne 0 ]]; then
            echo "    [stderr mismatch]"
            echo "    --- Expected stderr ---"
            cat "${stem}.err" | sed 's/^/    /'
            echo "    --- Actual stderr ---"
            cat "$tmp_err" | sed 's/^/    /'
            echo "    ----------------------"
        fi
        if [[ $v_fail -ne 0 ]]; then
            echo "    [valgrind leak/error]"
            grep -A3 "LEAK\|ERROR\|definitely lost\|Invalid" "$vglo" | sed 's/^/    /' | head -20
        fi
        FAIL=$((FAIL+1))
    fi
    rm "$tmp_out" "$tmp_err" "$vglo"
}

# Helper: generate .out and .err from a sample executable
gen() {
    local sample="$1"
    local stem="$2"
    $sample < "${stem}.in" > "${stem}.out" 2> "${stem}.err"
}

echo "========================================"
echo "  A4 Bonus Complete Testing Script"
echo "  g++ flags: -std=c++17 -Wall -Werror"
echo "  Checking: stdout + stderr + valgrind"
echo "========================================"

########################################
# Q1 — CONWAYS GAME OF LIFE
########################################
echo ""
echo "======== Q1: CONWAYS GAME OF LIFE ========"
cd "$BASEDIR/q1"
chmod +x conways_sample
SAMPLE=./conways_sample

# ── Provided test cases ──────────────────────
for t in blinker stasis worker gospers large; do
    gen $SAMPLE $t
done

# ── Still lifes ──────────────────────────────
# 2×2 block — never changes
cat > block.in << 'EOF'
......
..OO..
..OO..
......
x
p
s p
s p
s p
EOF
gen $SAMPLE block

# Beehive — never changes
cat > beehive.in << 'EOF'
......
..OO..
.O..O.
..OO..
......
x
p
s p
s p
EOF
gen $SAMPLE beehive

# ── Oscillators ──────────────────────────────
# Blinker already provided.
# Toad (period 2)
cat > toad.in << 'EOF'
......
..OOO.
.OOO..
......
x
p
s p
s p
s p
s p
EOF
gen $SAMPLE toad

# Beacon (period 2)
cat > beacon.in << 'EOF'
......
.OO...
.OO...
...OO.
...OO.
......
x
p
s p
s p
s p
EOF
gen $SAMPLE beacon

# ── Spaceships ───────────────────────────────
# Glider — moves diagonally
cat > glider.in << 'EOF'
..........
..O.......
...OO.....
..OO......
..........
..........
..........
..........
x
p
s s s s p
s s s s p
s s s s p
s s s s p
EOF
gen $SAMPLE glider

# ── Edge / boundary handling ─────────────────
# Single live cell — dies (underpopulation)
cat > single_cell.in << 'EOF'
.....
..O..
.....
x
p
s p
EOF
gen $SAMPLE single_cell

# All dead — stays dead
cat > all_dead.in << 'EOF'
.....
.....
.....
x
p
s p
s p
EOF
gen $SAMPLE all_dead

# All alive — overpopulation kills interior, edges may survive
cat > all_alive.in << 'EOF'
OOOO
OOOO
OOOO
OOOO
x
p
s p
s p
EOF
gen $SAMPLE all_alive

# Corner cells (boundary check — no out-of-bounds neighbors)
cat > corners.in << 'EOF'
O...O
.....
.....
.....
O...O
x
p
s p
s p
EOF
gen $SAMPLE corners

# Single-row grid
cat > one_row.in << 'EOF'
.OOO.
x
p
s p
s p
s p
EOF
gen $SAMPLE one_row

# Single-column grid
cat > one_col.in << 'EOF'
.
O
O
O
.
x
p
s p
s p
s p
EOF
gen $SAMPLE one_col

# 1×1 grid — alive (dies)
cat > one_by_one_alive.in << 'EOF'
O
x
p
s p
EOF
gen $SAMPLE one_by_one_alive

# 1×1 grid — dead (stays dead)
cat > one_by_one_dead.in << 'EOF'
.
x
p
s p
EOF
gen $SAMPLE one_by_one_dead

# ── Commands ─────────────────────────────────
# Many steps with p between each
cat > many_steps.in << 'EOF'
.....
.OOO.
.....
x
s p s p s p s p s p s p s p s p s p s p
EOF
gen $SAMPLE many_steps

# p with no prior step (print initial state)
cat > just_print.in << 'EOF'
..OO
.O.O
..OO
x
p
EOF
gen $SAMPLE just_print

# Invalid command characters → stderr
cat > invalid_cmd.in << 'EOF'
..O..
.....
x
p
z
q
x
1
p
EOF
gen $SAMPLE invalid_cmd

# Multiple p commands, no s (grid never changes)
cat > multi_print.in << 'EOF'
.OO.
.OO.
x
p p p
EOF
gen $SAMPLE multi_print

echo "Generated .out/.err files from conways_sample"

# Compile
$GPP -o conways conways.cc
if [[ $? -ne 0 ]]; then
    echo "  COMPILE FAILED for conways.cc"
else
    echo "Compiled conways.cc"
    for t in blinker stasis worker gospers large \
              block beehive toad beacon glider \
              single_cell all_dead all_alive corners \
              one_row one_col one_by_one_alive one_by_one_dead \
              many_steps just_print invalid_cmd multi_print; do
        run_test ./conways "$t"
    done
fi

########################################
# Q2 — INTEGER SETS
########################################
echo ""
echo "======== Q2: INTEGER SETS ========"
cd "$BASEDIR/q2"
chmod +x intSet_sample
SAMPLE=./intSet_sample

# ── Provided test ─────────────────────────────
gen $SAMPLE addInt

# ── Basic operations ──────────────────────────
cat > basic_ops.in << 'EOF'
n a 1 2 3 q
n b 3 4 5 q
p a
p b
| a b
& a b
= a b
q
EOF
gen $SAMPLE basic_ops

# ── Subset checks ─────────────────────────────
cat > subset.in << 'EOF'
n a 1 2 3 4 5 q
n b 2 4 q
n c 1 2 6 q
s a b
s a c
s b a
s b b
q
EOF
gen $SAMPLE subset

# ── Subset of empty sets ──────────────────────
cat > subset_empty.in << 'EOF'
n a 1 2 3 q
n b q
s a b
s b a
s b b
q
EOF
gen $SAMPLE subset_empty

# ── Contains ─────────────────────────────────
cat > contains.in << 'EOF'
n a 10 20 30 40 50 q
c a 10
c a 30
c a 50
c a 0
c a 99
c a -1
q
EOF
gen $SAMPLE contains

# ── Remove ────────────────────────────────────
cat > remove.in << 'EOF'
n a 1 2 3 4 5 q
p a
r a 3
p a
r a 1
p a
r a 5
p a
r a 99
p a
q
EOF
gen $SAMPLE remove

# ── Remove from empty set ─────────────────────
cat > remove_empty.in << 'EOF'
n a q
r a 5
p a
q
EOF
gen $SAMPLE remove_empty

# ── Duplicate adds ────────────────────────────
cat > duplicates.in << 'EOF'
n a 5 5 5 5 q
p a
a a 5
a a 5
p a
a a 6
a a 6
p a
q
EOF
gen $SAMPLE duplicates

# ── Add after remove ─────────────────────────
cat > add_after_remove.in << 'EOF'
n a 1 2 3 q
r a 2
p a
a a 2
p a
a a 4
p a
q
EOF
gen $SAMPLE add_after_remove

# ── Copy constructor (d) ──────────────────────
cat > copy_ctor.in << 'EOF'
n a 1 2 3 q
d b a
p a
p b
a a 99
a b 100
p a
p b
= a b
q
EOF
gen $SAMPLE copy_ctor

# ── Copy assignment (<) ───────────────────────
cat > copy_assign.in << 'EOF'
n a 7 8 9 q
n b 1 2 q
< b a
p a
p b
= a b
a b 42
p a
p b
= a b
q
EOF
gen $SAMPLE copy_assign

# ── Self copy-assignment ──────────────────────
cat > self_assign.in << 'EOF'
n a 5 10 15 q
< a a
p a
q
EOF
gen $SAMPLE self_assign

# ── Move constructor (m) ──────────────────────
cat > move_ctor.in << 'EOF'
n a 10 20 30 q
m b a
p b
q
EOF
gen $SAMPLE move_ctor

# ── Move assignment (v) ───────────────────────
cat > move_assign.in << 'EOF'
n a 100 200 300 q
n b 1 q
v b a
p b
q
EOF
gen $SAMPLE move_assign

# ── Self-operations (same index for both args) ─
cat > self_ops.in << 'EOF'
n a 3 6 9 q
| a a
& a a
= a a
s a a
q
EOF
gen $SAMPLE self_ops

# ── Empty set operations ──────────────────────
cat > empty_ops.in << 'EOF'
n a q
n b q
p a
p b
= a b
| a b
& a b
s a b
s b a
c a 0
r a 0
q
EOF
gen $SAMPLE empty_ops

# ── Union with empty ──────────────────────────
cat > union_empty.in << 'EOF'
n a 1 2 3 q
n b q
| a b
| b a
& a b
& b a
= a b
q
EOF
gen $SAMPLE union_empty

# ── Array doubling (forces 4→8→16→32 doublings) ─
cat > grow.in << 'EOF'
n a 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 q
p a
c a 1
c a 25
c a 26
r a 13
p a
c a 13
q
EOF
gen $SAMPLE grow

# ── Negative numbers ─────────────────────────
cat > negatives.in << 'EOF'
n a -5 -3 -1 0 1 3 5 q
p a
c a -3
c a -4
r a -5
p a
| a a
& a a
q
EOF
gen $SAMPLE negatives

# ── Large union / intersection ────────────────
cat > large_ops.in << 'EOF'
n a 1 3 5 7 9 11 13 15 17 19 q
n b 2 4 6 8 10 12 14 16 18 20 q
n c 5 10 15 20 25 q
| a b
& a b
& a c
| b c
= a b
= a a
s a c
s c a
q
EOF
gen $SAMPLE large_ops

# ── Rebuild set after move (n after m) ───────
cat > move_then_new.in << 'EOF'
n a 1 2 3 q
m b a
n a 7 8 9 q
p a
p b
= a b
q
EOF
gen $SAMPLE move_then_new

# ── Invalid commands → stderr ─────────────────
cat > invalid_cmd.in << 'EOF'
n a 1 2 3 q
p a
z
! a b
q
EOF
gen $SAMPLE invalid_cmd

# ── Chain: add, remove, copy, modify, compare ─
cat > chain.in << 'EOF'
n a 10 20 30 40 50 q
d b a
r a 30
a b 60
p a
p b
= a b
| a b
& a b
s a b
s b a
< a b
= a b
q
EOF
gen $SAMPLE chain

echo "Generated .out/.err files from intSet_sample"

# Compile
$GPP -o intSet main.cc intSet.cc
if [[ $? -ne 0 ]]; then
    echo "  COMPILE FAILED for intSet"
else
    echo "Compiled intSet"
    for t in addInt basic_ops subset subset_empty contains \
              remove remove_empty duplicates add_after_remove \
              copy_ctor copy_assign self_assign \
              move_ctor move_assign self_ops \
              empty_ops union_empty grow negatives \
              large_ops move_then_new invalid_cmd chain; do
        run_test ./intSet "$t"
    done
fi

########################################
echo ""
echo "========================================"
echo "  Results: $PASS passed, $FAIL failed"
echo "========================================"
