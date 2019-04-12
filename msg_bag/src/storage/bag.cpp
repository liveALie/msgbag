#include "bag.h"
#include "common/msgbag_constants.h"
#include "common/msgbag_exception.h"
#include "common/util.h"
#include "geometry_msgs.pb.h"
#include "log/logger.h"

namespace nullmax {
namespace msgbag {

Bag::Bag() { Init(); }

Bag::Bag(const std::string &filename, uint32_t mode) {
  Init();
  Open(filename, mode);
}

Bag::~Bag() {}
void Bag::SetCompression(CompressionType compression) {
  compression_ = compression;
}

void Bag::SetChunkThreshold(int chunk_threshold) {
  chunk_threshold_ = chunk_threshold;
}

void Bag::Open(const std::string &filename, uint32_t mode) {
  mode_ = (BagMode)mode;
  // check if already open a file.
  if (filestream_.is_open()) {
    throw MsgbagException(std::string("File already open ").append(filename_));
  }

  if (mode & BagMode::Append) {
    OpenReadWrite(filename);
  } else if (mode_ & BagMode::Write) {
    OpenWrite(filename);
  } else if (mode_ & BagMode::Read) {
    OpenRead(filename);
  } else {
    throw MsgbagException(
        std::string("Unknown mode:").append(std::to_string(mode)));
  }
  if (!filestream_)
    throw MsgbagException(std::string("bag open file error.").append(filename));
  filename_ = filename;
  UpdateFileInfo();
}

void Bag::OpenReadWrite(const std::string &filename) {
  // check if file exist.
  filestream_.open(filename, std::ios::in);
  if (!filestream_) {
    throw MsgbagException(std::string("File not exist :").append(filename));
  } else {
  }
  filestream_.close();
  filestream_.open(filename, std::ios::in | std::ios::app | std::ios::out |
                                 std::ios::binary);
  if (!filestream_)
    throw MsgbagException("bag openreadwrite open file error.");
  ReadVersion();
  //   if (version_ != 200)
  //     throw MsgbagException(std::string("version:") +
  //     std::to_string(version_) +
  //                           "is unsupported for appending");
}

void Bag::ReadVersion() {
  std::string version_line;
  getline(filestream_, version_line);
  offset_ = filestream_.tellg();

  int version_major, version_minor;
  if (sscanf(version_line.c_str(), "#MSGBAG V%d.%d", &version_major,
             &version_minor) != 2)
    throw MsgbagException("Error reading version line");

  version_ = version_major * 100 + version_minor;

  LOG_DEBUG("Read VERSION: version={}", version_);
}

void Bag::WriteVersion() {
  std::string version = std::string("#MSGBAG V") + VERSION + std::string("\n");

  LOG_DEBUG("Writing VERSION {}: {}", (unsigned long long)offset_, version);

  version_ = std::stof(VERSION) * 100 / 100;

  Write(version);
}

void Bag::WriteHeaderRecord() {
  HeaderType header;
  header[OP_FIELD_NAME] = HexToString(FILE_HEADER);
  header[START_TIMESTAMP_FIELD_NAME] = std::to_string(start_timestamp_);
  header[END_TIMESTAMP_FIELD_NAME] = std::to_string(end_timestamp_);
  header[MSG_COUNT_FIELD_NAME] = std::to_string(msg_count_);
  Buffer buffer(1024);
  uint32_t header_len = WriteHeader(header, buffer);
  uint32_t data_len = 0;
  if (header_len < FILE_HEADER_LEN)
    data_len = FILE_HEADER_LEN - header_len;
  if (data_len > 0) {
    std::string padding;
    padding.resize(data_len, ' ');
    buffer.append(padding);
  }
  LOG_DEBUG("buf readablebytes should be 1028,header_len:{},data_len:{}",
            header_len, data_len);
  LOG_DEBUG("write header record,buf readablebytes:{}", buffer.readableBytes());

  filestream_.write(buffer.peek(), buffer.readableBytes());
  offset_ = filestream_.tellp();
  LOG_DEBUG("write header record:{}", buffer.retrieveAllAsString());
}

void Bag::OpenWrite(const std::string &filename) {
  filestream_.open(filename, std::ios::out | std::ios::binary);
  if (!filestream_)
    throw MsgbagException(
        std::string("bag openreadwrite open file error,filename:")
            .append(filename));
  StartWriting();
}

void Bag::StartWriting() {
  WriteVersion();
  file_header_pos_ = offset_;
  WriteHeaderRecord();
  index_data_pos_ = offset_;
  start_timestamp_ = Time::Now().ToNano();
}

void Bag::OpenRead(const std::string &filename) {
  filestream_.open(filename, std::ios::in | std::ios::binary);
  if (!filestream_)
    throw MsgbagException(std::string("Bag::OpenRead file:")
                              .append(filename)
                              .append(std::string(" failed.")));
  filename_ = filename;
  UpdateFileInfo();
  ReadVersion();
  file_header_pos_ = offset_;
  switch (version_) {
  case 100: {
    StartReadingVersion100();
  } break;
  default:
    throw MsgbagException(std::string("unsuported bag file version:") +
                          std::to_string(version_));
  }
}

void Bag::StartReadingVersion100() {
  ReadHeaderRecord();
  filestream_.seekg(0, std::ios::end);
  file_size_ = filestream_.tellg();

  filestream_.seekg(index_data_pos_);
  if (!filestream_) {
    std::cout << "+++++++++++++++++++++++++++++++++bag file stream is null."
              << std::endl;
  }
  while (offset_ < file_size_)
    ReadTopicIndexRecord100();
  filestream_.seekg(0, std::ios_base::beg);
  if (!filestream_) {
    std::cout << "bag file stream is null." << std::endl;
  }
}

bool Bag::CheckEnoughDataToRead(size_t size) {
  offset_ = filestream_.tellg();
  bool ret = (file_size_ - offset_ > size);
  if (!ret) {
    offset_ = file_size_;
    filestream_.seekg(0, std::ios_base::end);
  }
  return ret;
}

void Bag::ReadTopicIndexRecord100() {
  size_t msg_size = 0;
  long long time_stamp = 0;
  offset_ = filestream_.tellg();
  if (!CheckEnoughDataToRead(sizeof(size_t))) {
    std::cout << "not enough data to read." << std::endl;
    return;
  }
  filestream_.read((char *)&msg_size, sizeof(size_t));
  offset_ = filestream_.tellg();
  if (!CheckEnoughDataToRead(sizeof(long long))) {
    std::cout << "not enough data to read." << std::endl;
    return;
  }
  filestream_.read((char *)&time_stamp, sizeof(long long));
  std::string topic;
  getline(filestream_, topic);
  Buffer buf(msg_size);
  if (!CheckEnoughDataToRead(msg_size)) {
    std::cout << "not enough data to read." << std::endl;
    return;
  }
  filestream_.read(buf.beginWrite(), msg_size);
  buf.hasWritten(msg_size);
  offset_ = filestream_.tellg();
  pb::geometry_msgs::PoseStamped pb_msg;
  pb_msg.ParsePartialFromArray(buf.peek(), msg_size);
  std::cout << "msg size:" << msg_size << ",timestamp:" << time_stamp
            << ",topic:" << topic << ",msg body:" << pb_msg.DebugString();
  fflush(stdout);
}

void Bag::ReadHeaderRecord() {
  LOG_INFO("Bag::ReadHeaderRecord");
  uint32_t total_size = FILE_HEADER_LEN + sizeof(uint32_t);
  std::cout << "total size:" << total_size << std::endl;
  Buffer buffer(total_size);
  filestream_.read(buffer.beginWrite(), total_size);
  buffer.hasWritten(total_size);
  offset_ = filestream_.tellg();
  index_data_pos_ = offset_;
  std::cout << "index data pos:" << index_data_pos_ << std::endl;
  std::cout << "buffer readablebytes should be " << 1028 << std::endl;
  std::cout << "buffer readablebytes:" << buffer.readableBytes() << std::endl;
  HeaderType file_header;
  ReadHeader(file_header, buffer);
  for (auto field : file_header) {
    // LOG_DEBUG("read file header record:{}={}", field.first, field.second);
    std::cout << "read file header record:" << field.first << "="
              << field.second << std::endl;
  }
  LOG_DEBUG("read file header:{}", buffer.toStringPiece());
  offset_ = filestream_.tellg();
  // start_timestamp_ = std::stoll(file_header[START_TIMESTAMP_FIELD_NAME]);
  // end_timestamp_ = std::stoll(file_header[END_TIMESTAMP_FIELD_NAME]);
  // index_data_pos_ = std::stoi(file_header[INDEX_POS_FIELD_NAME]);
  // msg_count_ = std::stoi(file_header[MSG_COUNT_FIELD_NAME]);
}

// void Bag::Seek(uint64_t pos, int origin) {
//   // file_.seek(pos, origin);
// }

void Bag::CloseWrite() { StopWriting(); }

void Bag::StopWriting() {
  filestream_.seekp(0, std::ios::end);
  index_data_pos_ = offset_;
  filestream_.seekp(file_header_pos_);
  end_timestamp_ = Time::Now().ToNano();
  WriteHeaderRecord();
}

void Bag::UpdateFileInfo() {
  if (!filestream_) {
    throw MsgbagException(std::string("Bag file get UpdateFileSize error."));
  }

  offset_ = filestream_.tellp();
  filestream_.seekp(0, std::ios::end);
  file_size_ = filestream_.tellp();
  filestream_.seekp(offset_);
}

void Bag::Init() {
  filename_ = "";
  file_size_ = 0;
  mode_ = BagMode::Write;
  offset_ = 0;
  file_header_pos_ = 0;
  version_ = 0;
  compression_ = CompressionType::Uncompressed;
  chunk_threshold_ = 0;
  index_data_pos_ = 0;
  start_timestamp_ = 0;
  end_timestamp_ = 0;
  msg_count_ = 0;
}

void Bag::Close() {
  if (!filestream_.is_open())
    return;
  if (mode_ & BagMode::Write || mode_ & BagMode::Append) {
    CloseWrite();
  }
  filestream_.close();
  //   topic_connection_ids_.clear();
  //     header_connection_ids_.clear();
  //     for (map<uint32_t, ConnectionInfo*>::iterator i = connections_.begin();
  //     i != connections_.end(); i++)
  //         delete i->second;
  //     connections_.clear();
  //     chunks_.clear();
  //     connection_indexes_.clear();
  //     curr_chunk_connection_indexes_.clear();

  Init();
}

int Bag::GetSize() { return file_size_; }

std::string Bag::GetFileName() { return filename_; }

void Bag::Write(std::string const &s) { this->Write(s.c_str(), s.length()); }

void Bag::Write(char const *s, std::streamsize n) {
  LOG_DEBUG("Bag::Write");
  if (!filestream_)
    throw MsgbagException("Bag write file is not opened.");
  filestream_.write((char *)s, n);
  UpdateFileInfo();
}

} // namespace msgbag
} // namespace nullmax
