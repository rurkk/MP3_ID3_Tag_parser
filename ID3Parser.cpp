#include "ID3Parser.h"

void NextElementUnsynchronisation(unsigned char& prev_char, unsigned char& cur_char, std::ifstream& fin) {
  if (prev_char == 255 and cur_char == 0) {
    unsigned char new_char;
    new_char = fin.get();
    if ((new_char >> 7) % 2 == 1 and (new_char >> 6) % 2 == 1 and (new_char >> 5) % 2 == 1) {
      prev_char = cur_char;
      cur_char = new_char;
    } else {
      fin.seekg(-1, std::ios::cur);
    }
  }
}

void NextElement(unsigned char& prev_char, unsigned char& cur_char, std::ifstream& fin, bool unsynchronisation) {
  prev_char = cur_char;
  cur_char = fin.get();
  if (unsynchronisation) {
    NextElementUnsynchronisation(prev_char, cur_char, fin);
  }
}

void Tag::Parser() {
  std::ifstream fin(filename_.substr(2, filename_.length() - 2), std::ios::binary);
  bool frames_locates_eof;
  if (fin.is_open()) {
    fin.read(tag_, 3);                ///header
    if (strcmp(tag_, "ID3") == 0) {
      frames_locates_eof = false;
    } else if (strcmp(tag_, "3DI") == 0) {
      frames_locates_eof = true;
    } else {
      std::cout << "not ID3";

      return;
    }
    fin.seekg(2, std::ios::cur);
    unsigned char header_flags;
    header_flags = fin.get();
    for (int i = 0; i < 4; i++) {
      tag_size_ += fin.get() << 7 * (3 - i);
    }
    if ((header_flags >> 6) % 2 == 1) {   ///extended header
      uint32_t extended_header_size;
      extended_header_size = 0;
      for (int i = 0; i < 4; i++) {
        extended_header_size += fin.get() << 7 * (3 - i);
      }
      fin.seekg(extended_header_size - 4, std::ios::cur);
    }
    if ((header_flags >> 7) % 2 == 1) {
      unsynchronisation_ = true;
    }
    if (frames_locates_eof) {
      fin.seekg(tag_size_, std::ios::end);
    }
    cur_char_ = 0;
    while (tag_size_ > 10) {
      char frame_name[5];
      frame_name[4] = '\0';
      for (int i = 0; i < 4; i++) {
        NextElement(prev_char_, cur_char_, fin, unsynchronisation_);
        frame_name[i] = (char) cur_char_;
      }
      if (frame_name[0] == (char) 0) {
        break;
      }
      uint32_t frame_size;
      frame_size = 0;
      for (int i = 0; i < 4; i++) {
        NextElement(prev_char_, cur_char_, fin, unsynchronisation_);
        frame_size += cur_char_ << 7 * (3 - i);
      }
      unsigned char frame_flag1;
      unsigned char frame_flag2;
      NextElement(prev_char_, cur_char_, fin, unsynchronisation_);
      frame_flag1 = cur_char_;
      NextElement(prev_char_, cur_char_, fin, unsynchronisation_);
      frame_flag2 = cur_char_;

      std::string content;
      for (int i = 0; i < frame_size; i++) {
        prev_char_ = cur_char_;
        cur_char_ = fin.get();
        if (unsynchronisation_ and prev_char_ == 255 and cur_char_ == 0) {
          unsigned char new_char;
          new_char = fin.get();
          if ((new_char >> 7) % 2 == 1 and (new_char >> 6) % 2 == 1 and (new_char >> 5) % 2 == 1) {
            prev_char_ = cur_char_;
            cur_char_ = new_char;
            i++;
          } else {
            fin.seekg(-1, std::ios::cur);
          }
        }
        content.push_back((char) cur_char_);
      }

      Frame* frame_new;
      if (frame_name[0] == 'T' or frame_name[0] == 'W' or strcmp(frame_name, "PRIV") == 0
          or strcmp(frame_name, "UFID") == 0) {
        frame_new = new FrameStrings(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "USLT") == 0 or strcmp(frame_name, "COMM") == 0
          or strcmp(frame_name, "USER") == 0) {
        frame_new = new FrameLangStrings(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "APIC") == 0) {
        frame_new = new FrameAPIC(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "COMR") == 0) {
        frame_new = new FrameCOMR(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "ENCR") == 0 or strcmp(frame_name, "GRID") == 0) {
        frame_new = new FrameStringCharString(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "EQU2") == 0) {
        frame_new = new FrameEQU2(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "LINK") == 0) {
        frame_new = new FrameLINK(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "OWNE") == 0) {
        frame_new = new FrameOWNE(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "PCNT") == 0) {
        frame_new = new FramePCNT(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "POPM") == 0) {
        frame_new = new FramePOPM(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "POSS") == 0) {
        frame_new = new FramePOSS(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "RBUF") == 0) {
        frame_new = new FrameRBUF(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "SEEK") == 0) {
        frame_new = new FrameSEEK(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "SYLT") == 0) {
        frame_new = new FrameSYLT(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "ETCO") == 0) {
        frame_new = new FrameETCO(frame_name, frame_size, frame_flag1, frame_flag2);
      } else if (strcmp(frame_name, "RVA2") == 0) {
        frame_new = new FrameRVA2(frame_name, frame_size, frame_flag1, frame_flag2);
      } else {
        frame_new = new Frame(frame_name, frame_size, frame_flag1, frame_flag2);
      }
      frame_new->setContent(content);
      arguments.push_back(frame_new);
      tag_size_ -= 10 + frame_size;
    }
  } else {
    std::cout << "this file does not exist";

    return;
  }
  fin.close();
}
