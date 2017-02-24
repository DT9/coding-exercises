#usage: ./uva.sh [folder] [uva problem no.]
folder=$1
num=$2
in=UVa${num}.cpp 
out=${num}.out
input=${num}.in
g++ -Wall -Wextra -g -std=c++11 -pipe -DONLINE_JUDGE -stdlib=libc++ -lm -lcrypto $folder/$in -o $folder/$out
./$folder/$out < $folder/$input
