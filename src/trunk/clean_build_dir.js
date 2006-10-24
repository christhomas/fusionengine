var fso = new ActiveXObject("Scripting.FileSystemObject");

/*	
	You cannot run the script unless you have
	1) build directory
	2) extension of target file
	3) the target name itself

	reason:	cause if you dont have these three
			things, you can't make a serious
			judgement about whether to delete/manage
			the files inside the build directory
*/		

args = WScript.Arguments;
if(args.length == 3){
	builddir	= args(0);
	ext			= args(1);
	target		= args(2);

	lib = builddir+"/"+target+".exp";
	exp = builddir+"/"+target+".lib";

	/*	You wanna clean up all the crap that VS.NET 2003 outputs that you dont want other people to
		"accidentally" use, so, we use this, to delete it, so you can make sure the build directory
		is clean of potentially harmful crap
	*/
	if(ext == ".dll"){
		WScript.echo("Delete: "+lib);
		fso.DeleteFile(lib,true);
	
		WScript.echo("Delete: "+exp);
		fso.DeleteFile(exp,true);
	}else{
		WScript.echo("THIS PROJECT IS LIB, IGNORING");
	}
}