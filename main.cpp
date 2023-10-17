#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <stdio.h>
#include <lm.h>
#pragma comment(lib, "Netapi32.lib")

void wmain(int argc, TCHAR* argv[])
{
    NET_API_STATUS res;
    SHARE_INFO_2 p;
    DWORD parm_err = 0;

    if (argc < 3) {
        printf("Usage: \n");
        printf("\tNetShareAdd create <sharename> <directory>\n");
        printf("\tNetShareAdd del <sharename>\n");
    }
    else
    {
        if (!wcscmp(argv[1], L"del"))
        {
            if (argc != 3) {
                printf("Usage: \n");
                printf("\tNetShareAdd del <sharename>\n");
            }
            else {
                res = NetShareDel(NULL, argv[2], 0);
                if (res == 0)
                    printf("Share Removed.\n");
                else
                    printf("Error: %u\n", res);
            }
        }
        else if (!wcscmp(argv[1], L"create")) {
            if (argc != 4) {
                printf("Usage: \n");
                printf("\tNetShareAdd create <sharename> <directory>\n");
            }
            else {
                if (!CreateDirectory(argv[3], NULL))
                {
                    printf("Create Directory Failed!\n");
                    DWORD res = GetLastError();
                    if (res == ERROR_ALREADY_EXISTS)
                    {
                        printf("Directory already exist\n");
                    }
                }
                else
                    printf("Directory Create Succeed!\n");

                p.shi2_netname = argv[2];
                p.shi2_type = STYPE_DISKTREE; 
                p.shi2_remark = TEXT("Share Disk For WPS Service");
                p.shi2_permissions = 0;
                p.shi2_max_uses = 4;
                p.shi2_current_uses = 0;
                p.shi2_path = argv[3];
                p.shi2_passwd = NULL; // no password
  
                res = NetShareAdd(NULL, 2, (LPBYTE)&p, &parm_err);
    
                if (res == 0) {
                    printf("Share created Succeed!\n\n");
                    printf("Local Path is : %ls\n", p.shi2_path);
                    printf("ShareName is \\\\localhost\\\\%ls\n", p.shi2_netname);
                }

                else
                    printf("Error: %u\tparmerr=%u\n", res, parm_err);
            }
        }
    }
    return;
}

