#include "frames.h"

class Tag {
 private:
  std::string filename_;
  unsigned char prev_char_{};
  unsigned char cur_char_{};
  char tag_[4]{};
  bool unsynchronisation_ = false;
  uint32_t tag_size_ = 0;
 public:
  explicit Tag(const std::string& filename) {
    filename_ = filename;
    Parser();
  }
  ~Tag() {
    for (Frame* i : arguments) {
      delete i;
    }
  }
  void Parser();
  std::vector<Frame*> arguments;
};