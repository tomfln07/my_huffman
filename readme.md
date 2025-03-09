# Huffman Compression and Decompression

This project implements **Huffman encoding and decoding algorithms** unrelated to the file type to handle. It can handle **any file type**, transforming files into a compressed binary format and restoring them to their original state.

## 🚀 Getting Started

#### Prerequisites
- GCC
- Make

#### Installation
1. Clone the repo
```
git clone https://github.com/tomfln07/my_huffman.git my_huffman
cd my_huffman
```

2. Compile the project
```
make
```

#### Usage
Once the project is compiled, you can use the tool to compress and decompress files.
```
bin/my_huffman (-d) [file path]
```
- file path: path to the file you want to compress / decompress
- -d: should be put before the file path to decompress it

*The compressd / decompressed file will be in your current directory*

##### Benchmarks
To evaluate the effectiveness of the Huffman compression, a benchmark was run on different file types including text, audio, and images.

**Compression**
| File Type     | Original Size | Compressed Size | Time Taken |
|---------------|---------------|-----------------|------------|
| BMP Image     | 3,1KB         | 3,151 KB        | 1.6s       |
| TIFF Image    | 5,1KB         | 4,057 KB        | 2.3s       |
| CSV File      | 7,8KB         | 5,130 KB        | 1.3s       |
| TXT File      | 1,032 KB      | 454 KB          | 0.089s     |
| PCM Audio     | 51 KB         | 46 KB           | 0.031s     |
| WAV Audio     | 5,1 KB        | 4,711 KB        | 2.4s       |

**Decompression**
| File Type     | Compressed Size | Time Taken |
|---------------|-----------------|------------|
| BMP Image     | 3,151 KB        | 22s        |
| TIFF Image    | 4,057 KB        | 28.7s      |
| CSV File      | 5,130 KB        | 10.7s      |
| TXT File      | 454 KB          | 0.3s       |
| PCM Audio     | 46 KB           | 0.3s       |
| WAV Audio     | 4,711 KB        | 32.8s      |

*backmarked on an Intel Core i7 8700 3,2GHz*
##### 🌟 If you like this project, please give it a star!