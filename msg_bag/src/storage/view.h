#pragma once

#include "bag.h"
#include "query.h"
#include <set>

namespace nullmax {
namespace msgbag {

class View {
public:
  void AddQuery(const Bag &bag, Timestamp start_time = detail::TIME_MIN,
                Timestamp end_time = detail::TIME_MAX);
  void AddQuery(const Bag &bag, QueryFunc query,
                Timestamp start_time = detail::TIME_MIN,
                Timestamp end_time = detail::TIME_MAX);
  Timestamp GetBeginTime();
  Timestamp GetEndTime();

  size_t Size();

  const BagRecordsType &GetRecords() const;

private:
  void UpdateQueries(std::shared_ptr<BagQuery> bag_query);

private:
  BagRecordsType filtering_records_;
  std::vector<std::shared_ptr<BagQuery>> bag_querys_;
};
} // namespace msgbag
} // namespace nullmax