/////////////////////////////////////////////////////////////////////////////
// For info about the file structrure see
// https://docs.microsoft.com/en-us/windows/win32/menurc/versioninfo-resource
// and
// https://docs.microsoft.com/en-us/windows/win32/menurc/stringfileinfo-block

// Icon
//
// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDI_ICON1               ICON    DISCARDABLE     "icon.ico"

// File info for the LabRPSCmd.exe
//
1 VERSIONINFO 
FILEVERSION ${PACKAGE_VERSION_MAJOR},${PACKAGE_VERSION_MINOR},${LABRPS_VERSION_PATCH},${PACKAGE_VERSION_PATCH}
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "040904b0" // 409 stands for US English
        BEGIN
            VALUE "CompanyName", "${PROJECT_NAME} Team"
            VALUE "FileDescription", "${PROJECT_NAME} command line executable"
            VALUE "InternalName", "LabRPSCmd.exe"
            VALUE "LegalCopyright", "Copyright (C) 2024"
            VALUE "OriginalFilename", "LabRPSCmd.exe"
            VALUE "ProductName", "${PROJECT_NAME}"
            VALUE "ProductVersion", "${LABRPS_VERSION}.${LABRPS_VERSION_PATCH}${PACKAGE_VERSION_SUFFIX}"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x409, 1200 //US English, Unicode
    END
END
