
function setup(){
	if( parent.controls.document.forms["CTRLFORM"] ) {
		var CTRLFORM = parent.controls.document.forms["CTRLFORM"];
		var wform = document.forms["WFORM"];
		CTRLFORM.elements["startline"].value = wform.elements["startline"].value;
		CTRLFORM.elements["linecount"].value = wform.elements["linecount"].value; 
	}
}

function center_on_line(linenum){
	//Need to reset search box when recentering after doing a search
	//
	var wform = document.forms["WFORM"];
	wform.elements["cmd"].value = "viewlogdata";
	wform.elements["startline"].value = linenum;
	wform.submit();
}

function view_threads(tname){
	var workfrm = document.forms["WFORM"];
	workfrm.elements["cmd"].value = "viewthreads";
	workfrm.elements["curfile"].value = tname;
	workfrm.elements["startline"].value = 1;
	workfrm.submit();
}

function view_class(cname){
	var workfrm = document.forms["WFORM"];
	workfrm.elements["cmd"].value = "viewclass";
	workfrm.elements["curfile"].value = cname;
	workfrm.elements["startline"].value = 1;
	workfrm.submit();
}
