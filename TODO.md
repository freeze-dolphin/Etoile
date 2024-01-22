# Roadmap & TODOs

Plans are divided into different parts according to their stages of execution in the program.

## Prelude

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

## In-Operation

During the conversion

### Resources & Structure

- [x] Auto extract chart resources
- [x] Detect and extract sound effects of arctaps
- [ ] [TODO 1] If PST, PRS and BYD use the same audio / jacket, they should use 'base' as audio / jacket (currently, the program will
  copy `base.ogg` / `base.jpg` and rename it to `0.ogg` etc. if there is PST chart)

### Chart

- [x] Custom Lua script support for aff preprocessing
- [ ] Ask for user's response when encountering undetectable chart difficulty
- [ ] [TODO 2] or we could make a converter for those charts without a chart constant, which converts "Future 10" to chart constant 10.0

#### Lua

- [x] Restrict some parameters to be '%.2f' formatted
- [x] If the arcpkg extracts without required wav file (to be used in 'arc()'), the `hitsound` field will be replaced with 'none'

## Outputting

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
