#!/bin/sh

csv_names=(B KB MB)

mkdir data/${1}

# 1B - 1023B, 1KB - 1023KB, 1MB - 1023MB
for (( j = 0; j < 10; j++)) 
{
    l=$((1))
    for (( i = 1; i <= 3; i++)) 
    {
        echo "l is $l:"
        echo "filename is data/${1}/${csv_names[$((i - 1))]}_${j}.csv"
        echo "${csv_names[$((i - 1))]},Read Time (ns),Read Time / Number (ns)" > data/${1}/${csv_names[$((i - 1))]}_${j}.csv
        for (( n = 1; n < 2048; n++)) 
        {
            ./src/cachetest $n $l >> data/${1}/${csv_names[$((i - 1))]}_${j}.csv
            if [ ${csv_names[$((i - 1))]} == "MB" -a $n == 128 ] 
            then
                break
            fi
            
        }
        echo "\tn = $n done!"
        l=$((l * 1024))
    }
}
