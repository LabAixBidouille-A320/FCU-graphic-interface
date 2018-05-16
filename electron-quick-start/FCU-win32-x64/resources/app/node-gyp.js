var SerialPort = require('serialport');
var port = new SerialPort('COM6', {
  baudRate: 9600,
  autoOpen: false
});

port.on('readable', function () {
 	let txt = port.read()
  console.log('Data:', txt);
  $('#arduino').html(txt);
  test = false;
  port.write('a', function(err) {
		if (err) {
		return console.log('Error on write: ', err.message);
		}
	});
});

port.open(function (err) {
	
	if (err) {
	return console.log('Error opening port: ', err.message);
	}


	// Because there's no callback to write, write errors will be emitted on the port:
	port.write('a', function(err) {
	if (err) {
	return console.log('Error on write: ', err.message);
	}
	console.log('message written');
	});

}); 

function establishConctact(){
	console.log('Etablissement du contact');
	let reponse = '';
	let count = 0;
	while(test){
		if(++count > 50000000){
			console.log('Attente de réponse');
			$('#arduino').html('Attente de réponse');
			count = 0;
		}
	}
	
	console.log('message reçu : ' + reponse);
	
	port.write('a', function(err) {
		if (err) {
		return console.log('Error on write: ', err.message);
		}
	});
	console.log('Contact établi');
}