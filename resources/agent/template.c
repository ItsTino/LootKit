#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wininet.h>

int main(int argc, char *argv[]) {
    char* uuid = "{{uuid}}"; // Embed the UUID in the agent
    char* name = "{{name}}";
    printf("Name: %s\n", name);
    printf("Selected Options: {{options}}\n");
    printf("UUID: %s\n", uuid);
    char url[] = "https://reqinspect.alpine.cx/capture/9bc05d15-bc2a-41dc-84d2-9db7cf4053f1?uuid="; // https://reqinspect.alpine.cx/session/9bc05d15-bc2a-41dc-84d2-9db7cf4053f1

    //Build The URL
    char* full_url = malloc(strlen(url)+strlen(uuid)+1);
    strcpy(full_url, url);
    strcat(full_url, uuid);

    //Send blank GET request to full_url
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hSession = InternetOpenUrlA(hInternet, full_url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    InternetCloseHandle(hSession);
    InternetCloseHandle(hInternet);

    //Make GET request to https://ip.nf/me.json
    char* ip_url = "https://ip.nf/me.json";
    HINTERNET hInternet2 = InternetOpenA("Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hSession2 = InternetOpenUrlA(hInternet2, ip_url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    char buffer[1024];
    DWORD bytesRead;
    while(InternetReadFile(hSession2, buffer, sizeof(buffer), &bytesRead) && bytesRead) {
        printf("%s", buffer);
    }
    InternetCloseHandle(hSession2);
    InternetCloseHandle(hInternet2);

    //Output the buffer to console
    printf("\n%s\n", buffer);



    printf("\nPress any key to exit...\n");
    getchar();

    return 0;
}
