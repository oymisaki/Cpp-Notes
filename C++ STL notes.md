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

## Collections

### Vector
+ push_back
+ assign. **mind the difference between copy**

### list
+ splice. 
  **This effectively inserts those elements into the container and removes them from x, altering the sizes of both containers.**
  ```cpp
  // entire list (1)	
  void splice (iterator position, list& x);
  //single element (2)	
  void splice (iterator position, list& x, iterator i);
  //element range (3)	
  void splice (iterator position, list& x, iterator first, iterator last);
  ```
  **Iterator Validaty**
  after the splice operation, the `i` iterator and value it points to will not change, it means, the iterator will not be invalidated. Moreover, there is no copy and construction, there are only the changes about the pointers.  
  ```cpp
  list<int> l;
  l.push_back(1);    
  l.push_front(2);

  auto e = ++l.begin();
  cout <<*e<<endl; // 1
  l.erase(l.begin());
  cout << *e<<endl; // 1

  cout << (++e == l.end()); // true
  ```

+ push_back, pop_back
+ emplace_back, emplace_front
+ `front()` or `list.back()` 直接返回最后一个元素
+ assign. `l1.assign(l2.begin(), l2.last())`

**Difference between assign and copy**

copy can only be used in copy between containers that are the same types.
but `list.assign()` works between different types as long as they support assignment
operator. 

```cpp
list<A> l1;
list<B> l2;
l1.assign(l2.begin(), l2.end());
```


### Map

+ `Map[key]` this operation will defaultly add key to map
+ `map<string, int>` construction.
+ `map.find()`
+ `map.erase()`

## Iterators

+ `back_inserter()`. push_back functionalities
+ `inserter(vector, vector.end())`.
+ `ostream_iterator<string> os(cout, " ")`

### stringstream

```cpp
std::stringstream buffer;
buffer << "#";
std::copy(values.begin(), values.end(), std::ostream_iterator<int>(buffer, "#"));

cout<<buffer.str()<<endl;
```


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

## Function Objects

### Arithmetic
+ plus
+ minus
+ negate
+ multiplies
+ divides
+ modules

### Relational
+ less
+ than
+ equal_to
+ greater_equal
+ less_equal
+ not_equal_to

### Logical
+ logical_and
+ logical_or
+ logical_not

## Function Objects Adapters

+ bind2nd
+ not1, negate adapter
