
# pktcore — Modular Packet Processing System

pktcore is a lightweight and modular packet splitting, recombining, and metadata management system designed for efficient file transmission and processing. Think of it as the backend "core" that handles how files are chopped into pieces, optionally compressed or encrypted, and then recombined later with high reliability.


##  How to Build Locally
You can build pktcore easily on any Linux system (or WSL on Windows) using CMake.

Requirements:

- A C++17-compatible compiler (e.g., g++, clang++)
- cmake version ≥ 3.10

🚀 Steps to Build:

### 1. Clone the repository
```bash
git clone https://github.com/mukuldaroch/pktcore.git
cd pktcore
```
### 2. Create a build directory
```bash
mkdir build
cd build
```
### 3. Run CMake to configure the project
```bash
cmake ..
```
### 4. Compile the project
```bash
make
```
🏁 Run it:
After successful build, you'll get an executable inside the build folder:
```bash
./pktcore
```

### 💡 Pro Tips:
If you're on Windows, use WSL or MSYS2 for the best experience.

If cmake .. fails, make sure your compiler supports C++17. You can check using:

bash
Copy
Edit
g++ --version

##  Features
✅ Split files into packets with fixed-size headers.

✅ Embed rich metadata in each packet (file ID, flags, etc.).

✅ Custom full header for every file (like a manifest).

✅ Efficient file recombination via min-heap & metadata parsing.

✅ Terminal-based file selector with arrow key or Vim-style navigation.

✅ Designed to be embedded into larger systems 

## Structure
```bash
pktcore/
├── CMakeLists.txt
├── compile_commands.json
├── gui
│   ├── index.html
│   ├── main.js
│   └── style.css
├── include
│   ├── combiner.h
│   ├── explorer.h
│   ├── full_header.h
│   ├── mini_header.h
│   ├── pkt_utils.h
│   └── splitter.h
├── makepkg.sh
├── README.md
└── src
    ├── about.md
    ├── build.md
    ├── main.cpp
    └── notes.md
```
## 🛠️ Current Working (in development)
⚠️ **Note:** This project is currently under active development. Things are moving fast, so the README might be slightly outdated at times.

---

### 📁 `full_header.h`
- Creates and manages the 50-byte full header for the original file.
- Stores:
  - `file_id` (unique 5-byte ID for each file),
  - Total number of packets,
  - Payload size,
  - Encryption/compression flags,
  - Original file size,
  - Filename.
- Always used once per file with `part_number = 0`.

---

### 📁 `mini_header.h`
- Creates the lightweight 7-byte header for every packet (except full header).
- Stores:
  - `file_id` (to identify which file it belongs to),
  - Part number (to track order).
- Used by all chunks generated during splitting.
- Helps in identifying and combining packets properly.

---

### 📁 `pkt_utils.h`
- Contains all reusable helper functions used by multiple modules.
- Provides:
  - File utilities, header readers, ID matchers, byte converters, etc.
- Keeps the rest of the code clean and DRY (Don't Repeat Yourself).

---

### 📁 `splitter.h`
- Splits a file into small chunks with headers attached.
- Adds:
  - `full_header` to the first chunk,
  - `mini_header` to the rest.
- Supports future upgrades like compression and encryption.
- Ensures each chunk is packet-ready.

---

### 📁 `combiner.h`
- Scans all files in the current directory.
- Detects headers and groups chunks by `file_id`.
- Identifies full headers (where `part_number = 0`).
- Builds a min-heap based on part number to sort packets.
- Prepares packets for reassembly into the original file.

## Future Plans

Future Plans
 - Add compression and encryption support (flag-based)

 - Add progress bar during split/combine

 - Expose pktcore as a backend library for other apps (like a chess website)

 - Build HTTP API endpoints for file upload/download

 - Make a packet inspector/debug tool


## Want to Contribute?
Whether you're a seasoned C++ wizard, a Linux power user, or just curious and learning — you're welcome here!

This project is just getting started, and there's tons of room for creativity and ideas. Here’s how you can jump in:

🧠 Got an idea? Open an issue and share your thoughts — feature ideas, architecture suggestions, or cool new modules.

🐛 Found a bug? Please report it! Or even better, fix it and send a pull request 🚀

🧪 Want to test or optimize things? Help us make pktcore more robust and faster.

🎨 Got UI/UX skills? A Graphical User Interface is one of the biggest things we want! Join in and help build it.

📚 Improve the docs — better explanations help everyone. Even this README can evolve!

🧩 Make a module! Want to add compression? Encryption? Packet tracking? Go wild.
## Authors

- [@Mukul Daroch](https://www.github.com/mukuldaroch)

