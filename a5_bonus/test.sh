#!/bin/bash

# Define colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Compiling all map variations...${NC}"
bash buildExecs

if [ $? -ne 0 ]; then
  echo -e "${RED}Compilation failed! Please check your syntax.${NC}"
  exit 1
fi
echo -e "${GREEN}Compilation successful!${NC}\n"

# Array of all types
types=("int" "str" "float")

# Keep track of overall success
all_passed=true

for key in "${types[@]}"; do
  for val in "${types[@]}"; do
    # Capitalize first letter of val
    valCap="$(tr '[:lower:]' '[:upper:]' <<< ${val:0:1})${val:1}"
    
    execName="${key}To${valCap}"
    sampleExec="samples/${execName}Sample"
    testInput="samples/${execName}SimpleTest.in"

    if [ ! -f "${execName}" ]; then
      echo -e "${RED}Executable ${execName} not found.${NC}"
      continue
    fi

    if [ ! -f "${sampleExec}" ]; then
      echo -e "${YELLOW}Sample executable ${sampleExec} not found. Skipping...${NC}"
      continue
    fi

    # Ensure the sample executable has execute permissions
    chmod +x "${sampleExec}"

    echo -e "${YELLOW}--- Testing ${execName} ---${NC}"

    # 1. Test correctness
    ./${sampleExec} < ${testInput} > sample_out.txt 2> sample_err.txt
    ./${execName} < ${testInput} > my_out.txt 2> my_err.txt

    diff -q sample_out.txt my_out.txt > /dev/null
    if [ $? -eq 0 ]; then
      echo -e "  [${GREEN}PASS${NC}] Output matches sample"
    else
      echo -e "  [${RED}FAIL${NC}] Output differs from sample!"
      echo -e "  ${YELLOW}Sample output:${NC}"
      cat sample_out.txt
      echo -e "  ${YELLOW}Your output:${NC}"
      cat my_out.txt
      all_passed=false
    fi

    # 2. Test memory leaks using valgrind
    if command -v valgrind &> /dev/null; then
      valgrind --leak-check=full --error-exitcode=1 ./${execName} < ${testInput} > /dev/null 2> valgrind_err.txt
      if [ $? -eq 0 ]; then
        echo -e "  [${GREEN}PASS${NC}] Valgrind memory check"
      else
        echo -e "  [${RED}FAIL${NC}] Memory leak or error detected!"
        cat valgrind_err.txt
        all_passed=false
      fi
    else
      echo -e "  [${YELLOW}SKIP${NC}] Valgrind not installed, skipping memory check."
    fi

    echo ""
  done
done

# Cleanup temporary files
rm -f sample_out.txt sample_err.txt my_out.txt my_err.txt valgrind_err.txt

if [ "$all_passed" = true ]; then
  echo -e "${GREEN}==========================================${NC}"
  echo -e "${GREEN}All tests passed! You are ready to submit!${NC}"
  echo -e "${GREEN}==========================================${NC}"
else
  echo -e "${RED}==========================================${NC}"
  echo -e "${RED}Some tests failed. Please review the logs.${NC}"
  echo -e "${RED}==========================================${NC}"
fi
