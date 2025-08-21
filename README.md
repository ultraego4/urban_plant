# About project

POSTPONED UNTIL SUMMER 2026!

This repository contains my "Optimization of Plant Growth on Urban Terraces Using Embedded Devices" home project.

The project is developed in ESP-IDF and builds on an ESP32-S3 N16R8 with multiple sensors such as VEML7700, BME280 etc.


[Documentation](docs/urban_plant.pdf) is written in LaTeX.


![circuit](docs/assets/circuit_above.jpg)

## Setup

1. Follow [ESP-IDF wiki](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html) and install. Set this useful alias so you can source the tools:
```
alias get_idf='. $HOME/esp/esp-idf/export.sh'
```
2. Clone the used sensor library and copy the components folder:
```
cd ~/esp && git clone https://github.com/UncleRus/esp-idf-lib
```
```
mkdir -p ~/esp/esp-idf/components && cp -rf ~/esp/esp-idf-lib/components/* ~/esp/esp-idf/components/
```
3. Clone the repository and cd:

```
git clone https://github.com/ultraego4/urban_plant.git && cd urban_plant
```


## How to Use


1. Source the ESP-IDF environment:
   ```bash
   get_idf
   ```

2. List available targets:
   ```bash
   idf.py --list-targets
   ```

3. Set your target:
   ```bash
   idf.py set-target <target>
   ```

4. Build the project:
   ```bash
   idf.py build
   ```

5. Flash the device (replace `/dev/ttyACM0` with your own device path):
   ```bash
   idf.py -p /dev/ttyACM0 flash
   ```

6. Monitor the device output:
   ```bash
   idf.py -p /dev/ttyACM0 monitor
   ```

