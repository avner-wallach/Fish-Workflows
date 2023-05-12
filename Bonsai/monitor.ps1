
do{
	if((get-process "Bonsai64" -ea SilentlyContinue) -eq $Null){ 
	        echo "Not Running"
 		$Date = Get-Date 
		 Add-Content -Path .\log.txt -Value $Date
		C:\Users\avner\Documents\bonsai\Bonsai64.exe live_fish_tracker_onevid.bonsai --start
	}
} while (1 -eq 1)
