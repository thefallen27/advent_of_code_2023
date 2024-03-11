# advent_of_code_2023
Solutions to the advent of code challenge for the year 2023!

# notes
Day 6
------
The second versions embed the calculation of the number of ways directly into the main loop. By doing so, it avoids the overhead of function calls and makes the code shorter, though at the cost of readability and maintainability.

Day 8
-----
Using:
https://github.com/hanickadot/compile-time-regular-expressions

Optimizations made:
String Storage: Store each unique string in a separate map (nodes) to avoid duplicating strings.
Pointers to Strings: The network map now holds pointers to strings, reducing memory usage and avoiding string copies.
Efficient Looping: Cached the size of instructions and used pointers to manipulate current nodes, reducing the overhead of string comparisons.

These changes improve the performance of the program, particularly in reducing memory allocations and enhancing string handling efficiency. However, the actual performance gain can vary based on the specifics of the input data and the execution environment.

Day 16
------
For the second part, I tried to make it as fast as possible. I even defined a custom hash function. It didn't help. It is still a bit slower than expected.

Day 20
------
This was the first time I didn't see any benefit on doing the two parts seperately. The second part had very little changes compared to the first part and I realised that much of the code for the second was the same as the first part. So I bundled the two together. I tried to make it as modular as possible (to separate the two parts) using all the techniques I know. It is readable, but I couldn't make it any faster. It is still a bit slow.