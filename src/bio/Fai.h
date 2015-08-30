//
// Created by Bayo Lau on 8/15/15.
//

#ifndef BIO_FAI_H
#define BIO_FAI_H

#include <memory>
#include <seqan/seq_io.h>
#include "util/Logging.h"

namespace bayolau {
namespace bio {

struct Fai {
  using NameItr = seqan::Iterator<const seqan::StringSet<seqan::CharString> >::Type;

  Fai(Fai&& other) = default;

  Fai() = delete;

  Fai(const Fai&) = delete;

  Fai& operator=(const Fai&) = delete;

  /**
   * @brief get a sequence, optionally for a specific region
   * @param[in] name sequence name
   * @param[in] begin base-0 start base
   * @param[in] end base-0 start base
   * @return unique_ptr to the sequence in type Seq_, null if retrival failed
   */
  template<class Seq_, class String_>
  std::unique_ptr<Seq_> get(const String_& name, uint64_t begin = 0,
                            uint64_t end = std::numeric_limits<uint64_t>::max()) const {
    if (not open_) return nullptr;
    const int64_t idx = getIdByName(name);
    if (idx < 0) return nullptr;
    const uint64_t len = sequenceLength(handler_, idx);
    end = std::min(end, len);
    if (begin >= end) return nullptr;
    std::unique_ptr<Seq_> ret(new Seq_());
    readRegion(*ret, handler_, idx, begin, end);
    return ret;
  }

  explicit Fai(const std::string& file) : handler_(), open_(false) {
    LOG(info) << "opening " << file;
    open_ = open(handler_, file.c_str());
    if (!open_) {
      LOG(error) << "Could not open " << file << " with .fai index" << std::endl;
    }
  }

  /**
   * @brief get the length of the specified sequence
   * @param[in] name sequence name
   * @return length
   */
  template<class String_>
  bool has(const String_& name) const {
    const int64_t idx = getIdByName(name);
    return idx >= 0;
  }

  /**
   * @brief get the length of the specified sequence
   * @param[in] name sequence name
   * @return length
   */
  template<class String_>
  uint64_t size(const String_& name) const {
    const int64_t idx = getIdByName(name);
    if (idx < 0) return 0;
    return sequenceLength(handler_, idx);
  }

  /**
   * @brief get the number of sequences in the file
   * @return length
   */
  uint64_t size() const {
    return static_cast<uint64_t>(std::distance(name_begin(), name_end()));
  }

  /**
   * @brief begin iterator for the list of sequence names
   * @return begin iterator
   */
  NameItr name_begin() const {
    return begin(handler_.seqNameStore);
  }

  /**
   * @brief end iterator for the list of sequence names
   * @return end iterator
   */
  NameItr name_end() const {
    return end(handler_.seqNameStore);
  }

private:
  seqan::FaiIndex handler_;
  bool open_;

  template<class String_>
  int64_t getIdByName(const String_& name) const {
    int64_t idx;
    if (not seqan::getIdByName(idx, handler_, name)) {
      LOG(error) << "Could not get first entry index";
      return -1;
    }
    return idx;
  }
};

}
}

#endif //BIO_FAI_H
