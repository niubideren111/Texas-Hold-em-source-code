# 🎮 Unity 客户端 - 德州扑克游戏

本目录包含《实时多人德州扑克》的 **Unity 客户端完整源码**，使用 C# 开发，支持 Android / iOS / PC 平台。

---

## 📦 环境要求

| 组件 | 版本要求 |
| :--- | :--- |
| **Unity 编辑器** | 2019.4 或更高版本 (推荐 2020.3 LTS) |
| **操作系统** | Windows 10 / macOS 11+ / Ubuntu 20.04 |
| **目标平台** | Android, iOS, Windows, macOS |

---

## 🚀 快速开始（5分钟运行客户端）

### 第一步：用 Unity 打开项目

```bash
# 1. 确保您已克隆完整仓库
git clone https://github.com/pokerdeveloper/Texas-Hold-em-Poker-Source-Code.git
cd Texas-Hold-em-Poker-Source-Code/client

# 2. 打开 Unity Hub，点击 "添加项目" → 选择本目录 (client 文件夹)
# 3. 等待 Unity 导入所有资源（首次打开需 2-5 分钟）
第二步：配置服务器地址（关键步骤）
客户端需要连接到一个运行中的游戏服务端才能进行对战。请按以下步骤修改服务器地址：

在 Unity 编辑器中，打开目录：Assets/Scripts/Network/

找到 GameServerConfig.cs 文件，双击打开

修改其中的服务器地址配置：
// 请根据您的实际服务端部署地址修改
public static class GameServerConfig
{
    // 开发测试环境（本地服务端）
    public static string ServerHost = "127.0.0.1";
    public static int ServerPort = 8080;
    
    // 如果是连接作者提供的演示服务器，请联系获取地址和端口
    // public static string ServerHost = "demo.poker.com";
    // public static int ServerPort = 8080;
    
    // WebSocket 连接地址（完整）
    public static string WebSocketUrl = $"ws://{ServerHost}:{ServerPort}/ws";
}

第三步：运行游戏
在 Unity 编辑器中，打开场景：Assets/Scenes/Login.unity

点击顶部工具栏的 ▶ 播放按钮，即可在编辑器中直接试玩

如需打包到手机/PC：

点击 File → Build Settings

选择目标平台（Android / iOS / PC）

点击 Build 生成安装包

🔧 常见问题
Q1：打开项目后出现脚本编译错误？
A：请检查 Unity 版本是否为 2019.4 或更高。若仍报错，请尝试：Assets → Reimport All。

Q2：连接服务器失败 / 无法登录？
A：请依次检查：

服务端是否已启动（运行 ./poker_server）

防火墙是否开放了对应端口（默认 8080）

GameServerConfig.cs 中的 IP 和端口是否正确

Q3：在哪里修改游戏界面 / 添加新功能？
A：主要代码位于：

Assets/Scripts/UI/ → 所有 UI 界面逻辑

Assets/Scripts/Game/ → 牌桌逻辑、动画控制

Assets/Scripts/Network/ → WebSocket 通信协议

Q4：是否可以商用 / 二次分发？
A：本客户端源码作为完整项目的一部分，商用需购买商业授权。具体请联系：Telegram @alibabama401

📞 获取技术支持
商务合作 / 完整服务端对接：联系 Telegram @alibabama401

技术问题：请先查阅 ../Doc/ 目录下的协议与部署文档

📄 客户端功能清单（当前版本）
模块	功能
登录/注册	账号密码、游客登录
大厅	金币场入口、俱乐部入口、商城
牌桌	9人德州、实时下注/弃牌/加注、动画特效
俱乐部	创建/加入、好友约局
个人中心	战绩、金币记录、头像
祝您开发愉快！ 🎉

如有任何问题，欢迎通过 Issue 或联系作者反馈。

text

---

## 使用步骤

1. 打开您的仓库：`https://github.com/pokerdeveloper/Texas-Hold-em-Poker-Source-Code`
2. 进入 `client` 文件夹
3. 点击 **Add file** → **Create new file**
4. 在文件名处输入：`README.md`
5. 将上面的**全部代码**复制粘贴到内容框中
6. 滚动到底部，点击 **Commit new file**

完成！之后任何人访问 `client/` 目录，都会直接看到这份格式完整的说明文档。
本回答由 AI 生成，内容仅供参考，请仔细甄别。




---

## 使用步骤

1. 打开您的仓库：`https://github.com/pokerdeveloper/Texas-Hold-em-Poker-Source-Code`
2. 进入 `client` 文件夹
3. 点击 **Add file** → **Create new file**
4. 在文件名处输入：`README.md`
5. 将上面的**全部代码**复制粘贴到内容框中
6. 滚动到底部，点击 **Commit new file**

完成！之后任何人访问 `client/` 目录，都会直接看到这份格式完整的说明文档。