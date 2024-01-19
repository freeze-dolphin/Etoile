<h5 align="right"> 💫 </h5>
<img src="https://arcaea.lowiro.com/img/11_icon.d91d4854.png" align="right" />

---

# Étoile

A mini tool written in C that helps convert `.arcpkg` to Arcaea readable `songlist`

## Problem

This program is under very early stage, and you may encounter bugs

## Usage

| args        | desc         | example                         |
|:------------|:-------------|:--------------------------------|
| --input, -i | .arcpkg file | -i ./Pilcrow_.LivingWill.arcpkg |
| --songs, -o | songs dir    | -o ./songs                      |
| --bg, -b    | bg dir       | -b assets/img/bg                |
| --pack, -p  | pack name    | -p default                      |

| options        | desc                            |
|:---------------|:--------------------------------|
| --force        | enable force mode [^1]          |
| --fix-constant | enable auto constant fixer [^2] |
| --help, -h     | show the help message           |

[^1]: After enabling force mode, the program will disable prompts when overwriting `songlist` and background images
[^2]: Some charts are packed without a specific chart constant but a sting like "Future 10", we cannot detect the proper constant from this
since some charters may fill "Eternity 12" and so on into it... By enabling this option, the program will

## Building

Native development libraries version:

| library       | version                               |
|:--------------|:--------------------------------------|
| libcyaml      | 1.3.1                                 |
| cJSON         | 1.7.15                                |
| libmagickwand | 8:6.9.11.60+dfsg-1.6 (from debian 12) |

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

4. Make sure you have CMake installed:

   ```shell
   $ cmake --build cmake-build-debug --target Etoile
   ```

## Roadmap & Plans

Plans are divided into different parts according to their stages of execution in the program.

### Prelude

Before we actually start the conversion...

- [ ] selective extraction:

  Only the song(s) of selected id(s) will be extracted and others will be ignored
  Using the following launch args, Etoile will only extract `ShiMei.SimpleFolds` and  `Arcthesia.Unplayable`

    ```shell
    $ ./Etoile \
    --select "ShiMei.SimpleFolds;Arcthesia.Unplayable" \
    -i ./arcpkg/ACC5.Standard.arcpkg \
    -o ./songs \
    -b ./img/bg/1080 \
    -p default \
    --force
    ```

### In-Operation

During the conversion

#### Resources & Structure

- [x] Auto extract chart resources
- [ ] Log warn while processing charts with scene-control
- [ ] [TODO 1] If PST, PRS and BYD use the same audio / jacket, they should use 'base' as audio / jacket (currently, the program will
  copy `base.ogg` / `base.jpg` and rename it to `0.ogg` etc. if there is PST chart)

#### Chart Constant

- [ ] Ask for user's response when encountering undetectable chart difficulty
- [ ] [TODO 2] or we could make a converter for those charts without a chart constant, which converts "Future 10" to chart constant 10.0

### Outputting

The output of the conversion, the `songlist` in short

- [ ] settings overwriting: (I didn't realize how complicated the structure of `songlist` is until I looked up 'PRAGMATISM'...)

    ```json
    {
      "ratingClass": 3,
      "title_localized": {
        "en": "PRAGMATISM -RESURRECTION-"
      },
      "audioOverride": true,
      "chartDesigner": "THE LAST DREAM",
      "jacketDesigner": "シエラ",
      "jacketOverride": true,
      "bg": "pragmatism3",
      "rating": 11,
      "date": 1639008004,
      "version": "3.10"
    } 
    ```
