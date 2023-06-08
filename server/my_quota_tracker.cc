// http://www.cplusplus.com/reference/ctime/time/ is helpful here
#include <deque>
#include <iostream>
#include <memory>
#include <ctime>

#include "quota_tracker.h"

using namespace std;

/// quota_tracker stores time-ordered information about events.  It can count
/// events within a pre-set, fixed time threshold, to decide if a new event can
/// be allowed without violating a quota.
class my_quota_tracker : public quota_tracker {

public:
  size_t currentSize = 0;
  size_t maxSize;
  double time;
  deque<pair<size_t, double>> q;
  /// Construct a tracker that limits usage to quota_amount per quota_duration
  /// seconds
  ///
  /// @param amount   The maximum amount of service
  /// @param duration The time over which the service maximum can be spread out
  my_quota_tracker(size_t amount, double duration) {
    maxSize = amount;
    time = duration;
  }

  /// Destruct a quota tracker
  virtual ~my_quota_tracker() {}

  /// Decide if a new event is permitted, and if so, add it.  The attempt is
  /// allowed if it could be added to events, while ensuring that the sum of
  /// amounts for all events within the duration is less than q_amnt.
  ///
  /// @param amount The amount of the new request
  ///
  /// @return false if the amount could not be added without violating the
  ///         quota, true if the amount was added while preserving the quota
  virtual bool check_add(size_t amount) {
    time_t curTime; 
    std::time(&curTime);
    while(q.size() > 0){
      if (curTime - q.front().second > time){
        currentSize -= q.front().first;
        q.pop_front();
      }else{
        break;
      } 
    }
    if(amount + currentSize > maxSize){
      return false;
    }else{
      pair<size_t, double> newEntry(amount, curTime);
      q.push_back(newEntry);
      currentSize += amount;
      return true; 
    }

  }
};

/// Construct a tracker that limits usage to quota_amount per quota_duration
/// seconds
///
/// @param amount   The maximum amount of service
/// @param duration The time over which the service maximum can be spread out
quota_tracker *quota_factory(size_t amount, double duration) {
  return new my_quota_tracker(amount, duration);
}