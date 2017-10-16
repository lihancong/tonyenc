## 介绍

一个简洁、高性能、跨平台的 PHP7 代码加密扩展，当前版本为 0.2.2

## 特点

- 简单快速，经实测，几乎不影响性能
- 兼容 OPcache、Xdebug 等其他扩展
- 兼容 Linux、macOS、Windows 等系统
- 兼容 Apache、Nginx + PHP-fpm、命令行等运行模式
- 加密算法较简单，这是出于速度考虑，但仍不易解密
- 若项目的 php 文件很多，建议只加密部分重要代码文件
- 要求 PHP >= 7.0

## 安装

编译前请在 core.h 中做如下修改:
```c
/* 这里定制你的加密特征头，不限长度，十六进制哦 */
const u_char tonyenc_header[] = {
        0x66, 0x88, 0xff, 0x4f,
        0x68, 0x86, 0x00, 0x56,
        0x11, 0x16, 0x16, 0x18,
};

/* 这里指定密钥，长一些更安全 */
const u_char tonyenc_key[] = {
        0x9f, 0x49, 0x52, 0x00,
        0x58, 0x9f, 0xff, 0x21,
        0x3e, 0xfe, 0xea, 0xfa,
        0xa6, 0x33, 0xf3, 0xc6,
};
```

#### 在 Linux、macOS 上编译
```
git clone https://github.com/lihancong/tonyenc.git
cd tonyenc
phpize
./configure
make
```
将编译好的文件 modules/tonyenc.so 加入到配置项 extension=tonyenc.so，重启 PHP 服务

#### 在 Windows 上编译

已编译了以下模块，可供测试（这里的密钥与源代码中的不同，需要安装有 [VC14 运行库](https://www.microsoft.com/zh-CN/download/details.aspx?id=48145)）:
```bash
# php7.0 64位 线程安全版
php_tonyenc_php70_ts_VC14_x64.dll
# php7.0 64位 线程非安全版
php_tonyenc_php70_nts_VC14_x64.dll
```
[手动编译方法](http://lihancong.cn/blog/article/104)


## 加密

代码中的 `tonyenc.php` 是加密工具:
```bash
php tonyenc.php example.php dir/
```
这样即可加密 `example.php` 和 `dir` 目录下的所有 php 文件，PHP 在运行它们时会自动解密，够简单吧！

## 下一步

有时间的时候把加密方式改为加密成 OPcache 文件

## 版权

允许转载、修改、商用，请注明原作者及网址：Tony (http://lihancong.cn/)

这是我开发的第一个扩展，如有不足欢迎指正 :)
