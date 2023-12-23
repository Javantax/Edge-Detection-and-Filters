# Edge-Detection-and-Filters

# Image Processing Program

This C program performs various image processing operations on BMP images, including grayscale conversion, reflection, blurring, and edge detection.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Usage](#usage)
- [Filters](#filters)
- [Build](#build)
- [Contributing](#contributing)

## Description

This program allows you to apply different image processing filters to BMP images. It serves as a tool for performing basic image manipulation tasks.

## Features

- Grayscale Conversion: Convert color images to grayscale.
- Reflection: Create a horizontally reflected version of the image.
- Blurring: Apply a blurring effect to the image.
- Edge Detection: Highlight edges within the image.

## Usage

To use the program, follow these steps:

1. Clone this repository to your local machine.
2. Build the program using the provided instructions in the [Build](#build) section.
3. Run the program with the desired filter and input BMP file.

Example usage:
```bash
./filter -g input.bmp output.bmp
```

## Filters
The program supports the following filters:

```
  -g or --grayscale: Convert the image to grayscale.
  -r or --reflect: Create a horizontally reflected version of the image.
  -b or --blur: Apply a blurring effect to the image.
  -e or --edges: Highlight edges within the image.
```

## Build
To build the program, execute the following command in your terminal:
```
- clang -ggdb3 -gdwarf-4 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -lm -o filter filter.c helpers.c
```
## Contributing
If you would like to contribute to this project, please open an issue or submit a pull request with your changes. Contributions are welcome!

