<h5 align="right"> 💫 </h5>
<img width="92" height="92" src="https://arcaea.lowiro.com/img/11_icon.d91d4854.png" align="right" />

[English](./README.en.md) | [简体中文](./README.md)

# Étoile

A mini tool written in C that helps convert `.arcpkg` to Arcaea readable `songlist`

## Usage

### Command-line arguments

| args          | desc               | example                         |
|:--------------|:-------------------|:--------------------------------|
| --input, -i   | .arcpkg file       | -i ./Pilcrow_.LivingWill.arcpkg |
| --songs, -o   | songs dir          | -o ./songs                      |
| --bg, -b      | bg dir             | -b assets/img/bg                |
| --pack, -p    | pack name          | -p default                      |
| --lua, -l     | Lua script path    | -l aff_processor.lua            |
| --version, -v | version when added | -v 2.33                         |

| options        | desc                            |
|:---------------|:--------------------------------|
| --force        | enable force mode [^1]          |
| --fix-constant | enable auto constant fixer [^2] |
| --help, -h     | show the help message           |

[^1]: After enabling force mode, the program will disable prompts when overwriting `songlist` and background images
[^2]: Some charts are packed without a specific chart constant but a sting like "Future 10", we cannot detect the proper constant from this
since some charters may fill "Eternity 12" and so on into it... By enabling this option, the program will use 1.0 as chart constant for them

## Building

Native development libraries version:

| library       | version                   |
|:--------------|:--------------------------|
| cargs         | 1.1.0 [^3]                |
| zip           | 0.3.1 [^3]                |
| libcyaml      | 1.3.1-1 [^4]              |
| cJSON         | 1.7.15-1 [^4]             |
| libmagickwand | 8:6.9.11.60+dfsg-1.6 [^4] |
| lua           | 3.0.8-2 [^4]              |

[^3]: from github
[^4]: from debian 12

1. Install [`libcyaml`](https://github.com/tlsa/libcyaml):

   ```shell
   $ sudo apt install libcyaml-dev
   ```

2. Install [`libcjson`](https://github.com/DaveGamble/cJSON/):

   ```shell
   $ sudo apt install libcjson-dev
   ```

3. Install [`MagickWand`](http://www.imagemagick.org/script/magick-wand.php):

   ```shell
   $ sudo apt install libmagickwand-dev
   ```

4. Install [`GNU Guile 3.0`](https://www.gnu.org/software/guile/):

    ```shell
   $ sudo apt install guile-3.0-dev
   ```

5. Make sure you have had `CMake` installed:

   ```shell
   $ mkdir build && cd build
   $ cmake -DCMAKE_BUILD_TYPE=Release ..
   $ make
   ```

   You can also use the `Makefile` in project root to build automatically

## [TODO](./TODO.md)