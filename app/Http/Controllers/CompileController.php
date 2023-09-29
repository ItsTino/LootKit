<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Storage;
use Symfony\Component\Process\Process;
use Symfony\Component\Process\Exception\ProcessFailedException;

class CompileController extends Controller
{
    public function generateAgent(Request $request)
    {

        $name = $request->input('name');
        $options = $request->input('options', []);

        $uuid = \Illuminate\Support\Str::uuid();


        $templatePath = resource_path('agent/template.c');
        $template = file_get_contents($templatePath);

        // Modify the template with the user's input
        $code = str_replace('{{name}}', $name, $template);
        $code = str_replace('{{options}}', implode(' ', $options), $code);
        $code = str_replace('{{uuid}}', $uuid, $code);
        if (in_array('MELT_SELECT', $options)) {
            $code = "#define MELT_SELECT\n" . $code;
        }
        

        // Save the modified C code to a temporary file
        $tempPath = tempnam(sys_get_temp_dir(), 'agent_') . '.c';
        file_put_contents($tempPath, $code);

        // Compile the C code
        $outputPath = tempnam(sys_get_temp_dir(), 'agent_') . '.exe';

        // Use MinGW for compilation
        $process = new Process([
            'x86_64-w64-mingw32-gcc', // Use MinGW compiler
            '-Oz', // Optimize for size
            '-s',  // Strip symbol table and relocation information
            '-mwindows', // Prevent terminal popup by compiling as a Windows GUI application
            $tempPath,
            '-o',
            $outputPath,
            '-lwininet', // Link against the wininet library
        ]);

        // Set the PATH environment variable for the Process instance
        $process->setEnv(['PATH' => '/usr/lib/gcc/x86_64-w64-mingw32/12-win32:/usr/lib/gcc/x86_64-w64-mingw32/12-posix:/usr/x86_64-w64-mingw32:' . getenv('PATH')]);


        $process->run();

        // Check if the process was successful
        if (!$process->isSuccessful()) {
            throw new ProcessFailedException($process);
        }

        // Run UPX on the compiled binary
        $upxProcess = new Process([
            resource_path('upx'), // UPX command
            $outputPath, // Path to the compiled binary
        ]);

        $upxProcess->run();

        // Check if the UPX process was successful
        if (!$upxProcess->isSuccessful()) {
            throw new ProcessFailedException($upxProcess);
        }

        // Provide the compiled executable as a download
        return response()->download($outputPath, basename($outputPath))->deleteFileAfterSend(true);
    }
}
