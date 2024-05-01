// 
/**
 * 压测
 * 每工作一段时间就休息一下 观察数据有没有问题
 * 
 * 2024/04/30
 */

let net = require('net')

let [port, ip] = [12345, '127.0.0.1']

// 运行时间 / 休息时间 秒
let [runTime, sleepTime] = [10, 3]

// 目标客户量 / 实际客户量
let [targetClientCount, clientCount] = [999, 0]

// 每次发送包的大小
let packLen = 1000 * 1
let packInterval = 10
let chunk = '1234567890'.repeat(packLen / 10)

// 统计
let [cSend, cRecv, cDiff] = [0, 0, 0]

//----------------------------------------------------

// 0 not work. 1 work
let workState = 0

let createClient = () => {
	let fdWorking = false
	let c = net.connect(port, ip)
	c.on('close', () => {
		fdWorking = false
		console.log(`client close`)
	}).on('connect', () => {
		fdWorking = true
		console.log(`client connect`)
	}).on('data', data => {
		// console.log(`client data. send from server length ${data.length}`)
		cRecv += data.length
		// console.log(`client data. send from server: ${data}`)
	}).on('drain', () => {
		// console.log(`client drain`)
	}).on('end', () => {
		console.log(`client end`)
	}).on('error', () => {
		fdWorking = false
		console.log(`client error`)
	}).on('lookup', () => {
		console.log(`client lookup`)
	}).on('timeout', () => {
		console.log(`client timeout`)
	})
	setInterval(() => {
		if (!working)
			return
		if (!fdWorking)
			return
		c.write(chunk)
		cSend += chunk.length
	}, packInterval)
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
		cDiff += cSend - cRecv
		console.log(`diff ${cDiff} send ${cSend} recv ${cRecv}`)
		cSend = 0
		cRecv = 0
	}, 1000)

	while (true) {
		workState = 1
		await sleep(runTime)
		workState
		await sleep(sleepTime)
		console.log(`init`)
		cSend = 0
		cRecv = 0
	}
}

main()
