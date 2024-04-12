垃圾桶服务器，负责的功能有；
	ClientServer发来的消息转发给DatabaseServer
	AuthServer请求用户数据
	后期如果有需要，可以给服务器添加消息广播功能。

DustbinServer有所有ClientServer的数据，验证ClientServer的有效性。
