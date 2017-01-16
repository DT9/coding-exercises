#usage: ./uva.sh [uva problem no.]
num=$1
in=UVa${num}.cpp 
out=${num}.out
input=${num}.in
g++ -Wall -Wextra -O2 -std=c++11 -pipe -DONLINE_JUDGE -stdlib=libc++ -lm -lcrypto $in -o $out
./$out < $input