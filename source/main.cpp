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
std::string write_path = "../data/house_with_classes.nds";

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
  std::ifstream infile("../data/house_with_classes.csv");

  // std::vector<uint8_t> Start_Lat;
  // std::vector<uint8_t> Start_Lng;
  // std::vector<uint8_t> End_Lat;
  // std::vector<uint8_t> End_Lng;

  std::vector<uint8_t> id;
  std::vector<uint8_t> price;
  std::vector<uint8_t> type;
  std::vector<uint8_t> sqfeet;
  std::vector<uint8_t> beds;
  std::vector<uint8_t> baths;
  std::vector<uint8_t> cats_allowed;
  std::vector<uint8_t> dogs_allowed;
  std::vector<uint8_t> wheelchair_access;
  std::vector<uint8_t> smoking_allowed;
  std::vector<uint8_t> electric_vehicle_charge;
  std::vector<uint8_t> comes_furnished;
  std::vector<uint8_t> laundry_options;
  std::vector<uint8_t> parking_options;
  std::vector<coordinates_t> coords;
  std::vector<uint8_t> cluster_class;
  std::string line, word;

  getline(infile, line);

  while (getline(infile, line)) {
    std::stringstream str(line);

    getline(str, word, ',');
    // id.push_back(std::stoi(word));
    getline(str, word, ',');
    // price.push_back(std::stoi(word));
    getline(str, word, ',');
    // type.push_back(std::stoi(word));
    getline(str, word, ',');
    // sqfeet.push_back(std::stoi(word));
    getline(str, word, ',');
    // beds.push_back(std::stoi(word));
    getline(str, word, ',');
    // baths.push_back(std::stoi(word));
    getline(str, word, ',');
    // cats_allowed.push_back(std::stoi(word));
    getline(str, word, ',');
    // dogs_allowed.push_back(std::stoi(word));
    getline(str, word, ',');
    // wheelchair_access.push_back(std::stoi(word));
    getline(str, word, ',');
    // smoking_allowed.push_back(std::stoi(word));
    getline(str, word, ',');
    // electric_vehicle_charge.push_back(std::stoi(word));
    getline(str, word, ',');
    // comes_furnished.push_back(std::stoi(word));
    getline(str, word, ',');
    // laundry_options.push_back(std::stoi(word));
    getline(str, word, ',');
    // parking_options.push_back(std::stoi(word));

    getline(str, word, ',');
    float start_lat = std::stof(word);
    getline(str, word, ',');
    float start_lon = std::stof(word);
    coords.push_back(coordinates_t(start_lat, start_lon));

    getline(str, word, ',');
    cluster_class.push_back(std::stoi(word));
  }

  std::ofstream outFile("../data/house_with_classes.nds",
                        std::ios::binary);

  header.records = 31893;
  header.bytes = 0;
  outFile.write((char*)&header, sizeof(BinaryHeader));
  outFile.write((char*)&coords[0], sizeof(coordinates_t) * coords.size());
  outFile.write((char*)&cluster_class[0], sizeof(uint8_t) * coords.size());
  outFile.close();

  return 0;
}
