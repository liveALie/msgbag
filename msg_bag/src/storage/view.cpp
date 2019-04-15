#include "view.h"
#include "common/msgbag_exception.h"

namespace nullmax {
namespace msgbag {

void View::AddQuery(const Bag &bag, Timestamp start_time, Timestamp end_time) {
  if ((bag.GetMode() & BagMode::Read) != BagMode::Read)
    throw MsgbagException("Bag not opened for reading");
  QueryFunc query = TrueQuery();
  std::shared_ptr<BagQuery> bag_query = std::make_shared<BagQuery>(
      &bag, Query(query, start_time, end_time), bag.version_);
  bag_querys_.push_back(bag_query);
  UpdateQueries(bag_query);
}

void View::AddQuery(const Bag &bag, QueryFunc query, Timestamp start_time,
                    Timestamp end_time) {
  if ((bag.GetMode() & BagMode::Read) != BagMode::Read)
    throw MsgbagException("Bag not opened for reading");
  std::shared_ptr<BagQuery> bag_query = std::make_shared<BagQuery>(
      &bag, Query(query, start_time, end_time), bag.version_);
  bag_querys_.push_back(bag_query);
  UpdateQueries(bag_query);
}

Timestamp View::GetBeginTime() { return filtering_records_.begin()->first; }

Timestamp View::GetEndTime() { return filtering_records_.rbegin()->first; }

size_t View::Size() { return filtering_records_.size(); }

const BagRecordsType &View::GetRecords() const { return filtering_records_; }

void View::UpdateQueries(std::shared_ptr<BagQuery> bag_query) {
  for (auto it : bag_query->bag->records_) {
    if (!bag_query->query.GetQuery()(it.second->topic_))
      continue;
    filtering_records_.insert({it.first, it.second});
  }
}

} // namespace msgbag
} // namespace nullmax