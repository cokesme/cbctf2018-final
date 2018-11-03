```
$ node carve.js

# These can be parallelized
$ cargo run --release 0 > result0.txt
$ cargo run --release 1 > result1.txt
$ cargo run --release 2 > result2.txt
$ cargo run --release 3 > result3.txt
$ cargo run --release 4 > result4.txt

$ ruby set.rb > memo.txt
```
