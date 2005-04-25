var fso = new ActiveXObject("Scripting.FileSystemObject");
var shell = new ActiveXObject("WScript.Shell");

//	The current location of the directory navigation
var curdir = "./";

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

WScript.echo("Processing, please wait");

//	Recursively list directories
listDir(curdir);

//	Function to recursively navigate a directory structure
function listDir(dir)
{
	curdir = dir;
	curdir = relativeFilename(curdir);
	
	var f = fso.GetFolder(curdir);
	
	listFiles();
	
	for(var fc = new Enumerator(f.SubFolders);!fc.atEnd();fc.moveNext()){
		//	List the subdirectory
		listDir(fc.item().Path);
	}
}

//	iterates through a directory's files
function listFiles()
{
	var f = fso.GetFolder(curdir);

	for(var fc = new Enumerator(f.Files);!fc.atEnd();fc.moveNext()){
		//	Is this a good idea? making all file Paths relative to the FusionEngine directory?
		stats( relativeFilename(fc.item().Path) );		
	}
}

function stats(file)
{
	if(fileExtension(file) == ".cpp" || fileExtension(file) == ".h"){
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
	if(file == "./") return file;
	
	return file.substr(shell.CurrentDirectory.length+1);
}

function fileExtension(file)
{
	return file.Name().substr(file.Name().indexOf("."));
}
