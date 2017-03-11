### 介绍

一个简洁、高性能、跨平台的 PHP7 代码加密扩展，当前版本为 0.1.0

### 特点

- 经实测，几乎不影响性能
- 支持 Linux、macOS、Windows 等32/64位系统
- 支持线程安全与非安全，支持命令行模式
- 加密算法较简单，这是处于速度考虑，但仍不易解密(欢迎纠正)
- 可只加密核心代码

### 要求

- PHP >= 7.0

### 在 Linux、macOS 上安装

1. `git clone https://github.com/lihancong/tonyenc.git`
2. `phpize`
3. `./configure`
4. `make`
5. 将编译好的文件 `modules/tonyenc.so` 加入到 `php.ini` 配置，重启 PHP 服务即可


### 在 Windows 上安装

已编译好了以下几种，可直接使用（需要 VC14 运行库）：

1. tonyenc.php70.x64.ts.dll
2. tonyenc.php70.x64.nts.dll 
3. tonyenc.php70.x86.ts.dll 
4. tonyenc.php70.x86.nts.dll 

手动编译待补充

### 如何加解密

先将 php 加入系统环境变量

代码中的 tonyenc.php 可以直接用来加解密:

1. 加密 example.php 和 dir 目录下所有 php 文件：`php tonyenc.php example.php dir/`
2. 解密 example.php 和 dir 目录下所有 php 文件：`php tonyenc.php -d example.php dir/`

够简单吧！

### 版权

允许转载、修改、商用，请注明作者：Tony (http://lihancong.cn/)