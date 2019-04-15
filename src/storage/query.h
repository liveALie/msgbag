#pragma once

#include "common/timestamp.h"
#include <functional>
#include <string>
#include <vector>

namespace nullmax {
namespace msgbag {

typedef std::function<bool(const std::string &)> QueryFunc;

class Query {
public:
  //! The base query takes an optional time-range
  /*!
   * param start_time the beginning of the time_range for the query
   * param end_time   the end of the time_range for the query
   */
  Query(QueryFunc &query, const Timestamp start_time, const Timestamp end_time);

  QueryFunc const &GetQuery() const; //!< Get the query functor

  Timestamp const &GetStartTime() const; //!< Get the start-time
  Timestamp const &GetEndTime() const;   //!< Get the end-time

private:
  QueryFunc query_;
  Timestamp start_time_;
  Timestamp end_time_;
};

class TopicQuery {
public:
  TopicQuery(std::string const &topic);
  TopicQuery(std::vector<std::string> const &topics);

  bool operator()(std::string const &) const;

private:
  std::vector<std::string> topics_;
};

class Bag;

struct BagQuery {
  BagQuery(Bag const *bag, Query const &query, uint32_t bag_revision);

  Bag const *bag;
  Query query;
  uint32_t bag_revision;
};

struct TrueQuery {
  bool operator()(const std::string &) const { return true; };
};

} // namespace msgbag
} // namespace nullmax