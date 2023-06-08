#include <deque>
#include <iostream>
#include <mutex>
#include <algorithm>    // std::find

#include "mru.h"

using namespace std;

/// my_mru maintains a listing of the K most recent elements that have been
/// given to it.  It can be used to produce a "top" listing of the most recently
/// accessed keys.
class my_mru : public mru_manager {
  std::recursive_mutex lock; // need mutex to protect the deque
  std::deque<std::string> deque;  // the deque
  size_t max_size; // the max size of the deque

public:
  /// Construct the mru_manager by specifying how many things it should track
  ///
  /// @param elements The number of elements that can be tracked
  my_mru(size_t elements) {
    max_size = elements; // max size of the deque
    deque = std::deque<std::string>(); // initialize the deque
  }

  /// Destruct the mru_manager
  virtual ~my_mru() {
    deque.clear();
  }

  /// Insert an element into the mru_manager, making sure that (a) there are no
  /// duplicates, and (b) the manager holds no more than /max_size/ elements.
  ///
  /// @param elt The element to insert
  virtual void insert(const std::string &elt) {
    std::lock_guard<std::recursive_mutex> guard(lock);
    remove(elt);
    if (deque.size() >= max_size) { // need to check if mru is full, if is we kill last element
      deque.pop_back(); // remove the last element
      deque.push_front(elt); // push new element to the front
    }
    else deque.push_front(elt); // push new element to the front
  }

  /// Remove an instance of an element from the mru_manager.  This can leave the
  /// manager in a state where it has fewer than max_size elements in it.
  ///
  /// @param elt The element to remove
  virtual void remove(const std::string &elt) {
    std::lock_guard<std::recursive_mutex> guard(lock);
    auto it = std::find(deque.begin(), deque.end(), elt); // get iterator of the first thing in deque == elt
    if (it != deque.end()) deque.erase(it); // remove if not end
  }

  /// Clear the mru_manager
  virtual void clear() {
    std::lock_guard<std::recursive_mutex> guard(lock);
    deque.clear();
  }

  /// Produce a concatenation of the top entries, in order of popularity
  ///
  /// @return A newline-separated list of values
  virtual std::string get() { 
    std::lock_guard<std::recursive_mutex> guard(lock);
    std::string result;
    for (auto it = deque.begin(); it != deque.end(); ++it) { // for each element in deque
      result.append(*it); // append the element to the result
      if (it != deque.end() - 1) result.append("\n"); // append a newline if not the last element
    }
    return result;
  }
};

/// Construct the mru_manager by specifying how many things it should track
///
/// @param elements The number of elements that can be tracked in MRU fashion
///
/// @return An mru manager object
mru_manager *mru_factory(size_t elements) { return new my_mru(elements); }