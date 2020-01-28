#pragma once


struct Slope : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    if (value == 0) {
      return "LIN";
    } else {
      return "EXP";
    }
  }
};

struct Toggle : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    if (value == 0) {
      return "Off";
    } else {
      return "On";
    }
  }
};

struct Direction : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    if (value == 0) {
      return "Down";
    } else {
      return "Up";
    }
  }
};

struct WaveShape : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    if (value == 0) {
      return "Square";
    } else {
      return "Sine";
    }
  }
};

struct Swing : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    if (value == 0) {
      return "/8";
    } else {
      return "/16";
    }
  }
};

struct Percent : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    return std::to_string(int(value * 100)) + "%";
  }
};

struct WaveMix : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    return std::to_string(100 - int(value * 10)) + "/" + std::to_string(int(value * 10));
  }
};

struct PercentTen : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    return std::to_string(int(value * 10)) + "%";
  }
};

struct Blank : ParamQuantity {
  std::string getDisplayValueString() override {
    return "";
  }
};

struct SubOct : ParamQuantity {
  std::string getDisplayValueString() override {
    float value = getValue();

    return std::to_string(value ? (-int(value)) : 0);
  }
};
