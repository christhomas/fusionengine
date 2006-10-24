var fso = new ActiveXObject("Scripting.FileSystemObject");
var shell = new ActiveXObject("WScript.Shell");
	
//	File to cache what happens as you navigate the file tree
var cachefile = fso.CreateTextFile("cache.txt");

//	The location of all the backup files
var abspath		=	"..";
var backup		=	abspath+"/backup";

//	The current location of the directory navigation
var curdir;

//	Default function to call
var func = donothing;

//	A Flag to say whether you want the tree on the console as well as the cache file
var printtree = false;

//	A Variable to hold the number of lines/[src/header files] currently counted
var cnt_lin	= 0;
var cnt_src	= 0;
var cnt_hdr	= 0;
var cnt_cls	= 0;

//	Lets setup some custom string functions that are helpful at a later stage
//	you do this, by adding functionality to the string class prototype
//	then all your strings can call these functions, I love this prototype stuff
//	amazing!
String.prototype.Name = function(){
	return this.substr(this.lastIndexOf("\\")+1);
}
String.prototype.Path = function(){
	return this.substr(0,this.lastIndexOf("\\"));
}
String.prototype.Find = function(str){
	if(this.Name().indexOf(str) != -1) return true;
	return false;
}

args = WScript.Arguments;
for(var i=0;i<args.length;i++){
	if(args(i) == "--process"){
		func = processStrings;
	}
	
	if(args(i) == "--restore"){
		func = restoreFile;
	}
	
	if(args(i) == "--restore-remove"){
		func = restoreRemoveFile;
	}
	
	if(args(i) == "--restore-backup"){
		func = restoreBackup;
	}
	
	if(args(i) == "--stats"){
		func = stats;
	}
	
	if(args(i) == "--print-tree"){
		printtree = true;
	}
}

WScript.echo("Processing, please wait");
WScript.echo("Current dir = "+shell.CurrentDirectory);
WScript.echo("parent = "+abspath);
WScript.echo("backup = "+backup);

//	Recursively list directories
listDir("./");

//	Function to recursively navigate a directory structure
function listDir(dir)
{
	curdir = dir;
	curdir = relativeFilename(curdir);
	
	cachefile.WriteLine("Dir = "+curdir);
	if(printtree == true) WScript.echo(curdir);

	var f = fso.GetFolder(curdir);
	
	listFiles();
	
	for(var fc = new Enumerator(f.SubFolders);!fc.atEnd();fc.moveNext()){
		//	List the subdirectory
		var str = fc.item().Path;
		listDir(str);
	}
}

//	iterates through a directory's files
function listFiles()
{
	var f = fso.GetFolder(curdir);

	for(var fc = new Enumerator(f.Files);!fc.atEnd();fc.moveNext()){
		//	Output the file to the cache and call the function to process it
		cachefile.WriteLine("\tFile = "+fc.item().Name);
		if(printtree == true) WScript.echo("\t"+fc.item().Name);
		
		//	Is this a good idea? making all file Paths relative to the FusionEngine directory?
		fn = relativeFilename(fc.item().Path);
		func(fn);		
	}
}

function donothing(file){}

function processStrings(file, sstring, rstring, sterm)
{
	//	This function only processes vcproj (ignore those with a .backup extension) files
	if(file.Find("vcproj") == false || file.Find("backup") == true || file.Find("sed") == true) return;
	
	if(sstring == undefined){
		var s,r; // search/replace terms
		
		//	Remove all duplicate "" from the project files
		s = "\\\"\\\"";
		r = "\\\"";
		processStrings(file,s,r);
		
		addinc="AdditionalIncludeDirectories";
		
		s = "=.*";
		r = "="+add_quotes("$(SolutionDir)include;$(SolutionDir)include\\\\arch",true);
		processStrings(file,s,r,addinc);
		
		//	Add an extra path to vfs project include directories
		if(file.Path().indexOf("vfs") != -1){
			s = "$(SolutionDir)include;";
			r = "$(SolutionDir)vfs;"+s;
			processStrings(file,s,r);
		}
		
		//	Add an extra path to all projects, which include their own project directory	
		s = "\\\"$(SolutionDir)";
		r = "\\\"$(ProjectDir);$(SolutionDir)";
		processStrings(file,s,r,addinc);
		
		//	Time to remove duplicate instances of proj and vfs include directories
		dup_proj = "$(ProjectDir)";
		processStrings(file,add_quotes(dup_proj+";"+dup_proj),add_quotes(dup_proj)+";");		
		
		dup_vfs = "$(SolutionDir)vfs";
		processStrings(file,add_quotes(dup_vfs+";"+dup_vfs),add_quotes(dup_vfs)+";");
		
		//	Now find any instances of £(SolutionDir) or £(ProjectDir)
		//	I think I've made some typos, lets clean them up
		processStrings(file,add_quotes("£(SolutionDir)"),add_quotes("$(SolutionDir)"));
		processStrings(file,add_quotes("£(ProjectDir)"),add_quotes("$(ProjectDir)"));
		processStrings(file,add_quotes(";;"),add_quotes(";"));
		
		//	strip any &quot; from the file
		processStrings(file,"&quot;","");
		processStrings(file,"$(NOINHERIT)","");
	}else{
		WScript.echo("\n+++++++++++++++++++++++++++++++");
		WScript.echo("processStrings("+file.Name()+")");
		WScript.echo("\tsearch: "+sstring);
		WScript.echo("\trplace: "+rstring);

		//	Attempt to restore from .backup, or perm backup any file which has 0 bytes (empty)		
		if(file.size == 0){
			//	Attempt to restore from .backup file
			restoreFile(file);

			if(file.size == 0){
				//	That failed, attempt to restore from permenant backup
				restoreBackup(file);	
			}
		}else{
			//	Only create a backup if there WASNT A problem in opening the original
			//	Dont want to overwrite it willy nilly with all kinds of crap you might find
			createBackup(file);
		}		
		
		var replace_exp = "s/"+sstring+"/"+rstring+"/g";
		
		if(sterm != undefined) replace_exp = "/"+sterm+"/"+replace_exp;
		
		cmd = sed_command(replace_exp,file);
		WScript.echo(cmd);
		exec = shell.Exec(cmd);
		sed_writefile(file,exec);
	}
}

function restoreFile(file)
{
	//	If the file is a backup file, or the backup doesnt exist
	//	return.  You can't backup a backup file and you can't
	//	restore from one, if it doesnt exist
	if(file.Find("backup") == true || fso.FileExists(file+".backup") == false) return;
	
	WScript.echo("restoreFile("+file.Name()+")");
	
	fso.CopyFile(file+".backup",file,true);
}

function restoreRemoveFile(file)
{
	restoreFile(file);
	
	if(file.Name().indexOf(".backup") != -1){
		fso.DeleteFile(file);
	}
}

function createBackup(file)
{
	fso.CopyFile(file,file+".backup",true);
}

function restoreBackup(file)
{
	if(file.Find("vcproj") == false || file.Find("backup") == true) return;
	
	WScript.echo("restoreBackup("+file.Name()+")");	
	
	if(fso.FileExists(file) == true){
		
		var str = backup+"\\"+file;
	
		if(fso.FileExists(str) == true) fso.CopyFile(str,file,true);
	}
}

function createArchive(file)
{
	WScript.echo("createBackup("+file.Name()+")");
	
	if(fso.FileExists(file) == true){
		
		var str = backup+"\\"+file;
		
		fso.CopyFile(file,str,true);
	}
}

function stats(file)
{
	if((file.Find(".cpp") == false && file.Name().length != (file.Name().indexOf(".h")+2)) || file.Find("backup") == true) return;
		
	//	Count src and header files
	if(file.Find(".cpp") == true)	cnt_src++;
	if(file.Find(".h") == true)		cnt_hdr++;
	
	cmd = "sed -n \"$=;/^$/d\" "+add_quotes(file);
	exec = shell.exec(cmd);

	cnt_lin += Number(exec.StdOut.ReadLine());
	
	//	You need to prepend the sed command with %comspec% /c because you are using a pipe in the command
	//	and it doesnt work without it, strange, but true, works exactly the same, but for some reason
	//	shell.exec doesnt like pipes ( | )
	cmd = "%comspec% /c sed -n \"/^class.*{/p;/^$/d\" " + add_quotes(file) + " | " + "sed -n \"$=\"";
	exec = shell.Exec(cmd);
	
	cnt_cls += Number(exec.StdOut.ReadLine());	
	
	WScript.StdOut.write("Lines: "+cnt_lin+", src = "+cnt_src+", hdr = "+cnt_hdr+", classes = "+cnt_cls+"\r");	
}

function add_quotes(str, escape)
{
	if(escape == undefined)	escape = "";
	else					escape = "\\";

	return escape+"\""+str+escape+"\"";
}

//	This function chops off the part you dont really need (the part before the fusion engine directory)
function relativeFilename(file)
{
	//WScript.echo("relativeFilename("+file+")");
	if(file == "./") return file;
	
	return file.substr(shell.CurrentDirectory.length+1);
}

function sed_command(exp,file,cmd)
{
	if(cmd == undefined) cmd = "";
	
	return "sed -e " + add_quotes(exp) + " " + add_quotes(file) + " > " + sedfile(file);
}

function sed_writefile(file,exec)
{
	//	Write a new output file
	var output = fso.CreateTextFile(sedfile(file),true);
	output.Write(exec.StdOut.ReadAll());	
	output.Close();
	
	//	Copy over the existing input file
	fso.CopyFile(sedfile(file),file,true);
	fso.DeleteFile(file+".sed");
}

function sedfile(file)
{
	return file+".sed";
}