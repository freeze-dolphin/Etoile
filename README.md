<h5 align="right"> 💫 </h5>
<img width="92" height="92" src="https://arcaea.lowiro.com/img/11_icon.d91d4854.png" align="right" />

[English](./README.en.md) | [简体中文](./README.md)

# Étoile

读取 ArcCreate 的 `.arcpkg` 格式，自动解压相关资源并生成 `songlist`，并且支持单个 `.arcpkg` 内塞多首曲目

此工具旨在将 `.arcpkg` 转换为 `Arcaea` 可读的谱面，去除部分不可实现的 `scenecontrol` 和 `timinggroup` 的同时保留原谱的可玩性

使用过程中碰到任何问题，欢迎在 [Issues](https://github.com/freeze-dolphin/Etoile/issues) 中交流，中英文皆可 ╮(￣▽￣)╭

## 食用方法

这是一个命令行工具，你需要在你系统的终端中执行

以下参数列表，你也可以通过执行 `Etoile --help` 来查看

参数名中带 `*` 的是必须参数，这些参数控制程序的主要行为

| 参数名           | 简介             | 例子                              |
|:--------------|:---------------|:--------------------------------|
| --input, -i * | .arcpkg 文件     | -i ./Pilcrow_.LivingWill.arcpkg |
| --songs, -o * | songs 目录       | -o ./songs                      |
| --bg, -b *    | 背景图片目录         | -b assets/img/bg                |
| --pack, -p *  | 曲包名称           | -p default                      |
| --lua, -l     | Lua 脚本路径 [^1]  | -l ./aff_processor.lua          |
| --version, -v | 曲目加入时的游戏版本[^2] | -v 2.33                         |

| 选项             | 简介          |
|:---------------|:------------|
| --force        | 启用强制模式[^3]  |
| --fix-constant | 启用定数修正[^4]  |
| --help, -h     | 显示帮助信息和版本信息 |

一般来说，推荐开启定数修正

下面是一个简单的使用示例：

- Windows：

    ```batch
    Etoile -i Pilcrow_.LivingWill.arcpkg -o songs -b bg -p default -l aff_processor.lua -v 3.4 --force --fix-constant
    ```

- Linux：

    ```shell
    $ ./Etoile \
        -i ./Pilcrow_.LivingWill.arcpkg \
        -o ./songs \
        -b ./bg \
        -p default \
        -l aff_processor.lua \
        -v 3.4 \
        --force \
        --fix-constant
    ```

这两则示例功能相同

[^1]: 若不写此参数，程序将读取执行目录内的 'aff_processor.lua'，如果文件不存在将禁用此功能

[^2]: 若不写此参数，程序将默认为 1.0 版本

[^3]: 强制模式不会在覆盖背景文件和 `songlist` 文件时提醒用户

[^4]: 有些曲师没有填写定数参数，只是写了一个定数显示文本（如：“Future 10”），我们无法从这样的文本中判断具体定数，因为有些人可能会写类似“Eternity
???”之类的东西；开启此项后，缺失定数参数的谱面将被定为
1.0；若不开启此项参数，缺失定数参数的谱面将被转换程序忽略

## 构建

注意，对于 Windows，由于本人没有现成的 Windows 开发环境，无法对 Windows 平台进行本地编译，只能在自己的电脑进行交叉编译并使用 wine 进行测试

至于 macOS，我既不会交叉编译也没有类似的兼容层来测试，如果你会写 macOS
兼容并且愿意为这个项目支持，那么欢迎 [Pull Request](https://github.com/freeze-dolphin/Etoile/pulls)～

开发时使用的库版本：

| library       | version                  |
|:--------------|:-------------------------|
| cargs         | 1.1.0[^5]                |
| zip           | 0.3.1[^5]                |
| libcyaml      | 1.3.1-1[^6]              |
| cJSON         | 1.7.15-1[^6]             |
| libmagickwand | 8:6.9.11.60+dfsg-1.6[^6] |
| lua           | 5.4.4-3[^6]              |

[^5]: 从 github 中克隆的源代码

[^6]: 从 debian 12 的仓库中安装的软件包版本

1. 安装 [`libcyaml`](https://github.com/tlsa/libcyaml):

   ```shell
   $ sudo apt install libcyaml-dev
   ```
2. 安装 [`libcjson`](https://github.com/DaveGamble/cJSON/):

   ```shell
   $ sudo apt install libcjson-dev
   ```
3. 安装 [`MagickWand`](http://www.imagemagick.org/script/magick-wand.php):

   ```shell
   $ sudo apt install libmagickwand-dev
   ```
4. 安装 [`Lua`](https://www.lua.org/):

   ```shell
   $ sudo apt install liblua5.4-dev
   ```
5. 确保 `CMake` 已安装，并执行:

   ```shell
   $ mkdir build && cd build
   $ cmake -DCMAKE_BUILD_TYPE=Release ..
   $ make
   ```

   或者使用项目根目录下的 `Makefile` 完成自动构建

## [TODO](./TODO.md)
