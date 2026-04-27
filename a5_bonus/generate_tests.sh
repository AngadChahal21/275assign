#!/bin/bash

# Arrays of sample keys and values for each type
# 0 = int, 1 = str, 2 = float

key_0=("10" "5" "20" "15" "25")
key_1=("hello" "apple" "zebra" "mango" "zoo")
key_2=("10.5" "5.5" "20.5" "15.5" "25.5")

val_0=("100" "50" "200" "150" "250")
val_1=("one_hundred" "fifty" "two_hundred" "one_fifty" "two_fifty")
val_2=("100.9" "50.9" "200.9" "150.9" "250.9")

types=("int" "str" "float")

for k in 0 1 2; do
  for v in 0 1 2; do
    kName=${types[$k]}
    vName=${types[$v]}
    # Capitalize vName
    vNameCap="$(tr '[:lower:]' '[:upper:]' <<< ${vName:0:1})${vName:1}"
    
    execName="${kName}To${vNameCap}"
    outFile="samples/${execName}ExtensiveTest.in"

    # We need to evaluate the variable name to get the array
    eval "kArr=(\"\${key_${k}[@]}\")"
    eval "vArr=(\"\${val_${v}[@]}\")"

    cat <<EOF > $outFile
p
s ${kArr[0]} ${vArr[0]}
s ${kArr[1]} ${vArr[1]}
s ${kArr[2]} ${vArr[2]}
p
r ${kArr[1]}
r ${kArr[0]}
r ${kArr[2]}
r ${kArr[3]}
p
f ${kArr[1]}
f ${kArr[4]}
p
s ${kArr[1]} ${vArr[2]}
p
s ${kArr[4]} ${vArr[0]}
r ${kArr[4]}
f ${kArr[3]}
p
q
EOF
    echo "Generated $outFile"
  done
done
