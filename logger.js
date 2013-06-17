

function setup(){
}

//Need routine to write warning message to work window
//Will require status container in work area
function write_msg(){
}


function view_log_alt(eleobj){
	//First clear the view area.
	var workfrm = parent.work.document.forms["WFORM"];
	workfrm.elements["cmd"].value = "clearview";
	workfrm.submit();

	if( eleobj.options[eleobj.selectedIndex].value == "select" ){
		return;
	}
	//Not set view information
	var zform = document.forms["ZFORM"];
	zform.elements["cmd"].value = "viewlog";
	zform.elements["logname"].value = eleobj.options[eleobj.selectedIndex].value;
	zform.submit();
}

function view_log(logname){
	//First clear the view area.
	var workfrm = parent.work.document.forms["WFORM"];
	workfrm.elements["cmd"].value = "clearview";
	workfrm.submit();
	//Not set view information
	var zform = document.forms["ZFORM"];
	zform.elements["cmd"].value = "viewlog";
	zform.elements["logname"].value = logname;
	zform.submit();
}

function initialize(logname){
	var zform = document.forms["ZFORM"];
	zform.elements["cmd"].value = "initlog";
	zform.elements["logname"].value = logname;
	zform.submit();
}

function view_threads(logname){
	var workfrm = parent.work.document.forms["WFORM"];
	workfrm.elements["cmd"].value = "viewthreads";
	workfrm.elements["subcmd"].value = "";
	workfrm.elements["logname"].value = logname;
	workfrm.elements["startline"].value = 1;
	workfrm.submit();
}

function view_classes(logname){
	var workfrm = parent.work.document.forms["WFORM"];
	workfrm.elements["cmd"].value = "viewclasses";
	workfrm.elements["subcmd"].value = "";
	workfrm.elements["logname"].value = logname;
	workfrm.elements["startline"].value = 1;
	workfrm.submit();
}

function view_log_data(logname) {
	var workfrm = parent.work.document.forms["WFORM"];
	workfrm.elements["cmd"].value = "viewlogdata";
	workfrm.elements["subcmd"].value = "";
	workfrm.elements["logname"].value = logname;
	workfrm.elements["startline"].value = 1;
	workfrm.submit();
}

function view_exceptions(logname){
	var workfrm = parent.work.document.forms["WFORM"];
	workfrm.elements["cmd"].value = "viewexceptions";
	workfrm.elements["subcmd"].value = "";
	workfrm.elements["logname"].value = logname;
	workfrm.elements["startline"].value = 1;
	workfrm.submit();
}

function update_display(){
	var dspoptfrm = document.forms['DOFORM'];
	var dspopt = 0x10;

	if( dspoptfrm.elements["check1"].checked ) {
		dspopt |= 0x01;
	}
	if( dspoptfrm.elements["check2"].checked ) {
		dspopt |= 0x02;
	}
	if( dspoptfrm.elements["check4"].checked ) {
		dspopt |= 0x04;
	}
	if( dspoptfrm.elements["check8"].checked ) {
		dspopt |= 0x08;
	}
	if( dspoptfrm.elements["check10"].checked ) {
		dspopt |= 0x10;
	}
	if( dspoptfrm.elements["check20"].checked ) {
		dspopt |= 0x20;
	}
	if( dspoptfrm.elements["check40"].checked ) {
		dspopt |= 0x40;
	}
	document.forms['ZFORM'].elements["dspopts"].value=dspopt;
	var workfrm = parent.work.document.forms["WFORM"];
	workfrm.elements['dspopts'].value = dspopt;
	workfrm.elements['dsplines'].value = dspoptfrm.elements["dsplines"].options[dspoptfrm.elements["dsplines"].selectedIndex].value;
	workfrm.submit();
}

function scroll_start(){
	var wform = parent.work.document.forms["WFORM"];
	wform.elements["subcmd"].value = "resetstart";
	wform.elements["startline"].value = 1;
	wform.submit();
}

function scroll_back_line(cnt){
	var wform = parent.work.document.forms["WFORM"];
	wform.elements["backward"].value = cnt;
	wform.elements["increment"].value = "line";
	wform.submit();
}

function scroll_back(cnt){
	var wform = parent.work.document.forms["WFORM"];
	wform.elements["backward"].value = cnt;
	wform.elements["increment"].value = "page";
	wform.submit();
}

function scroll_forward(cnt){
	var wform = parent.work.document.forms["WFORM"];
	wform.elements["forward"].value = cnt;
	wform.elements["increment"].value = "page";
	wform.submit();
}

function scroll_forward_line(cnt){
	var wform = parent.work.document.forms["WFORM"];
	wform.elements["forward"].value = cnt;
	wform.elements["increment"].value = "line";
	wform.submit();
}

function scroll_end(){
	var wform = parent.work.document.forms["WFORM"];
	wform.elements["subcmd"].value = "resettoend";
	wform.elements["startline"].value = 1;
	wform.submit();
}

function view_file(){
	var sform = document.forms["SFORM"];

	var incfile = sform.elements["incfile"].options[sform.elements["incfile"].selectedIndex].value;
	if( incfile != "select") {
		window.open(incfile,"_blank");
	}
}

function perform_search(curlog){
	var sform = document.forms["SFORM"];
	var sterm = sform.elements["searchstr"].value;
	var wform = parent.work.document.forms["WFORM"];

	wform.elements["cmd"].value = "performsearch";
	wform.elements["subcmd"].value = sterm;
	wform.elements["logname"].value = curlog;
	wform.elements["startline"].value = 1;
	wform.submit();
	view_log(curlog);
}

function view_search(curlog){
	var sterm = document.forms["SFORM"].elements["sterm"];
	if( sterm.options[sterm.selectedIndex].value == "select") {
		return;
	}
	var wform = parent.work.document.forms["WFORM"];
	wform.elements["cmd"].value = "viewsearch";
	wform.elements["curfile"].value =  sterm.options[sterm.selectedIndex].value;
	wform.elements["startline"].value = 1;
	wform.elements["logname"].value = curlog;
	wform.submit();
}
