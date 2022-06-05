#include <iostream>
#include <fstream>
#include <vector>

struct BinaryHeader {
  uint32_t bytes;
  uint32_t records;
};

struct coordinates_t {
  float lat, lon;
};

BinaryHeader header;

std::string path = "../data/brightkite-example.nds";

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
  std::ifstream infile(path, std::ios::binary);

  infile.read((char*)&header, sizeof(BinaryHeader));

  infile.close();

  std::cout << "Total number of records: " << header.records << "\n";

  // Read Dimensions
  std::vector<uint8_t> data;

  // Read day_of_week
  typedef uint8_t categorical_t;
  uint8_t offset = 8;
  get_data<categorical_t>(data, offset);

  for (int i = 0; i < header.records; ++i) {
    uint32_t entry = (*get_entry<categorical_t>(data, i));
    std::cout << entry << ";";
  }
  std::cout << std::endl;
   

  // Read hour_of_day
  std::vector<uint8_t> data2;
  typedef uint8_t categorical_t;
  offset = 9;
  get_data<categorical_t>(data2, offset);

  for (int i = 0; i < header.records; ++i) {
    uint32_t entry = (*get_entry<categorical_t>(data2, i));
    std::cout << entry << ";";
  }
  std::cout << std::endl;
  

  // Read temporal
  std::vector<uint8_t> data3;
  typedef uint32_t temporal_t;
  offset = 10;
  get_data<temporal_t>(data3, offset);

  for (int i = 0; i < header.records; ++i) {
    uint32_t entry = (*get_entry<temporal_t>(data3, i));
    uint32_t value = static_cast<temporal_t>(entry / static_cast<float>(604800)) * 604800;
    std::cout << value << ";";
  }

  // Read spatial
  std::vector<uint8_t> data4;
  offset = 32;
  get_data<coordinates_t>(data4, offset);

  for (int i = 0; i < header.records; ++i) {
    coordinates_t entry = (*get_entry<coordinates_t>(data, i));
    std::cout << entry.lat << std::endl;
  }

  return 0;
}