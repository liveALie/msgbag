#pragma once
#include "common/msgbag_types.h"
#include "common/timestamp.h"
#include <fstream>
#include <string>

namespace nullmax {
namespace msgbag {

struct MsgRecord {
  std::string topic_;
  Timestamp arrived_time;
  MsgbagBuffer serialized_msg;
  size_t msg_size_;
};

using MsgRecordPtr = std::shared_ptr<MsgRecord>;

using RecordsPtr = std::multimap<std::string, MsgRecordPtr>;

const static std::string VERSION = "1.0";
class Bag {
  friend class Recorder;
  friend class Player;
  friend class View;

public:
  Bag();
  Bag(const std::string &filename, uint32_t mode = BagMode::Read);
  ~Bag();
  void SetCompression(CompressionType compression);
  void SetChunkThreshold(int chunk_size);
  void Open(const std::string &filename, uint32_t mode = BagMode::Read);
  void Close();
  int GetSize();
  void Write(std::string const &s);
  void Write(char const *s, std::streamsize n);
  std::string GetFileName();

  uint32_t getMajorVersion() const {
    return version_ / 100;
  } //!< Get the major-version of the open bag file

  uint32_t getMinorVersion() const {
    return version_ % 100;
  } //!< Get the minor-version of the open bag file

  BagMode GetMode() const { return mode_; }

private:
  void Init();
  // void Seek(uint64_t pos, int origin = std::ios_base::beg);
  void OpenReadWrite(const std::string &filename);
  void OpenWrite(const std::string &filename);
  void OpenRead(const std::string &filename);
  void UpdateFileInfo();
  void CloseWrite();
  void ReadVersion();
  void StartWriting();
  void StopWriting();
  void WriteVersion();
  void WriteHeaderRecord();
  void ReadHeaderRecord();
  void StartReadingVersion100();
  void ReadTopicIndexRecord100();
  bool CheckEnoughDataToRead(size_t size);

private:
  std::string filename_;
  std::fstream filestream_;
  uint64_t file_size_;
  BagMode mode_;
  uint64_t offset_;
  uint64_t file_header_pos_;
  int version_;
  CompressionType compression_;
  uint32_t chunk_threshold_;
  uint64_t index_data_pos_;
  long long start_timestamp_;
  long long end_timestamp_;
  uint32_t msg_count_;

  BagRecordsType records_;

  ////////以下待处理
  // mutable ChunkedFile file_;

  // CompressionType compression_;
  // uint32_t chunk_threshold_;
  // uint32_t bag_revision_;

  // uint64_t file_size_;
  // uint64_t file_header_pos_;
  // uint64_t index_data_pos_;
  // uint32_t connection_count_;
  // uint32_t chunk_count_;

  // // Current chunk
  // bool chunk_open_;
  // // ChunkInfo curr_chunk_info_;
  // uint64_t curr_chunk_data_pos_;

  // std::map<std::string, uint32_t> topic_connection_ids_;
  // std::map<ros::M_string, uint32_t> header_connection_ids_;
  // std::map<uint32_t, ConnectionInfo *> connections_;

  // std::vector<ChunkInfo> chunks_;

  // std::map<uint32_t, std::multiset<IndexEntry>> connection_indexes_;
  // std::map<uint32_t, std::multiset<IndexEntry>>
  // curr_chunk_connection_indexes_;

  // mutable Buffer header_buffer_; //!< reusable buffer in which to assemble
  // the
  //                                //! record header before writing to file
  // mutable Buffer record_buffer_; //!< reusable buffer in which to assemble
  // the
  //                                //! record data before writing to file

  // mutable Buffer chunk_buffer_; //!< reusable buffer to read chunk into
  // mutable Buffer
  //     decompress_buffer_; //!< reusable buffer to decompress chunks into

  // mutable Buffer outgoing_chunk_buffer_; //!< reusable buffer to read chunk
  // into

  // mutable Buffer *current_buffer_;

  // mutable uint64_t decompressed_chunk_; //!< position of decompressed chunk

  // // Encryptor plugin loader
  // pluginlib::ClassLoader<rosbag::EncryptorBase> encryptor_loader_;
  // // Active encryptor
  // boost::shared_ptr<rosbag::EncryptorBase> encryptor_;

  // ChunkedFile file_;
};
} // namespace msgbag
} // namespace nullmax
