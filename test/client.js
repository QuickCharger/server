// 
/**
 * 压测
 * 每工作一段时间就休息一下 观察数据有没有问题
 * 
 * 2024/04/30
 */

let net = require('net')

let [port, ip] = [12345, '127.0.0.1']

// 目标客户量 / 实际客户量
let [targetClientCount, clientCount] = [999, 0]

// 每次发送包的大小
let packLen = 1000 * 1
let packInterval = 10
let sendTimes = -1		// 发送次数 如果达到此值则socket销毁 默认-1不销毁
let chunk = '1234567890'.repeat(packLen / 10)

// 统计
let [cSend, cRecv] = [0, 0]

// 运行时间 / 休息时间 秒
let [runTime, sleepTime] = [10, 3]

//----------------------------------------------------

// 0 not work. 1 work
let workState = 0

let createClient = () => {
	let fdWorking = false
	let c = net.connect(port, ip)
	let t = sendTimes

	let timer = setInterval(() => {
		if (!workState)
			return
		if (!fdWorking)
			return
		--t
		if(t === 0) {
			fdWorking = false
			c.end()
			return
		}
		c.write(chunk)
		cSend += chunk.length
	}, packInterval)

	c.on('close', () => {
		fdWorking = false
		c.destroy()
		clearInterval(timer)
		clientCount--
		// console.log(`client close`)
	}).on('connect', () => {
		fdWorking = true
		// console.log(`client connect`)
	}).on('data', data => {
		// console.log(`client data. send from server length ${data.length}`)
		cRecv += data.length
		// console.log(`client data. send from server: ${data}`)
	}).on('drain', () => {
		// console.log(`client drain`)
	}).on('end', () => {
		// console.log(`client end`)
	}).on('error', () => {
		fdWorking = false
		// console.log(`client error`)
	}).on('lookup', () => {
		console.log(`client lookup`)
	}).on('timeout', () => {
		console.log(`client timeout`)
	})
}

function sleep (ms) {
	return new Promise(resolve => setTimeout(resolve, ms))
}

async function main () {
	setInterval(() => {
		if (clientCount >= targetClientCount)
			return
		createClient()
		clientCount++
	}, 1)

	setInterval(() => {
		let cDiff = cSend - cRecv
		console.log(`diff ${cDiff} send ${cSend} recv ${cRecv}`)
		// cSend = 0
		// cRecv = 0
	}, 1000)

	while (true) {
		workState = 1
		await sleep(1000 * runTime)
		workState = 0
		await sleep(1000 * sleepTime)
		cSend = 0
		cRecv = 0
	}
}

main()
