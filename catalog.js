function catalogjsbase() {
    
    var model    = ["OpenGGCM","BATSRUS"];
    var OpenGGCM = [
		    {id:"0.5",title:"","runid":"Brian_Curtis_042213_1"},
		    {id:"2.0",title:"","runid":"Brian_Curtis_042213_5"},
		    {id:"3.5",title:"","runid":"Brian_Curtis_102114_1"}
		    ];

    var BATSRUS = [
		    {id:"0.5",title:"","runid":"Brian_Curtis_042213_2"},
		    {id:"2.0",title:"","runid":"Brian_Curtis_042213_6"},
		    {id:"3.5",title:"","runid":"Brian_Curtis_102114_2"}
		    ];

    var variables = ['B_x','B_y','B_z','J_x','J_y','J_z','U_x','U_y','U_z','P','N','B'];

    var ret = [];
    var k = 0;

    for (var v = 0;v < variables.length;v++) {
	for (var i = 0;i < OpenGGCM.length;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "OpenGGCM"+"/"+variables[v]+"/"+OpenGGCM[i]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + OpenGGCM[i]["runid"];
	    ret[k]["fulldir"] = "../data/"+OpenGGCM[i].runid+"/Results/";
	    ret[k]["sprintf"] = "Result_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "71";
	    k = k+1;
	}
    }

    for (var v = 0;v < variables.length;v++) {
	for (var i = 0;i < BATSRUS.length;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "BATSRUS"+"/"+variables[v]+"/"+BATSRUS[i]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + BATSRUS[i]["runid"];
	    ret[k]["fulldir"] = "../data/"+BATSRUS[i].runid+"/Results/";
	    ret[k]["sprintf"] = "Result_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "71";
	    k = k+1;
	}
    }

    for (var v = 0;v < variables.length;v++) {
	for (var i = 1;i < 3;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "OpenGGCM"+"/Precondition/"+i+"/"+variables[v]+"/"+OpenGGCM[0]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + OpenGGCM[i]["runid"];
	    if (i == 1) {
		ret[k]["about"] = "90 min reversal - 30 min reversal";
	    }
	    if (i == 2) {
		ret[k]["about"] = "210 min reversal - 30 min reversal";
	    }
	    ret[k]["fulldir"] = "../data/Precondition/"+OpenGGCM[i].runid + "_minus_" + OpenGGCM[0].runid+"/";
	    ret[k]["sprintf"] = "pcdiff_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "32";
	    k = k+1;
	}
    }

    for (var v = 0;v < variables.length;v++) {
	for (var i = 1;i < 3;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "BATSRUS"+"/Precondition/"+i+"/"+variables[v]+"/"+BATSRUS[0]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + BATSRUS[i]["runid"];
	    if (i == 1) {
		ret[k]["about"] = "90 min reversal - 30 min reversal";
	    }
	    if (i == 2) {
		ret[k]["about"] = "210 min reversal - 30 min reversal";
	    }
	    ret[k]["fulldir"] = "../data/Precondition/"+BATSRUS[i].runid + "_minus_" + BATSRUS[0].runid+"/";
	    ret[k]["sprintf"] = "pcdiff_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "32";
	    k = k+1;
	}
    }

    return ret;

}