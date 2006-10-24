var fso = new ActiveXObject("Scripting.FileSystemObject");
var shell = new ActiveXObject("WScript.Shell");

var src = "C:\\Documents and Settings\\kosh\\My Documents\\Visual Studio Projects\\FusionEngineCVS\\unstable";

var fusion		=	shell.CurrentDirectory+"\\fusion";
var include		=	fusion+"\\include"
var lib			=	fusion+"\\lib";
var bin			=	shell.CurrentDirectory+"\\bin";
var system		=	bin+"\\system";
var external	=	fusion+"\\external";
var einc		=	fusion+"\\external\\include";
var elib		=	fusion+"\\external\\lib";

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

createFolder(external);
createFolder(einc);
fso.CopyFolder(src+"\\external\\include",einc);

/*
	Have to copy all the fusion libraries into
	fusion/lib
*/
createFolder(lib);
fso.CopyFile(src+"\\bin\\*.lib",lib,true);
createFolder(elib);
fso.CopyFolder(src+"\\external\\lib",elib);


/*
	Have to copy all the fusion dll's into
	bin/system
*/
deleteFile(system+"\\*.dll",true);
createFolder(system);
fso.CopyFile(src+"\\bin\\*.dll",system+"\\",true);
//	Also copy specified DLL files to the executable directory
//	Testsuite apps (should) have post build steps to copy what they need from there
fso.CopyFile(src+"\\external\\lib\\pthreadVC.dll",bin+"\\",true);

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

function deleteFile(file, force)
{
	try{
		fso.DeleteFile(file,true);
	}catch(e){
		WScript.echo("Caught Exception: "+e.description);
	}
}