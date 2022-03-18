#!/usr/bin/expect

clang++ -Wall -Wextra -Werror bot_client.cpp -o bot_anton

i=0
num=5; # количество ботов
sum=80; # начальный порт, после ++

for (( ; i < num; i++, sum++))
do
  ./bot_anton anton $i $sum &
done