let ligthLeft = 0xFF;
let ligthCenter = 0xFF;
let ligthRight = 0xFF;
let aff7segG1 = 0x00;
let aff7segG2 = 0x00;
let aff7segD1 = 0x00;
let aff7segD2 = 0x00;
let aff7segC1 = 0x00;
let aff7segC2 = 0x00;
let aff7segC3 = 0x00;
let aff7segC4 = 0x00;
let aff7segC5 = 0x00;
let aff7segC6 = 0x00;
let aff7segC7 = 0x00;
let aff7segC8 = 0x00;

function serialPort(num){

	var SerialPort = require('serialport');
	const stream = require('stream');
	var port = new SerialPort('COM' + num, {
	  baudRate: 9600,
	  autoOpen: false
	});
	let connected = false;
	let txt;
	let txt2;
	let send;
	
	port.on('readable', function () {
		
		if(!connected ){
			txt = port.read(7);
			if(txt == null) return;
			let arr = [...txt]
			
			let arrf = "";
			for(let i = 0; i < arr.length ; i++){
					arrf += String.fromCharCode(arr[i]);
				}
			if(txt.toString() == "Contact"){
				$('#Connection').html('Connecté !');
				port.write('a');
				connected = true;
				$('#module').fadeIn();
			}
			else
				$('#Connection').html('Connexion incorrecte');
		}
		else{
			txt2 = port.read(6);
			if(txt2 == null ) return;
			let arr = [...txt2]
			if(arr[0] == 0 && arr[arr.length-1] == 255){
				refreshLeftDisplay(arr);
				$('#Gauche').html('');
				for(let i = 1; i < arr.length - 1 ; i++){
					$('#Gauche').append(parseInt(arr[i],10).toString(2) + ' ');
				}
			}
			else if(arr[0] == 1 && arr[arr.length-1] == 254){
				refreshCenterDisplay(arr);
				$('#Centre').html('');
				for(let i = 1; i < arr.length - 1 ; i++){
					$('#Centre').append(parseInt(arr[i],10).toString(2) + ' ');
				}
			}
			else if(arr[0] == 2 && arr[arr.length-1] == 253){
				refreshRightDisplay(arr);
				$('#Droite').html('');
				for(let i = 1; i < arr.length - 1 ; i++){
					$('#Droite').append(parseInt(arr[i],10).toString(2) + ' ');
				}
			}
			else 
				return;
			
		 	send = [ligthLeft, ligthCenter, ligthRight, aff7segG1, aff7segG2, aff7segD1, aff7segD2, aff7segC1,
		 			aff7segC2, aff7segC3, aff7segC4, aff7segC5, aff7segC6, aff7segC7, aff7segC8 ];
	 		port.write(send, function(err) {
	 			$('#ErreurPort').html('');
			if (err) {
				$('#ErreurPort').html('Error on write: ' + err.message);
				return console.log('Error on write: ', err.message);
			}
		});
		}	
	});

	port.open(function (err) {
		$('#ErreurPort').html('');

		$( "#aff7segD" ).change( aff7segChange.bind(this, '#aff7segD'));
		$( "#aff7segG" ).change( aff7segChange.bind(this, '#aff7segG'));
		$( "#aff7segC1" ).change( aff7segChange.bind(this, '#aff7segC1'));
		$( "#aff7segC2" ).change( aff7segChange.bind(this, '#aff7segC2'));
		$( "#aff7segC3" ).change( aff7segChange.bind(this, '#aff7segC3'));
		$( "#aff7segC4" ).change( aff7segChange.bind(this, '#aff7segC4'));

		if (err) {
			$('#ErreurPort').html('Error opening port: ' + err.message);
			return console.log('Error opening port: ', err.message);
		}

	}); 

} 

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
		$('#ErreurPort').html('');
		if (err) {
			$('#ErreurPort').html('Error on write: ' + err.message);
			return console.log('Error on write: ', err.message);
		}
	});
	console.log('Contact établi');
}

function aff7segChange(id){
	aff7segG1 = $( "#aff7segG" ).val().length > 0 ? parseInt($( "#aff7segG" ).val().charAt(0), 16) : 0;
	aff7segG1 <<= 4;
	aff7segG1 += $( "#aff7segG" ).val().length > 1 ? parseInt($( "#aff7segG" ).val().charAt(1), 16) : 0;
	aff7segG2 = $( "#aff7segG" ).val().length > 2 ? parseInt($( "#aff7segG" ).val().charAt(2), 16) : 0;
	aff7segG2 <<= 4;
	aff7segG2 += $( "#aff7segG" ).val().length > 3 ? parseInt($( "#aff7segG" ).val().charAt(3), 16) : 0;
	aff7segD1 = $( "#aff7segD" ).val().length > 0 ? parseInt($( "#aff7segD" ).val().charAt(0), 16) : 0;
	aff7segD1 <<= 4;
	aff7segD1 += $( "#aff7segD" ).val().length > 1 ? parseInt($( "#aff7segD" ).val().charAt(1), 16) : 0;
	aff7segD2 = $( "#aff7segD" ).val().length > 2 ? parseInt($( "#aff7segD" ).val().charAt(2), 16) : 0;
	aff7segD2 <<= 4;
	aff7segD2 += $( "#aff7segD" ).val().length > 3 ? parseInt($( "#aff7segD" ).val().charAt(3), 16) : 0;
	aff7segC1 = $( "#aff7segC1" ).val().length > 0 ? parseInt($( "#aff7segC1" ).val().charAt(0), 16) : 0;
	aff7segC1 <<= 4;
	aff7segC1 += $( "#aff7segC1" ).val().length > 1 ? parseInt($( "#aff7segC1" ).val().charAt(1), 16) : 0;
	aff7segC2 = $( "#aff7segC1" ).val().length > 2 ? parseInt($( "#aff7segC1" ).val().charAt(2), 16) : 0;
	aff7segC2 <<= 4;
	aff7segC2 += $( "#aff7segC2" ).val().length > 0 ? parseInt($( "#aff7segC2" ).val().charAt(0), 16) : 0;
	aff7segC3 = $( "#aff7segC2" ).val().length > 1 ? parseInt($( "#aff7segC2" ).val().charAt(1), 16) : 0;
	aff7segC3 <<= 4;
	aff7segC3 += $( "#aff7segC2" ).val().length > 2 ? parseInt($( "#aff7segC2" ).val().charAt(2), 16) : 0;
	aff7segC4 = $( "#aff7segC3" ).val().length > 0 ? parseInt($( "#aff7segC3" ).val().charAt(0), 16) : 0;
	aff7segC4 <<= 4;
	aff7segC4 += $( "#aff7segC3" ).val().length > 1 ? parseInt($( "#aff7segC3" ).val().charAt(1), 16) : 0;
	aff7segC5 = $( "#aff7segC3" ).val().length > 2 ? parseInt($( "#aff7segC3" ).val().charAt(2), 16) : 0;
	aff7segC5 <<= 4;
	aff7segC5 += $( "#aff7segC3" ).val().length > 3 ? parseInt($( "#aff7segC3" ).val().charAt(3), 16) : 0;
	aff7segC6 = $( "#aff7segC3" ).val().length > 4 ? parseInt($( "#aff7segC3" ).val().charAt(4), 16) : 0;
	aff7segC6 <<= 4;
	aff7segC6 += $( "#aff7segC4" ).val().length > 0 ? parseInt($( "#aff7segC4" ).val().charAt(0), 16) : 0;
	aff7segC7 = $( "#aff7segC4" ).val().length > 1 ? parseInt($( "#aff7segC4" ).val().charAt(1), 16) : 0;
	aff7segC7 <<= 4;
	aff7segC7 += $( "#aff7segC4" ).val().length > 2 ? parseInt($( "#aff7segC4" ).val().charAt(2), 16) : 0;
	aff7segC8 = $( "#aff7segC4" ).val().length > 3 ? parseInt($( "#aff7segC4" ).val().charAt(3), 16) : 0;
	aff7segC8 <<= 4;
	aff7segC8 += $( "#aff7segC4" ).val().length > 4 ? parseInt($( "#aff7segC4" ).val().charAt(4), 16) : 0;

}
