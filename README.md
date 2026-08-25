# MiniSearchLab
## Yehan WANG, ECNU

<div align="center">
    <img src="https://cdn.jsdelivr.net/gh/VisionNext100/VisionNext100.github.io@main/public/images/projects/project-minisearch.png" width="800" alt="MiniSearchLab">
</div>

### Ⅰ Overview
A desktop mini search engine built with C++ and Qt6 that retrieves, ranks, and displays text documents based on multi-keyword queries. The system constructs an inverted index using hash map structures to efficiently map keywords to their corresponding file names and line numbers. It features a user-friendly graphical interface for directory selection and query input, alongside comprehensive Python scripts for generating boundary and stress test datasets.

### Ⅱ Structure
```text
MiniSearchLab/
├── README.md
├── LICENSE
├── .gitignore
├── CMakeLists.txt              # CMake configuration script for building the project
├── main.cpp                    # Application entry point
├── mainwindow.h                # Header file for the GUI and search engine logic
├── mainwindow.cpp              # Implementation of the inverted index and retrieval algorithms
├── mainwindow.ui               # Qt Designer UI XML file for the graphical interface
├── MiniSearchLab_zh_CN.ts      # Qt translation file
├── scripts/                    # Python scripts for generating massive test datasets
│   ├── generate_data_pressure.py
│   └── generate_query.py
├── test_basic/                 # Basic functionality test cases (multiple matching, single matching, etc.)
├── test_boundary/              # Boundary condition test cases (empty files, line length limits)
└── test1/                      # Sample test case provided in the initial project description
```

### Ⅲ Implementation
- **Inverted Index Construction:** The core data structure relies on a nested map `QMap<QString, QMap<QString, QSet<int>>> index;`. During initialization, the engine reads all files in the selected directory line by line, splits the text into words using regular expressions, converts them to lowercase, and records the specific line numbers where each word appears.  
- **Query Processing:** User queries are split into individual words and converted to lowercase for case-insensitive matching. The engine then looks up each word in the pre-built inverted index.  
- **Multi-Keyword Intersection:** To handle queries with multiple words, the system maintains tracker maps (`fileMatchCounts` and `matchedLines`). It sequentially intersects the file lists for each word, ensuring that only files containing all queried words are retained as valid matches.  
- **Result Ranking:** The matched files are sorted in descending order based on the number of query words they contain (match count). If multiple files have the same match count, they are sorted alphabetically by their filenames in ascending order.  
- **Output Generation:** For each successfully matched file, the engine formats the output by printing the filename first, followed by the specific lines where the keywords appeared. The use of `QSet<int>` guarantees that duplicate lines are not printed repeatedly. 

### Ⅳ Compilation
1\. **Prerequisites**
- A C++ compiler supporting modern C++ standards.
- The Qt 6 framework, specifically the `Widgets` module.
- CMake (minimum version 3.16).   

2\. **Compilation Steps**  
Navigate to the project root directory in your terminal and execute the following commands to build the executable:
```shell
mkdir build
cd build
cmake ..
cmake --build .
```

3\. **Running the Application**
- Launch the compiled executable (e.g., `./MiniSearchEngine` on Linux/macOS or `.\MiniSearchEngine.exe` on Windows).
- Click the "Browse..." button in the GUI to select a directory containing your target text files (e.g., the `test_basic/` folder). 
- In the query input box, enter the total number of queries on the first line, followed by one query per line.
- Click "Search" to view the retrieved documents and matched lines in the results area.  
