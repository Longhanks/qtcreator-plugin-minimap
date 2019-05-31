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
| `QTCREATOR_BIN`     | Path to Qt Creator executable binary             |
| `QTCREATOR_VERSION` | Target version of Qt Creator (defaults to 4.9.0) |

For example (macOS):

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DQTCREATOR_SRC="~/Downloads/qt-creator-opensource-src-4.9.0" -DQTCREATOR_BIN="/Applications/Qt Creator.app/Contents/MacOS/Qt Creator"
make
sudo make install
```
