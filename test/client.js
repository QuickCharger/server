let net = require('net')

// 每工作一段时间就休息一下 观察数据有没有问题
let working = true

let chunk1k = `1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890`

let cSend = 0
let cRecv = 0
let cDiff = 0
clientSocket1 = net.connect(12345, "127.0.0.1")
clientSocket1
	.on('close', () => {
		console.log(`client close`)
	}).on('connect', () => {
		console.log(`client connect`)
		setInterval(() => {
			if (!working)
				return
			for (let i = 0; i < 100; ++i) {
				clientSocket1.write(chunk1k)
				cSend += chunk1k.length
			}
			// if (clientSocket1.write(chunk1k)) {
			// 	// console.log("send qwer success")
			// } else {
			// 	console.log("send qwer failed")
			// }
		}, 1)
		setInterval(() => {
			cDiff += cSend - cRecv
			console.log(`diff ${cDiff} send ${cSend} recv ${cRecv}`)
			cSend = 0
			cRecv = 0
		}, 1000)

		setInterval(() => {
			working = false
			setTimeout(() => {
				working = true
			}, 3000)
		}, 20000)

		//clientSocket1.write("qwer")
	}).on('data', data => {
		// console.log(`client data. send from server length ${data.length}`)
		cRecv += data.length
		// console.log(`client data. send from server: ${data}`)
	}).on('drain', () => {
		// console.log(`client drain`)
	}).on('end', () => {
		console.log(`client end`)
	}).on('error', () => {
		console.log(`client error`)
	}).on('lookup', () => {
		console.log(`client lookup`)
	}).on('timeout', () => {
		console.log(`client timeout`)
	})



setTimeout(() => {
	clientSocket1.end()
}, 100000000)
