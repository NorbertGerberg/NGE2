@echo off
setlocal
set "zipFile=BUILD.zip"
set "zipFile2=BUILD2.zip"
set "zipFile3=BUILD3.zip"
set "destPath=NGE/../"
powershell -command "Expand-Archive -Path '%zipFile%' -DestinationPath '%destPath%' -Force"
powershell -command "Expand-Archive -Path '%zipFile2%' -DestinationPath '%destPath%' -Force"
powershell -command "Expand-Archive -Path '%zipFile3%' -DestinationPath '%destPath%' -Force"
powershell -command "Remove-Item '%zipFile%'"
powershell -command "Remove-Item '%zipFile2%'"
powershell -command "Remove-Item '%zipFile3%'"
endlocal
