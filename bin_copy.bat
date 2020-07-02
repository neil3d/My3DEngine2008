md \NexusBin
xcopy debug\*.exe \NexusBin\01_NexusEd\debug /I
xcopy debug\*.nex \NexusBin\01_NexusEd\debug /I
xcopy debug\*.dll \NexusBin\01_NexusEd\debug /I
xcopy debug\*.config \NexusBin\01_NexusEd\debug /I

xcopy release\*.exe \NexusBin\01_NexusEd\release /I
xcopy release\*.nex \NexusBin\01_NexusEd\release /I
xcopy release\*.dll \NexusBin\01_NexusEd\release /I
xcopy release\*.config \NexusBin\01_NexusEd\release /I

xcopy debug\zh-CHT\*.dll \NexusBin\01_NexusEd\debug\zh-CHT /I
xcopy debug\en-US\*.dll \NexusBin\01_NexusEd\debug\en-US /I
xcopy release\zh-CHT\*.dll \NexusBin\01_NexusEd\release\zh-CHT /I
xcopy release\en-US\*.dll \NexusBin\01_NexusEd\release\en-US /I

::xcopy nexus_game\*.* \NexusBin\01_NexusEd\nexus_game /s /I


