# Reed-Solomon on Composite Field
This projects implements a composite Reed Solomon coder based on GF((2^8)^2) in C.
The sample function in main.c shows an example of usage.

- To use it in your own project, follow the following steps:

1. Include header:
```c
#include "comp_rscoding.h"
```

2. Create Table
Create a pre-computed table in GF(2^8) by:
```c
struct Tables *tables = newTables(285, 8);
```

3. Prepare message:
```c
Put the message of length k that you would like to encode as an array, and create a message polynomial by:
struct Polynomial *mesecc_poly = newPolynomial(message_arr, k);
```

4. Encode:
```c
struct Polynomial *encoded_mesecc_poly = rs_encode_msg(mesecc_poly, n - k, tables);
```

5. Decode:
```c
struct Polynomial *corrected_message_poly = rs_correct_msg(encoded_mesecc_poly, n - k, tables, 16);
```

6. If correction is possible, the above function will return a corrected message in"
```c
corrected_message_poly->poly_arr
```
If correction is possible when the message is too long or if there are too many errors, an error message will show up and the program will terminate.

- To print a message from a polynomial:
```c
printPolynomial(corrected_message_poly);
```

- To run the sample, which encodes and decodes RS(12000, 10000):
# compile
make

# run
./main
