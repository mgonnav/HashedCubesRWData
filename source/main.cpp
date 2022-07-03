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
std::string write_path = "../data/brightkite_with_classes_ni.nds";

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
  std::ifstream infile("../data/brightkite_with_classes_ni.csv");

  std::vector<uint8_t> days;
  std::vector<uint8_t> hrs;
  std::vector<uint32_t> timestamps;
  std::vector<coordinates_t> coords;
  std::vector<uint8_t> classes;
  std::vector<float> color_classes;

  std::string line, word;

  getline(infile, line);

  while (getline(infile, line)) {
    std::stringstream str(line);
    // std::cout << str.str() << std::endl;

    getline(str, word, ',');
    days.push_back(std::stoi(word));

    getline(str, word, ',');
    hrs.push_back(std::stoi(word));

    getline(str, word, ',');
    timestamps.push_back(std::stoi(word));

    getline(str, word, ',');
    float lat = std::stof(word);
    getline(str, word, ',');
    float lon = std::stof(word);
    coords.push_back(coordinates_t(lat, lon));

    getline(str, word, ',');
    classes.push_back(std::stoi(word));

    getline(str, word);
    color_classes.push_back(std::stof(word));
  }

  for (size_t i = 0; i < 10; ++i)
    std::cout << +days[i] << " " << +hrs[i] << " " << timestamps[i] << " " <<
              coords[i].lat << " " << coords[i].lon << " " << +classes[i] << " " <<
              color_classes[i] << "\n";

  std::ofstream outFile("../data/brightkite_with_classes_ni.nds",
                        std::ios::binary);

  std::ifstream infile3("../data/brightkite.nds", std::ios::binary);

  infile3.read((char*)&header, sizeof(BinaryHeader));

  infile3.close();

  outFile.write((char*)&header, sizeof(BinaryHeader));
  outFile.write((char*)&coords[0], sizeof(coordinates_t) * days.size());
  outFile.write((char*)&days[0], sizeof(uint8_t) * days.size());
  outFile.write((char*)&hrs[0], sizeof(uint8_t) * days.size());
  outFile.write((char*)&timestamps[0], sizeof(uint32_t) * days.size());
  outFile.write((char*)(&classes[0]), sizeof(uint8_t) * days.size());
  outFile.write((char*)(&color_classes[0]), sizeof(float) * days.size());
  outFile.close();

  std::cout << "Reading..." << std::endl;

  std::ifstream infile2("../data/brightkite_with_classes_ni.nds",
                        std::ios::binary);

  infile2.read((char*)&header, sizeof(BinaryHeader));

  infile2.close();

  std::cout << "Total number of records: " << header.records << "\n";

  // Read Dimensions
  std::vector<uint8_t> data;

  // Read day_of_week
  typedef uint8_t categorical_t;
  uint8_t offset = 8;
  get_data<categorical_t>(data, offset);

  // for (size_t i = 0; i < header.records; ++i) {
  // uint32_t entry = (*get_entry<categorical_t>(data, i));
  // std::cout << entry << ";";
  // }
  // std::cout << std::endl;


  // Read hour_of_day
  std::vector<uint8_t> data2;
  typedef uint8_t categorical_t;
  offset = 9;
  get_data<categorical_t>(data2, offset);

  // for (size_t i = 0; i < header.records; ++i) {
  // uint32_t entry = (*get_entry<categorical_t>(data2, i));
  // std::cout << entry << ";";
  // }
  // std::cout << std::endl;


  // Read temporal
  std::vector<uint8_t> data3;
  typedef uint32_t temporal_t;
  offset = 10;
  get_data<temporal_t>(data3, offset);

  // for (size_t i = 0; i < header.records; ++i) {
  // uint32_t entry = (*get_entry<temporal_t>(data3, i));
  // uint32_t value = static_cast<temporal_t>(entry / static_cast<float>(604800)) * 604800;
  // std::cout << value << ";";
  // }

  // Read spatial
  std::vector<uint8_t> data4;
  offset = 0;
  get_data<coordinates_t>(data4, offset);

  // for (size_t i = 0; i < header.records; ++i) {
  // coordinates_t entry = (*get_entry<coordinates_t>(data4, i));
  // std::cout << entry.lat << std::endl;
  // }

  for (size_t i = 0; i < 10; ++i) {
    uint32_t day_entry = (*get_entry<categorical_t>(data, i));

    uint32_t hour_entry = (*get_entry<categorical_t>(data2, i));

    uint32_t time_entry = (*get_entry<temporal_t>(data3, i));
    uint32_t time_value = static_cast<temporal_t>(time_entry / static_cast<float>
                          (604800)) * 604800;

    coordinates_t coordinates_entry = (*get_entry<coordinates_t>(data4, i));

    std::cout << day_entry << "," << hour_entry << "," << time_value << "," <<
              coordinates_entry.lat << "," << coordinates_entry.lon << std::endl;
  }

  return 0;
}
