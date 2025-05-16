# Set working directory to the compiler folder
$workDir = "C:\Users\Satish Yadav\OneDrive\Desktop\Satish Coding\MINI_COMPILER"
Set-Location $workDir

# Clear previous TAC.txt and output.c if they exist
if (Test-Path "TAC.txt") {
    Clear-Content "TAC.txt"
    Write-Host "Cleared TAC.txt" -ForegroundColor Yellow
}
if (Test-Path "output.c") {
    Clear-Content "output.c"
    Write-Host "Cleared output.c" -ForegroundColor Yellow
}

# Compile parser and lexer
Write-Host "Running Bison..." -ForegroundColor Cyan
bison -d parser.y
if ($LASTEXITCODE -ne 0) { Write-Host "Bison failed!" -ForegroundColor Red; exit }

Write-Host "Running Flex..." -ForegroundColor Cyan
flex lexer.l
if ($LASTEXITCODE -ne 0) { Write-Host "Flex failed!" -ForegroundColor Red; exit }

# Compile with GCC
Write-Host "Compiling with GCC..." -ForegroundColor Cyan
gcc lex.yy.c parser.tab.c semantic.c TAC.c -o compiler.exe
if ($LASTEXITCODE -ne 0) { Write-Host "GCC Compilation failed!" -ForegroundColor Red; exit }

# Run the compiler with the test file
Write-Host "Executing the test file..." -ForegroundColor Green
Get-Content test.satish -Raw | ./compiler.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host "Test file execution failed!" -ForegroundColor Red
    exit
}

Write-Host "Converting TAC to C..." -Foreground Green
gcc tac_to_c.c -o testFile.exe
if ($LASTEXITCODE -ne 0) { Write-Host "Conversion failed!" -ForegroundColor Red; exit }
./testFile.exe
Write-Host "Conversion successfull" -Foreground Green

gcc output.c -o outputFile.exe
if ($LASTEXITCODE -ne 0) { Write-Host "Execution failed" -ForegroundColor Red; exit }
./outputFile.exe

Write-Host "`nExecution finished!" -ForegroundColor Yellow
