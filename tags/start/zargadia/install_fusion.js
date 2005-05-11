var fso = new ActiveXObject("Scripting.FileSystemObject");
var shell = new ActiveXObject("WScript.Shell");

var projectdir = fso.GetParentFolderName(shell.CurrentDirectory);

var src = projectdir + "\\FusionEngineCVS\\unstable";

var fusion	=	shell.CurrentDirectory+"\\fusion";
var include	=	fusion+"\\include"
var lib		=	fusion+"\\lib";
var bin		=	shell.CurrentDirectory+"\\bin";
var system	=	bin+"\\system";

/*
	Firstly remove the fusion directory, so you can rebuild it
	this is to make sure you have a fresh copy and windows
	doesnt try anything clever
*/
if(fso.FolderExists(fusion+"\\") == true){
	fso.DeleteFolder(fusion,true);
}
fso.CreateFolder(fusion);

/*
	Have to copy all the fusion headers into 
	Zargadia/fusion/include
*/
fso.CreateFolder(include);
fso.CopyFolder(src+"\\include",include);

/*
	Have to copy all the fusion libraries into
	Zargadia/fusion/lib
*/
fso.CreateFolder(lib);
fso.CopyFile(src+"\\bin\\*.lib",lib,true);

/*
	Have to copy all the fusion dll's into
	Zargadia/bin/system
*/
fso.CopyFile(src+"\\bin\\*.dll",system+"\\",true);
//	Also copy the FMOD.dll file to the executable directory
fso.CopyFile(src+"\\external\\lib\\fmod.dll",bin+"\\",true);