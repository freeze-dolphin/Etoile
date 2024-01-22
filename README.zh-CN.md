<h5 align="right"> 💫 </h5>
<img width="92" height="92" src="https://arcaea.lowiro.com/img/11_icon.d91d4854.png" align="right" />

[English](./README.md) | [简体中文](./README.zh-CN.md)

# Étoile

读取 ArcCreate 的 `.arcpkg` 格式，自动解压相关资源并生成 `songlist`

支持单个 `.arcpkg` 内塞多首曲目

## 食用方法

这是一个命令行工具，你需要在你系统的终端中执行

以下参数列表，你也可以通过执行 `Etoile --help` 来查看

| 参数名         | 简介         | 例子                              |
|:------------|:-----------|:--------------------------------|
| --input, -i | .arcpkg 文件 | -i ./Pilcrow_.LivingWill.arcpkg |
| --songs, -o | songs 目录   | -o ./songs                      |
| --bg, -b    | 背景图片目录     | -b assets/img/bg                |
| --pack, -p  | 曲包名称       | -p default                      |

| 选项             | 简介          |
|:---------------|:------------|
| --force        | 启用强制模式 [^1] |
| --fix-constant | 启用定数修正 [^2] |
| --help, -h     | 显示帮助信息和版本信息 |

[^1]: 强制模式不会在覆盖背景文件和 `songlist` 文件时提醒用户
[^2]: 有些曲师没有填写定数参数，只是写了一个定数显示文本（如：“Future 10”），我们无法从这样的文本中判断具体定数，因为有些人可能会写类似“Eternity
???”之类的东西；开启此项后，缺失定数参数的谱面将被定为
1.0；若不开启此项参数，缺失定数参数的谱面将被转换程序忽略

## 构建

注意，对于 Windows，由于本人没有现成的 Windows 开发环境，无法对 Windows 平台进行本地编译，只能在自己的电脑进行交叉编译并使用 wine 进行测试；至于
macOS，我既不会交叉编译也没有类似的兼容层来测试，如果你会写 macOS 兼容并且愿意为这个项目支持，那么欢迎 Pull Request～

开发时使用的库版本：

| library       | version                   |
|:--------------|:--------------------------|
| cargs         | 1.1.0 [^3]                |
| zip           | 0.3.1 [^3]                |
| libcyaml      | 1.3.1-1 [^4]              |
| cJSON         | 1.7.15-1 [^4]             |
| libmagickwand | 8:6.9.11.60+dfsg-1.6 [^4] |
| lua           | 5.4.4-3 [^4]              |

[^3]: 从 github 中克隆的源代码
[^4]: 从 debian 12 的仓库中安装

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
   $ sudo apt install lua5.4
   ```

5. 确保 `CMake` 已安装，并执行:

   ```shell
   $ mkdir build && cd build
   $ cmake -DCMAKE_BUILD_TYPE=Release ..
   $ make
   ```

   或者使用项目根目录下的 `Makefile` 完成自动构建

## [TODO](./TODO.md)