# A Fast CSR matrix loader (`mtx` format)

## Build

1. With [QuickProject](https://github.com/Rhythmicc/QuickProject)

   ```shell
    qrun -b
   ```

2. With gcc:

   ```shell
    g++-11 -O3 main.cpp -IFastCSRLoader -o dist/test
   ```

## Test

1. With [QuickProject](https://github.com/Rhythmicc/QuickProject)

   ```shell
    qrun dist/input.txt
   ```

2. With gcc:

   ```shell
    ./dist/test ./dist/input.txt
   ```

### Input

```
%%MatrixMarket matrix coordinate real general
%-------------------------------------------------------------------------------
10 10 5
1 1 2e3
1 2 2e3
2 4 1e2
3 4 1e2
5 6 1e3
```

### Output

```
0       2       3       4       4       5       5       5       5       5
0       1       3       3       5
2000    2000    100     100     1000
```
