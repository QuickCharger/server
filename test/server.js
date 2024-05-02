let net = require('net')

let port = 12345

let CreateServer = () => {
	net.createServer((inSocket) => {
		inSocket
		.on('close', ()=>{
			// console.log(`insocket close`)
		}).on('connect', ()=>{
			// console.log(`insocket connect`)
		}).on('data', data =>{
			// console.log(`insocket data`)
			inSocket.write(data)
		}).on('drain', ()=>{
			//console.log(`insocket drain`)
		}).on('end', ()=>{
			inSocket.end()
			console.log(`insocket end`)
		}).on('error', ()=>{
			console.log(`insocket error`)
		}).on('lookup', ()=>{
			//console.log(`insocket lookup`)
		}).on('timeout', ()=>{
			console.log(`insocket timeout`)
		})
	}).listen({host:"0.0.0.0", port}, () => {
		console.log(`server listen port ${port} success`)
	}).on("error", (e)=>{
		console.log(`server error`)
		console.log(e)
	}).on("close", ()=>{
		console.log(`server close`)
	}).on('connection', ()=>{
		//console.log(`server connection`)
	}).on("listening", ()=>{ 
		console.log(`server listening`)
	})
}
CreateServer()
