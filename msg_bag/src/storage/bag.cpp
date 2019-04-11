#include "bag.h"
#include "common/msgbag_exception.h"
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

void Bag::ReadVersion() {}

void Bag::WriteVersion() {
  std::string version = std::string("#MSGBAG V") + VERSION + std::string("\n");

  LOG_DEBUG("Writing VERSION {}: {}", (unsigned long long)offset_, version);

  version_ = 200;

  Write(version);
}

void Bag::WriteHeaderRecord() {
  // connection_count_ = connections_.size();
  // chunk_count_      = chunks_.size();

  // CONSOLE_BRIDGE_logDebug("Writing FILE_HEADER [%llu]: index_pos=%llu
  // connection_count=%d chunk_count=%d",
  //           (unsigned long long) file_.getOffset(), (unsigned long long)
  //           index_data_pos_, connection_count_, chunk_count_);

  // // Write file header record
  // M_string header;
  // header[OP_FIELD_NAME]               = toHeaderString(&OP_FILE_HEADER);
  // header[INDEX_POS_FIELD_NAME]        = toHeaderString(&index_data_pos_);
  // header[CONNECTION_COUNT_FIELD_NAME] = toHeaderString(&connection_count_);
  // header[CHUNK_COUNT_FIELD_NAME]      = toHeaderString(&chunk_count_);
  // encryptor_->addFieldsToFileHeader(header);

  // boost::shared_array<uint8_t> header_buffer;
  // uint32_t header_len;
  // ros::Header::write(header, header_buffer, header_len);
  // uint32_t data_len = 0;
  // if (header_len < FILE_HEADER_LENGTH)
  //     data_len = FILE_HEADER_LENGTH - header_len;
  // write((char*) &header_len, 4);
  // write((char*) header_buffer.get(), header_len);
  // write((char*) &data_len, 4);

  // // Pad the file header record out
  // if (data_len > 0) {
  //     string padding;
  //     padding.resize(data_len, ' ');
  //     write(padding);
  // }
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
}

void Bag::OpenRead(const std::string &filename) {}

// void Bag::Seek(uint64_t pos, int origin) {
//   // file_.seek(pos, origin);
// }

void Bag::CloseWrite() { StopWriting(); }

void Bag::StopWriting() {
  filestream_.seekp(0, std::ios::end);
  index_data_pos_ = offset_;
  filestream_.seekp(file_header_pos_);
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
