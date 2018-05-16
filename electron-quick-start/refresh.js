function refreshLeftDisplay(arr){
	if((arr[1] | 0xFE) == 0xFE) 
		$('#ILS-G').html('ON');
	else 
		$('#ILS-G').html('OFF');

	if((arr[1] | 0xFD) == 0xFD)
		$('#FD-G').html('ON');
	else
		$('#FD-G').html('OFF');

	if((arr[1] | 0xEF) == 0xEF)
		$('#switch1G').html('PULL');
	else if((arr[1] | 0xDF) == 0xDF)
		$('#switch1G').html('PUSH');
	else
		$('#switch1G').html('Neutre');

	if((arr[1] | 0xBF) == 0xBF)
		$('#switch1cranG').html('hPa');
	else if((arr[1] | 0x7F) == 0x7F)
		$('#switch1cranG').html('in Hg');
	else
		$('#switch1cranG').html('?');

	if((arr[2] | 0x7F) == 0x7F) 
		$('#CSTR-G').html('ON');
	else 
		$('#CSTR-G').html('OFF');

	if((arr[2] | 0xBF) == 0xBF) 
		$('#WPT-G').html('ON');
	else 
		$('#WPT-G').html('OFF');

	
	if((arr[2] | 0xDF) == 0xDF) 
		$('#VORD-G').html('ON');
	else
		$('#VORD-G').html('OFF');

	if((arr[2] | 0xEF) == 0xEF) 
		$('#NDB-G').html('ON');
	else 
		$('#NDB-G').html('OFF');

	if((arr[2] | 0xF7) == 0xF7) 
		$('#ARPT-G').html('ON');
	else 
		$('#ARPT-G').html('OFF');

	if((arr[2] | 0xFB) == 0xFB) 
		$('#switch2G').html('LS');
	else if((arr[2] | 0xFD) == 0xFD)
		$('#switch2G').html('VOR');
	else if((arr[2] | 0xFE) == 0xFE)
		$('#switch2G').html('NAV');
	else if((arr[3] | 0x7F) == 0x7F)
		$('#switch2G').html('ARC');
	else if((arr[3] | 0xBF) == 0xBF)
		$('#switch2G').html('PLAN');

	if((arr[3] | 0xDF) == 0xDF) 
		$('#switch3G').html('10');
	else if((arr[3] | 0xEF) == 0xEF)
		$('#switch3G').html('20');
	else if((arr[3] | 0xF7) == 0xF7)
		$('#switch3G').html('40');
	else if((arr[3] | 0xFB) == 0xFB)
		$('#switch3G').html('80');
	else if((arr[3] | 0xFD) == 0xFD)
		$('#switch3G').html('160');
	else if((arr[3] | 0xFE) == 0xFE)
		$('#switch3G').html('320');

	if((arr[4] | 0x7F) == 0x7F) 
		$('#switch4G').html('ADF');
	else if((arr[4] | 0xBF) == 0xBF)
		$('#switch4G').html('VOR');
	else 
		$('#switch4G').html('OFF');


	LEDcheckedLeft();
}

function LEDcheckedLeft(){
	ligthLeft = $('#LED-ILS-G').is(':checked') ? ligthLeft | 0x40 : ~(~ligthLeft | 0x40);
	ligthLeft = $('#LED-FD-G').is(':checked') ? ligthLeft | 0x80 : ~(~ligthLeft | 0x80);
	ligthLeft = $('#LED-CSTR-G').is(':checked') ? ligthLeft | 0x08 : ~(~ligthLeft | 0x08);
	ligthLeft = $('#LED-WPT-G').is(':checked') ? ligthLeft | 0x20 : ~(~ligthLeft | 0x20);
	ligthLeft = $('#LED-VORD-G').is(':checked') ? ligthLeft | 0x10 : ~(~ligthLeft | 0x10);
	ligthLeft = $('#LED-NDB-G').is(':checked') ? ligthLeft | 0x02 : ~(~ligthLeft | 0x02);
	ligthLeft = $('#LED-ARPT-G').is(':checked') ? ligthLeft | 0x04 : ~(~ligthLeft | 0x04);
}

function refreshCenterDisplay(arr){

	if((arr[1] | 0x7F) == 0x7F)
		$('#switch1C').html('PUSH');
	else if((arr[1] | 0xBF) == 0xBF)
		$('#switch1C').html('PULL');
	else
		$('#switch1C').html('Neutre');

	if((arr[1] | 0xF7) == 0xF7)
		$('#switch2C').html('PUSH');
	else if((arr[1] | 0xFB) == 0xFB)
		$('#switch2C').html('PULL');
	else
		$('#switch2C').html('Neutre');

	if((arr[2] | 0x7F) == 0x7F) 
		$('#LOC-C').html('ON');
	else 
		$('#LOC-C').html('OFF');

	if((arr[2] | 0xBF) == 0xBF)
		$('#HDG-C').html('ON');
	else 
		$('#HDG-C').html('OFF');

	if((arr[2] | 0xDF) == 0xDF) 
		$('#AP1-C').html('ON');
	else 
		$('#AP1-C').html('OFF');

	if((arr[2] | 0xEF) == 0xEF)
		$('#AP2-C').html('ON');
	else 
		$('#AP2-C').html('OFF');

	if((arr[2] | 0xF7) == 0xF7)
		$('#ATHR-C').html('ON');
	else 
		$('#ATHR-C').html('OFF');

	if((arr[2] | 0xFB) == 0xFB)
		$('#METRIC-ALT-C').html('ON');
	else 
		$('#METRIC-ALT-C').html('OFF');

	if((arr[2] | 0xFD) == 0xFD)
		$('#EXPED-C').html('ON');
	else 
		$('#EXPED-C').html('OFF');

	if((arr[2] | 0xFE) == 0xFE)
		$('#APPR-C').html('ON');
	else 
		$('#APPR-C').html('OFF');

	if((arr[3] | 0x7F) == 0x7F) 
		$('#switch3C').html('PUSH');
	else if((arr[3] | 0xBF) == 0xBF) 
		$('#switch3C').html('PULL');
	else
		$('#switch3C').html('Neutre');

	if((arr[3] | 0xF7) == 0xF7) 
		$('#switch4C').html('PUSH');
	else if((arr[3] | 0xFB) == 0xFB) 
		$('#switch4C').html('PULL');
	else
		$('#switch4C').html('Neutre');

	if((arr[4] | 0x7F) == 0x7F) 
		$('#switch3cran').html('100');
	else if((arr[4] | 0xBF) == 0xBF) 
		$('#switch3cran').html('1000');
	else
		$('#switch3cran').html('?');

	if((arr[4] | 0xDF) == 0xDF) 
		$('#switch5G').html('ADF');
	else if((arr[4] | 0xEF) == 0xEF)
		$('#switch5G').html('VOR');
	else 
		$('#switch5G').html('OFF');

	if((arr[4] | 0xF7) == 0xF7) 
		$('#switch5D').html('ADF');
	else if((arr[4] | 0xFB) == 0xFB)
		$('#switch5D').html('VOR');
	else 
		$('#switch5D').html('OFF');

	LEDcheckedCenter();
	
}

function LEDcheckedCenter(){
	ligthCenter = $('#LED-LOC-C').is(':checked') ? ligthCenter | 0x04 : ~(~ligthCenter | 0x04);
	ligthCenter = $('#LED-AP1-C').is(':checked') ? ligthCenter | 0x02 : ~(~ligthCenter | 0x02);
	ligthCenter = $('#LED-AP2-C').is(':checked') ? ligthCenter | 0x08 : ~(~ligthCenter | 0x08);
	ligthCenter = $('#LED-ATHR-C').is(':checked') ? ligthCenter | 0x10 : ~(~ligthCenter | 0x10);
	ligthCenter = $('#LED-EXPED-C').is(':checked') ? ligthCenter | 0x20 : ~(~ligthCenter | 0x20);
	ligthCenter = $('#LED-APPR-C').is(':checked') ? ligthCenter | 0x40 : ~(~ligthCenter | 0x40);
}

function refreshRightDisplay(arr){
	if((arr[1] | 0xFE) == 0xFE) 
		$('#ILS-D').html('ON');
	else 
		$('#ILS-D').html('OFF');

	if((arr[1] | 0xFD) == 0xFD)
		$('#FD-D').html('ON');
	else
		$('#FD-D').html('OFF');

	if((arr[1] | 0xEF) == 0xEF)
		$('#switch1D').html('PULL');
	else if((arr[1] | 0xDF) == 0xDF)
		$('#switch1D').html('PUSH');
	else
		$('#switch1D').html('Neutre');

	if((arr[1] | 0xBF) == 0xBF)
		$('#switch1cranD').html('hPa');
	else if((arr[1] | 0x7F) == 0x7F)
		$('#switch1cranD').html('in Hg');
	else
		$('#switch1cranD').html('?');

	if((arr[2] | 0x7F) == 0x7F) 
		$('#CSTR-D').html('ON');
	else 
		$('#CSTR-D').html('OFF');

	if((arr[2] | 0xBF) == 0xBF) 
		$('#WPT-D').html('ON');
	else 
		$('#WPT-D').html('OFF');

	
	if((arr[2] | 0xDF) == 0xDF) 
		$('#VORD-D').html('ON');
	else
		$('#VORD-D').html('OFF');

	if((arr[2] | 0xEF) == 0xEF) 
		$('#NDB-D').html('ON');
	else 
		$('#NDB-D').html('OFF');

	if((arr[2] | 0xF7) == 0xF7) 
		$('#ARPT-D').html('ON');
	else 
		$('#ARPT-D').html('OFF');

	if((arr[2] | 0xFB) == 0xFB) 
		$('#switch2D').html('LS');
	else if((arr[2] | 0xFD) == 0xFD)
		$('#switch2D').html('VOR');
	else if((arr[2] | 0xFE) == 0xFE)
		$('#switch2D').html('NAV');
	else if((arr[3] | 0x7F) == 0x7F)
		$('#switch2D').html('ARC');
	else if((arr[3] | 0xBF) == 0xBF)
		$('#switch2D').html('PLAN');

	if((arr[3] | 0xDF) == 0xDF) 
		$('#switch3D').html('10');
	else if((arr[3] | 0xEF) == 0xEF)
		$('#switch3D').html('20');
	else if((arr[3] | 0xF7) == 0xF7)
		$('#switch3D').html('40');
	else if((arr[3] | 0xFB) == 0xFB)
		$('#switch3D').html('80');
	else if((arr[3] | 0xFD) == 0xFD)
		$('#switch3D').html('160');
	else if((arr[3] | 0xFE) == 0xFE)
		$('#switch3D').html('320');

	if((arr[4] | 0x7F) == 0x7F) 
		$('#switch4D').html('ADF');
	else if((arr[4] | 0xBF) == 0xBF)
		$('#switch4D').html('VOR');
	else 
		$('#switch4D').html('OFF');


	LEDcheckedRight();
	
}

function LEDcheckedRight(){
	ligthRight = $('#LED-ILS-D').is(':checked') ? ligthRight | 0x40 : ~(~ligthRight | 0x40);
	ligthRight = $('#LED-FD-D').is(':checked') ? ligthRight | 0x80 : ~(~ligthRight | 0x80);
	ligthRight = $('#LED-CSTR-D').is(':checked') ? ligthRight | 0x08 : ~(~ligthRight | 0x08);
	ligthRight = $('#LED-WPT-D').is(':checked') ? ligthRight | 0x02 : ~(~ligthRight | 0x02);
	ligthRight = $('#LED-VORD-D').is(':checked') ? ligthRight | 0x10 : ~(~ligthRight | 0x10);
	ligthRight = $('#LED-NDB-D').is(':checked') ? ligthRight | 0x20 : ~(~ligthRight | 0x20);
	ligthRight = $('#LED-ARPT-D').is(':checked') ? ligthRight | 0x04 : ~(~ligthRight | 0x04);
}