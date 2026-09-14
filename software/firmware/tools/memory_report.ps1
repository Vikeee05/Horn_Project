param(
    [Parameter(Mandatory = $true)]
    [string]$Elf
)

$row = (arm-none-eabi-size $Elf | Select-Object -Last 1) -split '\s+' | Where-Object { $_ }
$text = [int64]$row[0]
$data = [int64]$row[1]
$bss = [int64]$row[2]
$flashUsed = $text + $data
$ramUsed = $data + $bss

Write-Output ('FLASH: total 1048576 bytes, used {0} bytes, available {1} bytes ({2:N2}% used)' -f $flashUsed, (1048576 - $flashUsed), (100 * $flashUsed / 1048576))
Write-Output ('RAM:   total 131072 bytes, used {0} bytes, available {1} bytes ({2:N2}% used)' -f $ramUsed, (131072 - $ramUsed), (100 * $ramUsed / 131072))