# tokenc

Currently this repo is for exploring/learning/playing around with tokenization.
Implementation done in C.

The current state is able to parse words, ints, floats and other symbols.

## Quick start

Run example 
```{.sh}
cc -o example example.c && ./example
```

Run json formatter

```{.sh}
cc -o pj pj.c
printf '{"test1": [1,2,3,4.4], "test2": {"lala": 1, "po": " }{][{hj}{hei: 1}[1,,3,4,5]"    }, "test3": "https://localhost:8080", "test4": 5, 	"test5": 5.5 , "test6":     [{"lala": 1},{"lala2": 2}}' | ./pj
cat poke.json | ./pj
./pj < poke.json
./pj poke.json
./pj # for REPL like json formatter
```


## Author

Meelis Utt
