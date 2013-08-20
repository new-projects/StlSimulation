#ifndef ACM_VECTOR_H
#define ACM_VECTOR_H

#include <memory>
#include <algorithm>

namespace acm {

template <typename T, typename Alloc = std::allocator<T> >
class vector {
 public:
  typedef Alloc allocator_type;
  typedef T value_type;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef typename allocator_type::pointer iterator;
  typedef typename allocator_type::const_pointer const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef typename allocator_type::difference_type difference_type;
  typedef typename allocator_type::size_type size_type;

 private:
  static allocator_type static_allocator;
  iterator start;
  iterator finish;
  iterator end_of_storage;

  void destroy(iterator first, iterator last) {
    while (first != last) {
      static_allocator.destroy(first);
      ++first;
    }
  }

  void insert_aux(iterator position, const T& x) {
    if (finish != end_of_storage) {
      std::copy_backward(position, finish, finish + 1);
      *position = x;
      ++finish;
    } else {
      size_type len = size() ? 2 * size() : 1;
      iterator tmp = static_allocator.allocate(len);
      uninitialized_copy(begin(), position, tmp);
      static_allocator.construct(tmp + (position - begin()), x);
      uninitialized_copy(position, end(), tmp + (position - begin() + 1));
      destroy(begin(), end());
      static_allocator.deallocate(begin(), capacity());
      end_of_storage = tmp + len;
      finish = tmp + size() + 1;
      start = tmp;
    }
  }

 public:
  iterator begin() { return start; }

  const_iterator begin() const { return start; }

  iterator end() { return finish; }

  const_iterator end() const { return finish; }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const { return const_reverse_iterator(end()); }

  size_type size() const { return end() - begin(); }

  size_type max_size() const { return static_allocator.max_size(); }

  size_type capacity() const { return end_of_storage - begin(); }

  bool empty() const { return begin() == end(); }

  reference operator[](size_type n) { return *(begin() + n); }

  const_reference operator[](size_type n) const { return *(begin() + n); }

  vector() : start(0), finish(0), end_of_storage(0) {}

  vector(size_type n, const T& value = T()) {
    start = static_allocator.allocate(n);
    std::uninitialized_fill_n(start, n, value);
    finish = start + n;
    end_of_storage = finish;
  }

  vector(const vector<T>& x) {
    start = static_allocator.allocate(x.end() - x.begin());
    finish = uninitialized_copy(x.begin, x.end(), start);
    end_of_storage = finish;
  }

  vector(const_iterator first, const_iterator last) {
    size_type n = last - first;
    start = static_allocator.allocate(n);
    finish = uninitialized_copy(first, last, start);
    end_of_storage = finish;
  }

  ~vector() {
    destroy(begin(), end());
    static_allocator.deallocate(begin(), capacity());
  }

  void reserve(size_type n) {
    if (capacity() < n) {
      iterator tmp = static_allocator.allocate(n);
      uninitialized_copy(begin(), end(), tmp);
      destroy(begin(), end());
      static_allocator.deallocate(begin());
      finish = tmp + size();
      start = tmp;
      end_of_storage = begin() + n;
    }
  }

  reference front() { return *begin(); }

  const_reference front() const { return *begin(); }

  reference back() { return *(end() - 1); }

  const_reference back() const { return *(end() - 1); }

  void push_back(const T& x) {
    if (finish != end_of_storage) {
      static_allocator.construct(finish, x);
      ++finish;
    } else {
      insert_aux(end(), x);
    }
  }

  void swap(vector<T>& x) {
    std::swap(start, x.start);
    std::swap(finish, x.finish);
    std::swap(end_of_storage, x.end_of_storage);
  }

  void insert(iterator position, size_type n, const T& x) {
    if (n == 0)
      return;
    if (end_of_storage - finish >= n) {
      std::uninitialized_copy(end() - n, end(), end());
      std::copy_backward(position, end - n, end());
      std::fill(position, position + n, x);
    } else {
    }
  }
};

template <typename T, typename Alloc>
typename vector<T, Alloc>::allocator_type vector<T, Alloc>::static_allocator;

}  // namespace acm

#endif
