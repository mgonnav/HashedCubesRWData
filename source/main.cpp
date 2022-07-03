#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct BinaryHeader {
  uint32_t bytes;
  uint32_t records;
};

struct coordinates_t {
  float lat, lon;

  coordinates_t(float lat, float lon) {
    this->lat = lat;
    this->lon = lon;
  }
};

BinaryHeader header;

std::string path = "../data/brightkite.nds";
std::string write_path = "../data/victoria_with_classes.nds";

template <typename T>
void get_data(std::vector<uint8_t>& data, uint8_t offset) {
  data.clear();

  std::ifstream infile(path, std::ios::binary);

  infile.ignore(sizeof(BinaryHeader) + (header.records * offset));

  data.resize(sizeof(T) * header.records);
  infile.read(reinterpret_cast<char*>(&data[0]), sizeof(T) * header.records);

  infile.close();
}

template <typename T>
T* get_entry(std::vector<uint8_t>& data, size_t id) {
  return ((T*)&data[id * sizeof(T)]);
}

int main() {
  std::ifstream infile("../data/victoria_with_classes.csv");
  // "ALCOHOLTIME", "POLICE_ATTEND", "SPEED_ZONE", "FATALITY", "SERIOUSINJURY", "OTHERINJURY",
  // "NONINJURED", "MALES", "FEMALES", "LONGITUDE", "LATITUDE", "class"

  std::vector<uint8_t> alcoholtime;
  std::vector<uint8_t> police_attend;
  std::vector<uint8_t> speed_zone;
  std::vector<uint8_t> fatality;
  std::vector<uint8_t> seriousinjury;
  std::vector<uint8_t> otherinjury;
  std::vector<uint8_t> noninjured;
  std::vector<uint8_t> males;
  std::vector<uint8_t> females;
  std::vector<coordinates_t> coords;
  std::vector<uint8_t> classes;

  std::string line, word;

  getline(infile, line);

  while (getline(infile, line)) {
    std::stringstream str(line);
    // std::cout << str.str() << std::endl;

    getline(str, word, ',');
    alcoholtime.push_back(std::stoi(word));

    getline(str, word, ',');
    police_attend.push_back(std::stoi(word));

    getline(str, word, ',');
    speed_zone.push_back(std::stoi(word));

    getline(str, word, ',');
    fatality.push_back(std::stoi(word));

    getline(str, word, ',');
    seriousinjury.push_back(std::stoi(word));

    getline(str, word, ',');
    otherinjury.push_back(std::stoi(word));

    getline(str, word, ',');
    noninjured.push_back(std::stoi(word));

    getline(str, word, ',');
    males.push_back(std::stoi(word));

    getline(str, word, ',');
    females.push_back(std::stoi(word));

    getline(str, word, ',');
    float lon = std::stof(word);
    getline(str, word, ',');
    float lat = std::stof(word);
    coords.push_back(coordinates_t(lat, lon));

    getline(str, word, ',');
    classes.push_back(std::stoi(word));
  }

  for (size_t i = 0; i < 10; ++i)
    std::cout << +alcoholtime[i] << " " << +police_attend[i] << " " << +speed_zone[i] << " " <<
              +fatality[i] << " " << +seriousinjury[i] << " " << +otherinjury[i] << " " <<
              +noninjured[i] << " " << +males[i] << " " << +females[i] << " " <<
              coords[i].lat << " " << coords[i].lon << " " << +classes[i] << "\n";

  std::ofstream outFile("../data/victoria_with_classes.nds",
                        std::ios::binary);

  header.bytes = 0;
  header.records = 19726;

  outFile.write((char*)&header, sizeof(BinaryHeader));
  outFile.write((char*)&coords[0], sizeof(coordinates_t) * alcoholtime.size());
  outFile.write((char*)&alcoholtime[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&police_attend[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&speed_zone[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&fatality[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&seriousinjury[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&otherinjury[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&noninjured[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&males[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&females[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.write((char*)&classes[0], sizeof(uint8_t) * alcoholtime.size());
  outFile.close();

  return 0;
}
