var fso = new ActiveXObject("Scripting.FileSystemObject");
var shell = new ActiveXObject("WScript.Shell");

var src = "C:\\Documents and Settings\\kosh\\My Documents\\Visual Studio Projects\\FusionEngineCVS\\unstable";

var fusion	=	shell.CurrentDirectory+"\\fusion";
var include	=	fusion+"\\include"
var lib		=	fusion+"\\lib";
var bin		=	shell.CurrentDirectory+"\\bin";
var system	=	bin+"\\system";

createFolder(bin);

/*
	Firstly remove the fusion directory, so you can rebuild it
	this is to make sure you have a fresh copy and windows
	doesnt try anything clever
*/
createFolder(fusion,true);


/*
	Have to copy all the fusion headers into 
	fusion/include
*/
createFolder(include);
fso.CopyFolder(src+"\\include",include);

/*
	Have to copy all the fusion libraries into
	fusion/lib
*/
createFolder(lib);
fso.CopyFile(src+"\\bin\\*.lib",lib,true);

/*
	Have to copy all the fusion dll's into
	bin/system
*/
fso.DeleteFile(system+"\\*.dll",true);
createFolder(system);
fso.CopyFile(src+"\\bin\\*.dll",system+"\\",true);
//	Also copy the FMOD.dll file to the executable directory
fso.CopyFile(src+"\\external\\lib\\fmod.dll",bin+"\\",true);

//	This function stops the error when the directory 
//	already exists, a script error stops execution of the script
//	I want it to attempt to create the folder, if it exists, just
//	carry on
function createFolder(folder, deletefolder)
{
	try{
		if(deletefolder == true) fso.DeleteFolder(folder, true);
	}catch(e){}

	try{
		fso.CreateFolder(folder); 
	}catch(e){
		WScript.echo("Caught Exception: "+e.description);
		WScript.echo("Folder: "+folder);
	}
}