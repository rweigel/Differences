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
	    ret[k]["id"] = "OpenGGCM"+"/"+variables[v].replace(/_([a-z].*)/,"<sub>$1</sub>")+"/"+OpenGGCM[i]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + OpenGGCM[i]["runid"];
	    ret[i]["fulldir"] = "../data/"+OpenGGCM[i].runid+"/Results/";
	    ret[i]["sprintf"] = "Result_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[i]["sprintfstart"] = "0";
	    ret[i]["sprintfstop"] = "71";
	    k = k+1;
	}
    }

    for (var v = 0;v < variables.length;v++) {
	for (var i = 0;i < BATSRUS.length;i++) {
	    ret[k] = {};
	    ret[k]["id"] = "BATSRUS"+"/"+variables[v].replace(/_([a-z].*)/,"<sub>$1</sub>")+"/"+BATSRUS[i]["id"];
	    ret[k]["aboutlink"] = "http://ccmc.gsfc.nasa.gov/results/viewrun.php?domain=GM&amp;runnumber=" + BATSRUS[i]["runid"];
	    ret[i]["fulldir"] = "../data/"+BATSRUS[i].runid+"/Results/";
	    ret[i]["sprintf"] = "Result_%02d_Y_eq_0_"+variables[v]+".png";
	    ret[i]["sprintfstart"] = "0";
	    ret[i]["sprintfstop"] = "71";
	    k = k+1;
	}
    }

    return ret;

}