## 介绍

一个高性能、跨平台的 PHP7 代码加密扩展

## 特点

- 经实测，几乎不影响性能
- 支持 Linux、macOS、Windows 等32/64位系统
- 支持线程安全与非安全，支持命令行模式
- 加密算法较简单，这是处于速度考虑，但仍不易破解(欢迎纠正)

## 要求

- PHP >= 7.0

## 在 Linux、macOS 上安装

1. `git clone https://github.com/lihancong/tonyenc.git`
2. `phpize`
3. `./configure`
4. `make`
5. 将编译好的文件 `modules/tonyenc.so` 加入到 `php.ini` 配置


## 在 Windows 上安装

我已经编译好了几个版本，可直接使用（需要 VC14运行库）


## 版权

允许转载、修改、商用，请注明作者：Tony (http://lihancong.cn/)