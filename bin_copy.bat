md \nexus_bin
xcopy debug\*.exe \nexus_bin\debug /I
xcopy debug\*.nex \nexus_bin\debug /I
xcopy debug\*.dll \nexus_bin\debug /I

xcopy release\*.exe \nexus_bin\release /I
xcopy release\*.nex \nexus_bin\release /I
xcopy release\*.dll \nexus_bin\release /I

xcopy engine_data\*.* \nexus_bin\engine_data /s /I
xcopy game_data\*.* \nexus_bin\game_data /s /I

