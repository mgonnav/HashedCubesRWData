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
std::string write_path = "../data/california_with_classes.nds";

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
  std::ifstream infile("../data/california_with_classes.csv");

  // std::vector<uint8_t> Start_Lat;
  // std::vector<uint8_t> Start_Lng;
  // std::vector<uint8_t> End_Lat;
  // std::vector<uint8_t> End_Lng;

  std::vector<uint8_t> ID;
  std::vector<uint8_t> Severity;
  std::vector<std::string> Start_Time;
  std::vector<std::string> End_Time;
  std::vector<coordinates_t> start_coords;
  std::vector<coordinates_t> end_coords;
  std::vector<uint8_t> Distance;
  std::vector<uint8_t> Temperature;
  std::vector<uint8_t> Wind_Chill;
  std::vector<uint8_t> Humidity;
  std::vector<uint8_t> Pressure;
  std::vector<uint8_t> Visibility;
  std::vector<std::string> Wind_Direction;
  std::vector<uint8_t> Wind_Speed;
  std::vector<uint8_t> Precipitation;
  std::vector<std::string> Weather_Condition;
  std::vector<uint8_t> Amenity;
  std::vector<uint8_t> Bump;
  std::vector<uint8_t> Crossing;
  std::vector<uint8_t> Give_Way;
  std::vector<uint8_t> Junction;
  std::vector<uint8_t> No_Exit;
  std::vector<uint8_t> Railway;
  std::vector<uint8_t> Roundabout;
  std::vector<uint8_t> Station;
  std::vector<uint8_t> Stop;
  std::vector<uint8_t> Traffic_Calming;
  std::vector<uint8_t> Traffic_Signal;
  std::vector<uint8_t> Turning_Loop;
  std::vector<uint8_t> Sunrise_Sunset;
  std::vector<uint8_t> Civil_Twilight;
  std::vector<uint8_t> Nautical_Twilight;
  std::vector<uint8_t> Astronomical_Twilight;
  std::vector<uint8_t> cluster_class;

  std::string line, word;

  getline(infile, line);

  while (getline(infile, line)) {
    std::stringstream str(line);
    
    getline(str, word, ',');
    // ID.push_back(std::stoi(word));

    getline(str, word, ',');
    // Severity.push_back(std::stoi(word));

    getline(str, word, ',');
    Start_Time.push_back(word);
    getline(str, word, ',');
    End_Time.push_back(word);

    getline(str, word, ',');
    float start_lat = std::stof(word);
    getline(str, word, ',');
    float start_lon = std::stof(word);
    start_coords.push_back(coordinates_t(start_lat, start_lon));

    getline(str, word, ',');
    float end_lat = std::stof(word);
    getline(str, word, ',');
    float end_lon = std::stof(word);
    end_coords.push_back(coordinates_t(end_lat, end_lon));

    getline(str, word, ',');
    // Distance.push_back(std::stoi(word));

    getline(str, word, ',');
    // Temperature.push_back(std::stoi(word));

    getline(str, word, ',');
    // Wind_Chill.push_back(std::stoi(word));

    getline(str, word, ',');
    // Humidity.push_back(std::stoi(word));

    getline(str, word, ',');
    // Pressure.push_back(std::stoi(word));

    getline(str, word, ',');
    // Visibility.push_back(std::stoi(word));

    getline(str, word, ',');
    Wind_Direction.push_back(word);

    getline(str, word, ',');
    // Wind_Speed.push_back(std::stoi(word));

    getline(str, word, ',');
    // Precipitation.push_back(std::stoi(word));

    getline(str, word, ',');
    Weather_Condition.push_back(word);

    getline(str, word, ',');
    // Amenity.push_back(std::stoi(word));

    getline(str, word, ',');
    // Bump.push_back(std::stoi(word));

    getline(str, word, ',');
    // Crossing.push_back(std::stoi(word));

    getline(str, word, ',');
    // Give_Way.push_back(std::stoi(word));

    getline(str, word, ',');
    // Junction.push_back(std::stoi(word));

    getline(str, word, ',');
    // No_Exit.push_back(std::stoi(word));

    getline(str, word, ',');
    // Railway.push_back(std::stoi(word));

    getline(str, word, ',');
    // Roundabout.push_back(std::stoi(word));

    getline(str, word, ',');
    // Station.push_back(std::stoi(word));

    getline(str, word, ',');
    // Stop.push_back(std::stoi(word));

    getline(str, word, ',');
    // Traffic_Calming.push_back(std::stoi(word));

    getline(str, word, ',');
    // Traffic_Signal.push_back(std::stoi(word));

    getline(str, word, ',');
    // Turning_Loop.push_back(std::stoi(word));

    getline(str, word, ',');
    // Sunrise_Sunset.push_back(std::stoi(word));

    getline(str, word, ',');
    // Civil_Twilight.push_back(std::stoi(word));

    getline(str, word, ',');
    // Nautical_Twilight.push_back(std::stoi(word));

    getline(str, word, ',');
    // Astronomical_Twilight.push_back(std::stoi(word));

    getline(str, word, ',');
    cluster_class.push_back(std::stoi(word));
  }

  std::ofstream outFile("../data/california_with_classes.nds",
                        std::ios::binary);

  header.records = 25329;
  header.bytes = 0;
  outFile.write((char*)&header, sizeof(BinaryHeader));
  outFile.write((char*)&start_coords[0], sizeof(coordinates_t) * start_coords.size());
  outFile.write((char*)&end_coords[0], sizeof(coordinates_t) * start_coords.size());
  // outFile.write((char*)&Wind_Direction[0], sizeof(char) * start_coords.size());
  outFile.write((char*)&cluster_class[0], sizeof(uint8_t) * start_coords.size());
  outFile.close();

  return 0;
}
