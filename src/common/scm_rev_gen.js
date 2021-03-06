var wshShell        = new ActiveXObject("WScript.Shell")
var oFS             = new ActiveXObject("Scripting.FileSystemObject");

var outfile         = "scm_rev.cpp";
var cmd_revision    = " rev-parse HEAD";
var cmd_describe    = " describe --always --long --dirty";
var cmd_branch      = " rev-parse --abbrev-ref HEAD";

var git_search_paths = {
    "git.cmd": 1,
    "git": 1,
    "C:\\Program Files (x86)\\Git\\bin\\git.exe": 1,
    "C:\\Program Files\\Git\\bin\\git.exe": 1
};

function GetGitExe() {
    for (var gitexe in git_search_paths) {
        try {
            wshShell.Exec(gitexe);
            return gitexe;
        } catch (e) {
        }
    }

    WScript.Echo("Cannot find git or git.cmd, check your PATH:\n" +
        wshShell.ExpandEnvironmentStrings("%PATH%"));
    WScript.Quit(1);
}

function GetFirstStdOutLine(cmd) {
    try {
        return wshShell.Exec(cmd).StdOut.ReadLine();
    } catch (e) {
        // catch "the system cannot find the file specified" error
        WScript.Echo("Failed to exec " + cmd + " this should never happen");
        WScript.Quit(1);
    }
}

function GetFileContents(f) {
    try {
        return oFS.OpenTextFile(f).ReadAll();
    } catch (e) {
        // file doesn't exist
        return "";
    }
}

// get info from git
var gitexe      = GetGitExe();
var revision    = GetFirstStdOutLine(gitexe + cmd_revision);
var describe    = GetFirstStdOutLine(gitexe + cmd_describe);
var branch      = GetFirstStdOutLine(gitexe + cmd_branch);
var isMaster    = +("master" == branch);

// remove hash (and trailing "-0" if needed) from description
describe = describe.replace(/(-0)?-[^-]+(-dirty)?$/, '$2');

var out_contents =
    "#include \"common/scm_rev.h\"\n" +
    "namespace Common {\n" +
    "    const char g_scm_rev[]      = \"" + revision + "\";\n" +
    "    const char g_scm_branch[]   = \"" + branch + "\";\n" +
    "    const char g_scm_desc[]     = \"" + describe + "\";\n" +
    "}\n";

// check if file needs updating
if (out_contents == GetFileContents(outfile)) {
    WScript.Echo(outfile + " current at " + describe);
} else {
    // needs updating - writeout current info
    oFS.CreateTextFile(outfile, true).Write(out_contents);
    WScript.Echo(outfile + " updated to " + describe);
}
