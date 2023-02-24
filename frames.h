#include <cstring>
#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include "MimeTypes.h"


class Frame {
 private:
  unsigned char flag1_;
  unsigned char flag2_;
  uint32_t size_;
  std::string name_;
 public:
  explicit Frame(const char name[5], uint32_t size, unsigned char flag1, unsigned char flag2);
  [[nodiscard]] std::string getName() const;
  virtual ~Frame() = default;
  virtual void setContent(const std::string& content);
  virtual void getContent(std::ostream& stream) const;
  friend void operator<<(std::ostream& stream, const Frame& frame);
};

class FrameStrings : public Frame {
 protected:
  std::vector<std::string> strings_;
 public:
  FrameStrings(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameStringCharString : public Frame {
 private:
  std::string string1_;
  unsigned char c_{};
  std::string string2_;
 public:
  FrameStringCharString(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FramePOSS : public Frame {
 private:
  unsigned char time_stamp_format_{};
  uint32_t position_{};
 public:
  FramePOSS(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameOWNE : public Frame {
 private:
  std::string price_;
  std::string date_;
  std::string seller_;
 public:
  FrameOWNE(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameEQU2 : public Frame {
 private:
  uint8_t interpolation_method{};
  std::string identification_;
  int16_t frequency_{};
  int16_t volume_adjustment{};
 public:
  FrameEQU2(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameLINK : public Frame {
 private:
  std::string frame_identifier_;
  std::string URL_;
  std::vector<std::string> ID_and_data_;
 public:
  FrameLINK(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameCOMR : public Frame {
 private:
  std::string price_;
  std::string valid_until_;
  std::string contact_URL_;
  unsigned char received_as_{};
  std::string name_of_seller_;
  std::string description;
  std::string pic_type_;
 public:
  FrameCOMR(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameAPIC : public Frame {
 private:
  std::string pic_type_;
  std::string description_;
 public:
  FrameAPIC(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FramePCNT : public Frame {
 protected:
  uint64_t counter_{};
 public:
  FramePCNT(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FramePOPM : public FramePCNT {
 private:
  std::string email_;
  uint8_t rating_{};
 public:
  FramePOPM(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      FramePCNT(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameRBUF : public Frame {
 private:
  uint32_t buffer_size_{};
  bool embedded_info_flag_{};
  uint32_t offset_{};
 public:
  FrameRBUF(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameSEEK : public Frame {
 private:
  uint32_t offset_{};
 public:
  FrameSEEK(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameSYLT : public Frame {
 private:
  std::string language_;
  unsigned char time_stamp_format_{};
  unsigned char content_type_{};
  std::string content_descriptor_;
  std::string content_;
 public:
  FrameSYLT(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameETCO : public Frame {
 private:
  unsigned char time_stamp_format_{};
  unsigned char event_type_{};
  uint32_t time_stamp_{};
 public:
  FrameETCO(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameRVA2 : public Frame {
 private:
  std::string identification_;
  unsigned char type_of_channel_{};
  int16_t volume_adjustment_{};
  uint8_t bits_representing_peak_{};
  uint64_t peak_volume_{};
 public:
  FrameRVA2(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      Frame(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};

class FrameLangStrings : public FrameStrings {
 private:
  std::string language_;
 public:
  FrameLangStrings(const char name[5], uint32_t size, unsigned const char flag1, unsigned const char flag2) :
      FrameStrings(name, size, flag1, flag2) {}
  void setContent(const std::string& content) override;
  void getContent(std::ostream& stream) const override;
};