#include "query.h"

namespace nullmax {
namespace msgbag {

Query::Query(QueryFunc &query, const Timestamp start_time,
             const Timestamp end_time)
    : query_(query), start_time_(start_time), end_time_(end_time) {}

QueryFunc const &Query::GetQuery() const { return query_; }

Timestamp const &Query::GetStartTime() const { return start_time_; }
Timestamp const &Query::GetEndTime() const { return end_time_; }

TopicQuery::TopicQuery(std::string const &topic) { topics_.push_back(topic); }

TopicQuery::TopicQuery(std::vector<std::string> const &topics)
    : topics_(topics) {}

bool TopicQuery::operator()(std::string const &topic) const {
  for (const auto &it : topics_)
    if (topic == it)
      return true;

  return false;
}

BagQuery::BagQuery(Bag const *bag, Query const &query, uint32_t bag_revision)
    : bag(bag), query(query), bag_revision(bag_revision) {}

} // namespace msgbag
} // namespace nullmax