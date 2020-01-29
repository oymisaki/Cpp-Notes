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

**boost::flat_map -- Faster than O(1)?**
查找和遍历更快，插入和删除更慢.更少的内存。缓存优化。`Cache Locality` 问题，计算机多级存储，`L1` 最快，由于`vector`是连续的，因此可以一次性载入缓存，使得访问其他元素会更快一些。而对于 `linked list`这样的结构，访问会要慢很多。
>+ Faster lookup than standard associative containers
>+ Much faster iteration than standard associative containers
>+ Less memory consumption for small objects (and for big objects if shrink_to_fit is used)
>+ Improved cache performance (data is stored in contiguous memory)
>+ Non-stable iterators (iterators are invalidated when inserting and erasing elements)
>+ Non-copyable and non-movable values types can't be stored
>+ Weaker exception safety than standard associative containers (copy/move constructors can throw when shifting values in erasures and insertions)
>+ Slower insertion and erasure than standard associative containers (specially for non-movable types)

## Iterators

+ `back_inserter()`. push_back functionalities
+ `inserter(vector, vector.end())`.
+ `ostream_iterator<string> os(cout, " ")`
+ `distance(first, last)` 返回距离
+ `advance(first, steps)` 前移


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
+ `for_each(first, last, function)`. 注意 `std::mem_fn`的使用
  ```cpp
  std::for_each(threads.begin(),threads.end(),
       std::mem_fn(&std::thread::join));
  ```
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
+ bind. 可使用 `_1 placeholder` 
  ```cpp
  void f(int n1, int n2, int n3, const int& n4, int n5)
  {
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
  }

  auto f1 = std::bind(f, _2, 42, _1, std::cref(n), n);
  n = 10;
  f1(1, 2, 1001); // 1 为 _1 所绑定， 2 为 _2 所绑定，不使用 1001
                  // 进行到 f(2, 42, 1, n, 7) 的调用
  ```
  其中, `_1` 是参数位置占位符, `std::ref`, `std::cref` 是一种 `reference wrapper`, 
  因为 `bind` 的参数默认传值。