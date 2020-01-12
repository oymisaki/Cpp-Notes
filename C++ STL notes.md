# STL

## what's predicate in c++?

**basically a callable entity which is either a function, or a functor (or a lambda, which is essentially a functor or function depending on whether it captures variable(s) or not).**

+ lambda function. `std::find_if(begin(v), end(v), [](int i) { return i == 3; })`
+ function. 
  ```c++
  bool is_three(int i) { return i == 3; }

  std::vector<int>::iterator it = std::find_if(v.begin(), v.end(), is_three);
  ``` 
+ functor. `greater<int>()`

## Algorithm

**all the algorithms changes inplace**

### Search

+ `iter find(first, last, val)`
+ `iter find_if(first, last, pred)`
+ `iter binary_search(forwardFirst, forwardLast, val, comp)`

### Sorting
+ `merge(f1, l1, f2, l2, outputIter, comp)` merge two sorted array
+ `partition (myvector.begin(), myvector.end(), IsOdd);` partion inplace and return a iterator
+ `reverse()` inplace
+ `rotate (first, middle, last)` inplace. Middle becomes the new first.
+ `sort()`

### Copy, Deletion, Substitution
+ `Copy(first, last, back_inserter())`
+ `remove(first, last, val)` return a new end. the removed values are preserved.
+ `remove_if(first, last, pred)`
+ `replace(first, last, old, new)`
+ `unique(first, last, pred)` or `unique(first, last)`

### Relational
+ `equal(first, last, first1)`. if euqal in range.

### Generation, Mutation
+ `generate(first, last, gen)`, Eg. `generate (myvector.begin(), myvector.end(), RandomNumber)`
+ `for_each(first, last, function)`
+ `transform(first, last, function)`

### Numeric
+ `accumulate(first, last, initial)`
+ `adjacent_difference(first, last, outputIter)`
+ `partial_sum(first, last, outputIter)`

### Set
+ `set_union()`
+ `set_difference()`