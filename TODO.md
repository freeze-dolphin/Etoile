# TODOs

根据执行时的不同阶段，TODO 被分为不同的部分

## 转换操作之前

- [ ] 选择性提取:

  只有选中的 ID 代表的曲目才会被提取
  在这个例子中，只有 `ShiMei.SimpleFolds` 和 `Arcthesia.Unplayable` 会被提取

    ```shell
    $ ./Etoile \
    --select "ShiMei.SimpleFolds;Arcthesia.Unplayable" \
    -i ./arcpkg/ACC5.Standard.arcpkg \
    -o ./songs \
    -b ./img/bg/1080 \
    -p default \
    --force
    ```

## 转换过程中

### 资源和结构

- [x] 自动提取曲目相关资源
- [x] 自动提取特殊天键的打击音效
- [ ] [TODO 1] 对于同一张曲目有多个音频、曲绘的情况进行处理（目前无论该曲目是否有 PST，PRS 和 BYD 难度，资源文件都使用 'base'）

### 谱面

- [x] 使用自定义 Lua 脚本对 aff 进行处理，去除无效语句并进行语法转换
- [x] 当程序遇到无法判断定数的谱面时，自动修正为 1.0

#### Lua

- [x] 强制某些参数保留两位小数（写完才发现现在不用强制两位小数了？）
- [x] 如果在谱面中用到了某些打击音效但是 `.arcpkg` 中没有，将打击音效设为 'none'
- [x] 去除无效的 'scenecontrol' 语句
- [x] 去除无效的 'timinggroup' 参数，并将 'ArcCreate' 的参数格式转换为 'Arcaea' 的

## 输出 (songlist)

- [ ] 选项覆盖（点名白魔王 BYD）

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
