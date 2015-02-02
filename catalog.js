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

    var SWMF = [
		    {id:"0.5",title:"","runid":"Brian_Curtis_042213_3"},
		    {id:"2.0",title:"","runid":"Brian_Curtis_042213_7"},
		    {id:"3.5",title:"","runid":"Brian_Curtis_102114_3"}
		    ];

    var LFM = [
		    {id:"0.5",title:"","runid":"Brian_Curtis_042213_4"},
		    {id:"2.0",title:"","runid":"Brian_Curtis_042213_8"}
		    ];


    var variables    = ['B_x','B_y','B_z','J_x','J_y','J_z','U_x','U_y','U_z','P','N','B'];
    //var variablesLFM = ['B_x','B_y','B_z','E_i','E_j','E_k','U_x','U_y','U_z','V_th','N','B'];

    var ret = [];
    var k = 0;

    for (var v = 0;v < variables.length;v++) {
	for (var i = 0;i < OpenGGCM.length;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "OpenGGCM"+"/"+variables[v]+"/"+OpenGGCM[i]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + OpenGGCM[i]["runid"];
	    ret[k]["fulldir"] = "../output/"+OpenGGCM[i].runid+"/figures/cuts/";
	    ret[k]["sprintf"] = "Step_%02d_Y_eq_0_"+variables[v]+".png";
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
	    ret[k]["fulldir"] = "../output/"+BATSRUS[i].runid+"/figures/cuts/";
	    ret[k]["sprintf"] = "Step_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "71";
	    k = k+1;
	}
    }

    for (var v = 0;v < variables.length;v++) {
	for (var i = 0;i < SWMF.length;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "SWMF"+"/"+variables[v]+"/"+SWMF[i]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + SWMF[i]["runid"];
	    ret[k]["fulldir"] = "../output/"+SWMF[i].runid+"/figures/cuts/";
	    ret[k]["sprintf"] = "Step_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "71";
	    k = k+1;
	}
    }

    for (var v = 0;v < variables.length;v++) {
	for (var i = 0;i < LFM.length;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "LFM"+"/"+variables[v]+"/"+LFM[i]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + LFM[i]["runid"];
	    ret[k]["fulldir"] = "../output/"+LFM[i].runid+"/figures/cuts/";
	    ret[k]["sprintf"] = "Step_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "71";
	    k = k+1;
	}
    }


    ///

    for (var z=0;z<2;z++) {
	if (z == 0) {type = "d"}
	if (z == 1) {type = "pd"}

    for (var v = 0;v < variables.length;v++) {
	for (var i = 1;i < 3;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "OpenGGCM"+"/"+type+variables[v]+"/"+OpenGGCM[i]["id"]+"-"+OpenGGCM[0]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + OpenGGCM[i]["runid"];
	    if (i == 1) {
		ret[k]["about"] = "90 min reversal - 30 min reversal";
	    }
	    if (i == 2) {
		ret[k]["about"] = "210 min reversal - 30 min reversal";
	    }
	    ret[k]["fulldir"] = "../output/PreconditionDifferences/"+OpenGGCM[i].runid + "_minus_" + OpenGGCM[0].runid+"/figures/cuts/";
	    ret[k]["sprintf"] = "Step_%02d_Y_eq_0_"+type+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "32";
	    k = k+1;
	}
    }

    for (var v = 0;v < variables.length;v++) {
	for (var i = 1;i < 3;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "BATSRUS"+"/"+type+variables[v]+"/"+BATSRUS[i]["id"]+"-"+BATSRUS[0]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + BATSRUS[i]["runid"];
	    if (i == 1) {
		ret[k]["about"] = "90 min reversal - 30 min reversal";
	    }
	    if (i == 2) {
		ret[k]["about"] = "210 min reversal - 30 min reversal";
	    }
	    ret[k]["fulldir"] = "../output/PreconditionDifferences/"+BATSRUS[i].runid + "_minus_" + BATSRUS[0].runid+"/figures/cuts/";
	    ret[k]["sprintf"] = "Step_%02d_Y_eq_0_"+type+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "32";
	    k = k+1;
	}
    }

    for (var v = 0;v < variables.length;v++) {
	for (var i = 1;i < 3;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "SWMF"+"/"+type+variables[v]+"/"+SWMF[i]["id"]+"-"+SWMF[0]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + SWMF[i]["runid"];
	    if (i == 1) {
		ret[k]["about"] = "90 min reversal - 30 min reversal";
	    }
	    if (i == 2) {
		ret[k]["about"] = "210 min reversal - 30 min reversal";
	    }
	    ret[k]["fulldir"] = "../output/PreconditionDifferences/"+SWMF[i].runid + "_minus_" + SWMF[0].runid+"/figures/cuts/";
	    ret[k]["sprintf"] = "Step_%02d_Y_eq_0_"+type+variables[v]+".png";
	    ret[k]["sprintfstart"] = "0";
	    ret[k]["sprintfstop"] = "32";
	    k = k+1;
	}
    }
    }
    return ret;

}