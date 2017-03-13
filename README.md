### 介绍

一个简洁、高性能、跨平台的 PHP7 代码加密扩展，当前版本为 0.1.0

### 特点

- 编译前解密，简单快速，经实测，几乎不影响性能
- 兼容 OPcache、Xdebug 等其他扩展
- 支持 Linux、macOS、Windows 等系统
- 兼容 Apache、Nginx + PHP-fpm、命令行等运行模式
- 加密算法较简单，这是出于速度考虑，但仍不易解密
- 可只加密核心代码
- 要求 PHP >= 7.0

### 在 Linux、macOS 上安装
```
git clone https://github.com/lihancong/tonyenc.git
cd tonyenc
phpize
./configure
make
将编译好的文件 modules/tonyenc.so 加入配置项 extension=tonyenc.so ，重启 PHP 服务
```

### 在 Windows 上安装

已编译好了以下模块，可供测试（需要 [VC14](https://www.microsoft.com/zh-CN/download/details.aspx?id=48145) 运行库）:
```bash
# php7.0 64位 线程安全版
php_tonyenc_php70_ts_VC14_x64.dll
# php7.0 64位 线程非安全版
php_tonyenc_php70_nts_VC14_x64.dll
```
为保安全，要自己指定密钥：[手动编译方法](http://lihancong.cn/blog/article/104)

### 如何加解密

编译前请在 core.h 中做如下修改:
```c
/* 这里定制你的加密特征头，十六进制哦 */
const u_char tonyenc_header[] = {
        0x66, 0x88, 0xff, 0x4f,
        0x68, 0x86, 0x00, 0x56,
        0x11, 0x16, 0x16, 0x18,
};

/* 这里指定密钥，不限定长度 */
const u_char tonyenc_key[] = {
        0x9f, 0x49, 0x52, 0x00,
        0x58, 0x9f, 0xff, 0x21,
        0x3e, 0xfe, 0xea, 0xfa,
        0xa6, 0x33, 0xf3, 0xc6,
};
```

代码中的 tonyenc.php 是加解密工具:
```bash
# 加密
php tonyenc.php example.php dir/
# 解密
php tonyenc.php -d example.php dir/
```
这样即可加解密 `example.php` 和 `dir` 目录下的所有 php 文件，够简单吧！

### 版权

允许转载、修改、商用，请注明原作者及网址：Tony (http://lihancong.cn/)

这是我开发的第一个扩展，如有不足欢迎指正