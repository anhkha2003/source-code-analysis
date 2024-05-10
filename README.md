# Source Code Analysis Tool

## Overview

The Source Code Analysis Tool is designed to analyze and compare students' source code submissions against specified test data for programming challenges. It helps in identifying patterns and potential cases of copying by evaluating the similarity of the source codes to the test data. This tool is optimized for efficient memory and time usage, utilizing the Aho-Corasick algorithm for enhanced pattern detection.

## Features

- **Supports Multiple Formats**: Capable of analyzing both directories and zip files containing source code and test data.
- **Efficient Pattern Detection**: Utilizes the Aho-Corasick algorithm to detect and handle special strings and programming constructs efficiently.
- **Optimized Performance**: Designed for high performance in terms of processing speed and memory usage.

## Installation Requirements

- **C++17 Compiler or later**
- **Unzip Utility**: Must be installed on the system.
- **Operating System**: A modern Linux environment (tested on Ubuntu).

## Installing the Unzip Utility

Before running the tool, ensure that the `unzip` utility is installed on your system. This is necessary for processing zip files. Install it using the following command:

```bash
sudo apt-get install unzip

## Compilation

Compile the tool using the following command:

```bash
g++ -std=c++17 -o detector detector.cpp

## Usage

To run the tool, use the following command format:

```bash
./detector -s <submission path> -d <testdata path>

Where <submission path> and <testdata path> can be paths to either directories or zip files containing the submissions and test data, respectively.

## Examples

- **Analyzing Directories**:

```bash
./detector -s /path/to/submissions -d /path/to/testdata

- **Analyzing Zip files**:

```bash
./detector -s /path/to/submissions -d /path/to/testdata

## Output
The tool outputs the similarity scores of the analyzed source files, sorted from the highest to the lowest. Each output line includes the similarity score followed by the filename of the source code.
