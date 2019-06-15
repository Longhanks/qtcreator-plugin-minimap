# qtcreator-plugin-minimap

Minimap for [Qt Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).

## Copyright / License

Copyright (C) 2017 emJay Software Consulting AB (Mattias Johansson, matjo75@gmail.com),

Copyright (C) 2019 Andreas Schulz. All rights reserved.

Licensed under the [LGPLv2.1](https://github.com/Longhanks/qtcreator-plugin-minimap/blob/master/LICENSE) License.

##  LGPLv2.1 Obliations

This repository is a fork of [https://github.com/i94matjo/qtcreator-minimap-plugin](https://github.com/i94matjo/qtcreator-minimap-plugin).

The original source code can be downloaded at the above repository or by checking out this repository at commit `d78de1a6b1b72aaeedfe2bb140d5d4ac1bf24fe8`.

The commits up to 2019-05-31 are copyright protected as follows:

(C) 2017 emJay Software Consulting AB (Mattias Johansson, matjo75@gmail.com), licensed under the [LGPLv2.1](https://github.com/Longhanks/qtcreator-plugin-minimap/blob/master/LICENSE) License.

Changes to the original source can be retrieved by inspecting the commit history since the above date and/or commit.

## Build instructions

Provide the following variables to cmake:

| Variable            | Value                                            |
| ------------------- | ------------------------------------------------ |
| `QTCREATOR_SRC`     | Path to directory containing Qt Creator source   |
| `QTCREATOR_VERSION` | Target version of Qt Creator (defaults to 4.9.0) |

On macOS and Linux, the following variable is needed to find Qt Creator's plugin libraries:

| Variable            | Value                                            |
| ------------------- | ------------------------------------------------ |
| `QTCREATOR_BIN`     | Path to Qt Creator executable binary             |

### Examples

#### macOS

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DQTCREATOR_SRC="~/Downloads/qt-creator-opensource-src-4.9.1" -DQTCREATOR_BIN="/Applications/Qt Creator.app/Contents/MacOS/Qt Creator"
make
sudo make install
```

#### Windows

This example uses the Ninja build system.

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:/Qt/5.12.3/msvc2017_64 -DQTCREATOR_SRC=C:/Users/aschulz/Desktop/qt-creator-opensource-src-4.9.1 -GNinja
ninja
ninja install
```
