#include "frames.h"

Frame::Frame(const char name[5], uint32_t size, const unsigned char flag1, const unsigned char flag2)
    : name_(name), size_(size), flag1_(flag1), flag2_(flag2) {};

std::string Frame::getName() const{
  return name_;
}

void Frame::setContent(const std::string& content) {}

void Frame::getContent(std::ostream& stream) const{}

void operator<<(std::ostream& stream, const Frame& frame) {
  frame.getContent(stream);
  stream << "------------------------------------------------------------"
               "------------------------------------------------------------";
}

void FrameStrings::setContent(const std::string& content) {
  int i = 0;
  if (getName() == "WXXX" or getName()[0] == 'T') {
    i++;
  }
  std::string new_string;
  for (; i < content.length(); i++) {
    if (content[i] == '\0' and i != content.length() - 1) {
      strings_.push_back(new_string);
      new_string = "";
    } else {
      new_string += content[i];
    }
  }
  strings_.push_back(new_string);
}

void FrameStrings::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  for (const std::string& i : strings_) {
    stream << i << '\n';
  }
}

void FrameStringCharString::setContent(const std::string& content) {
  int i;
  for (i = 0; content[i] != '\0'; i++) {
    string1_ += content[i];
  }
  c_ = content[i + 1];
  for (i = i + 2; i < content.length(); i++) {
    string2_ += content[i];
  }
}

void FrameStringCharString::getContent(std::ostream& stream) const{
  stream << getName() << ":\n" << string1_ << '\n' << c_ << '\n' << string2_ << '\n';
}

void FrameEQU2::setContent(const std::string& content) {
  interpolation_method = content[0];
  int i;
  for (i = 1; content[i] != '\0'; i++) {
    identification_ += content[i];
  }
  i++;
  for (int j = 0; j < 2; j++) {
    frequency_ += content[i + j] << 8 * (1 - j);
  }
  i += 2;
  for (int j = 0; j < 2; j++) {
    volume_adjustment += content[i + j] << 8 * (1 - j);
  }
}

void FrameEQU2::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  if (interpolation_method == '\0') {
    stream << "Band" << '\n';
  } else {
    stream << "Linear" << '\n';
  }
  stream << identification_ << '\n';
}

void FrameAPIC::setContent(const std::string& content) {
  int i;
  for (i = 5; content[i] != '\0'; i++) {
    pic_type_ += content[i];
  }
  for (i = i + 2; content[i] != '\0'; i++) {
    description_ += content[i];
  }
  pic_type_ = MimeTypes::getExtension(pic_type_.c_str());
  uint8_t pic_number;
  pic_number = 1;
  std::ifstream fin;
  fin.open("image" + std::to_string(pic_number) + '.' + pic_type_);
  while (fin.is_open()) {
    fin.close();
    pic_number++;
    fin.open("image" + std::to_string(pic_number) + '.' + pic_type_);
  }
  fin.close();
  std::ofstream fout("image" + std::to_string(pic_number) + '.' + pic_type_, std::ios::binary);
  for (i = i + 1; i < content.length(); i++) {
    fout << content[i];
  }
  fout.close();
}

void FrameAPIC::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << description_ << '\n';
}

void FrameCOMR::setContent(const std::string& content) {
  int i;
  for (i = 1; content[i] != '\0'; i++) {
    price_ += content[i];
  }
  i++;
  for (int j = 0; j < 8; j++) {
    valid_until_ += content[i + j];
  }
  i += 8;
  for (i = i; content[i] != '\0'; i++) {
    contact_URL_ += content[i];
  }
  i++;
  received_as_ = content[i];
  for (i = i + 1; content[i] != '\0'; i++) {
    name_of_seller_ += content[i];
  }
  for (i = i + 1; content[i] != '\0'; i++) {
    description += content[i];
  }
  for (i = i + 1; content[i] != '\0'; i++) {
    pic_type_ += content[i];
  }
  std::ofstream fout(description + "." + (std::string) MimeTypes::getExtension(pic_type_.c_str()));
  for (i = i + 1; i < content.length(); i++) {
    fout << content[i];
  }
  fout.close();
}

void FrameCOMR::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << price_ << '\n';
  stream << valid_until_ << '\n';
  stream << contact_URL_ << '\n';
  stream << name_of_seller_ << '\n';
  stream << description << '\n';
}

void FrameLINK::setContent(const std::string& content) {
  int i;
  for (i = 0; i < 4; i++) {
    frame_identifier_ += content[i];
  }
  for (i = i; content[i] != '\0'; i++) {
    URL_ += content[i];
  }
  std::string new_string;
  for (i = i + 1; i < content.length(); i++) {
    if (content[i] == '\0' and i != content.length() - 1) {
      ID_and_data_.push_back(new_string);
      new_string = "";
    } else {
      new_string += content[i];
    }
  }
  ID_and_data_.push_back(new_string);
}

void FrameLINK::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << frame_identifier_ << '\n';
  stream << URL_ << '\n';
  for (const std::string& i : ID_and_data_) {
    stream << i << '\n';
  }
}

void FrameOWNE::setContent(const std::string& content) {
  int i;
  for (i = 1; content[i] != '\0'; i++) {
    price_ += content[i];
  }
  i++;
  for (int j = 0; j < 8; j++) {
    date_ += content[i + j];
  }
  i += 8;
  for (i = 1; i < content.length(); i++) {
    seller_ += content[i];
  }
}

void FrameOWNE::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << price_ << '\n';
  stream << date_ << '\n';
  stream << seller_ << '\n';
}

void FramePCNT::setContent(const std::string& content) {
  for (int j = 0; j < content.length(); j++) {
    counter_ += content[j] << 8 * (content.length() - 1 - j);
  }
}

void FramePCNT::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << counter_ << '\n';
}

void FramePOPM::setContent(const std::string& content) {
  int i = 0;
  for (i = 0; content[i] != '\0'; i++) {
    email_ += content[i];
  }
  rating_ = content[i + 1];
  i += 2;
  for (int j = 0; j < content.length() - i; j++) {
    counter_ += content[i + j] << 8 * (content.length() - i - 1 - j);
  }
}

void FramePOPM::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << email_ << '\n';
  stream << (int) rating_ << '\n';
  stream << counter_ << '\n';
}

void FramePOSS::setContent(const std::string& content) {
  time_stamp_format_ = content[0];
  for (int i = 0; i < 4; i++) {
    position_ += content[i + 1] << 8 * (3 - i);
  }
}

void FramePOSS::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << time_stamp_format_ << '\n';
  stream << position_ << '\n';
}

void FrameRBUF::setContent(const std::string& content) {
  int i;
  for (i = 0; i < 3; i++) {
    buffer_size_ += content[i] << 8 * (2 - i);
  }
  embedded_info_flag_ = content[i];
  i++;
  for (int j = 0; j < 4; j++) {
    offset_ += content[i + j] << 8 * (3 - j);
  }
}

void FrameRBUF::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << buffer_size_ << '\n';
  stream << embedded_info_flag_ << '\n';
  stream << offset_ << '\n';
}

void FrameSEEK::setContent(const std::string& content) {
  int i;
  for (i = 0; i < 4; i++) {
    offset_ += content[i] << 8 * (3 - i);
  }
}

void FrameSEEK::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << offset_ << '\n';
}

void FrameSYLT::setContent(const std::string& content) {
  int i;
  for (i = 1; i < 4; i++) {
    language_ += content[i];
  }
  time_stamp_format_ = content[i];
  content_type_ = content[i + 1];
  for (i = i + 2; i < content[i] != '\0'; i++) {
    content_descriptor_ += content[i];
  }
  i++;
  while (i < content.length()) {
    if (content[i] == 0) {
      i++;
      int32_t number = 0;
      for (int j = 0; j < 4; j++) {
        number += content[i] << 8 * (3 - j);
        i++;
      }
      content_ += ' ' + std::to_string(number) + ' ';
    }
    if (i < content.length()) {
      content_ += content[i];
    }
    i++;
  }
}

void FrameSYLT::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << language_ << '\n';
  stream << (int) time_stamp_format_ << '\n';
  stream << (int) content_type_ << '\n';
  stream << content_descriptor_ << '\n';
  stream << content_ << '\n';
}

void FrameETCO::setContent(const std::string& content) {
  time_stamp_format_ = content[0];
  event_type_ = content[1];
  for (int i = 0; i < 4; i++) {
    time_stamp_ += content[i + 2] << 8 * (3 - i);
  }
}

void FrameETCO::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << (int) time_stamp_format_ << '\n';
  stream << (int) event_type_ << '\n';
  stream << time_stamp_ << '\n';
}

void FrameRVA2::setContent(const std::string& content) {
  int i;
  for (i = 0; content[0] != 0; i++) {
    identification_ += content[i];
  }
  i++;
  type_of_channel_ = content[i];
  i++;
  for (int j = 0; j < 2; j++) {
    volume_adjustment_ += content[i + j] << 8 * (1 - j);
  }
  i += 2;
  bits_representing_peak_ = content[i];
  i++;
  for (int j = 0; j < bits_representing_peak_; j++) {
    volume_adjustment_ += content[i + j] << 8 * (bits_representing_peak_ - 1 - j);
  }
}

void FrameRVA2::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << identification_ << '\n';
  stream << (int) type_of_channel_ << '\n';
  stream << volume_adjustment_ << '\n';
  stream << bits_representing_peak_ << '\n';
  stream << peak_volume_ << '\n';
}

void FrameLangStrings::setContent(const std::string& content) {
  int i;
  for (i = 1; i < 4; i++) {
    language_ += content[i];
  }
  std::string new_string;
  for (; i < content.length(); i++) {
    if (content[i] == '\0' and i != content.length() - 1) {
      strings_.push_back(new_string);
      new_string = "";
    } else {
      new_string += content[i];
    }
  }
  strings_.push_back(new_string);
}

void FrameLangStrings::getContent(std::ostream& stream) const{
  stream << getName() << ":\n";
  stream << language_ << '\n';
  for (const std::string& i : strings_) {
    stream << i << '\n';
  }
}
