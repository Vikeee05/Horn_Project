Register-ArgumentCompleter -Native -CommandName make, make.exe -ScriptBlock {
    param($wordToComplete, $commandAst, $cursorPosition)

    @('clean', 'project', 'help') |
        Where-Object { $_.StartsWith($wordToComplete, [System.StringComparison]::OrdinalIgnoreCase) } |
        ForEach-Object {
            [System.Management.Automation.CompletionResult]::new($_, $_, 'ParameterValue', $_)
        }
}