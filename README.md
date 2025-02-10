RFC959 定义了 文件传输协议（FTP, File Transfer Protocol），于1985年发布，是互联网早期用于在客户端和服务器之间传输文件的核心协议。

核心概念
客户端-服务器模型

客户端通过控制连接（默认端口 21）发送命令，服务器返回响应。

数据连接（端口 20 或动态端口）用于实际传输文件或目录列表。

两种传输模式

主动模式（Active Mode）
服务器主动从端口 20 连接到客户端指定的端口（可能被防火墙拦截）。

被动模式（Passive Mode）
服务器开启一个随机端口，客户端主动连接（适用于客户端位于防火墙/NAT后）。

传输类型

ASCII 模式：自动转换文本文件的换行符（如 CR/LF ↔ LF）。

二进制模式：原样传输文件（如图像、压缩包）。

协议命令示例
控制命令（通过明文发送）：

USER <username>：认证用户名。

PASS <password>：认证密码。

LIST：请求目录列表。

RETR <filename>：下载文件。

STOR <filename>：上传文件。

TYPE A/I：设置传输模式（ASCII/二进制）。

PASV：切换到被动模式。

响应码

2xx：成功（如 200 Command OK）。

3xx：需进一步操作（如 331 Password required）。

4xx/5xx：临时或永久错误（如 550 Permission denied）。

安全性问题
明文传输：用户名、密码、文件内容均未加密，易被窃听。

替代方案：

SFTP（SSH File Transfer Protocol）：基于 SSH 加密。

FTPS（FTP over SSL/TLS）：通过 SSL/TLS 加密 FTP 流量。