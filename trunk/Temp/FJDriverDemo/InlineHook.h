#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    #define MAX_REPLACE_CODE_SIZE       0x20

    typedef struct _INLINE_HOOK_INFO
    {
        PBYTE   pTarget;            //Ҫ���صĺ�������Windows API��
        PBYTE   pDetour;            //�������Target�����ĺ���
        PBYTE   pOriginal;

        //UCHAR   targetBackup[MAX_REPLACE_CODE_SIZE];
        UCHAR   trampoline[MAX_REPLACE_CODE_SIZE];  //Target�����ĸ���Ʒ������� jmp
        ULONG   trampolineSize;                     //���� trampoline �ĳ��ȣ���������� jmp
        ULONG   targetBackupSize;                   //Target�����ı��ݳ���

    }INLINE_HOOK_INFO, *PINLINE_HOOK_INFO;

    //32 || 64
    C_ASSERT(sizeof(INLINE_HOOK_INFO) == 52 || sizeof(INLINE_HOOK_INFO) == 64);

    //struct _INLINE_HOOK_INFO;
    //typedef _INLINE_HOOK_INFO* PINLINE_HOOK_INFO;

    BOOL CreateInlineHook(PVOID* ppTarget, PVOID pDetour, PVOID* ppOriginal, PINLINE_HOOK_INFO* ppOutHookInfo);
    BOOL RestoreInlineHook(PINLINE_HOOK_INFO pHookInfo);
    //NTSTATUS EnableInlineHook(PVOID pTarget, PINLINE_HOOK_INFO pHookInfo);
    //NTSTATUS DisableInlineHook(PVOID pTarget, PINLINE_HOOK_INFO pHookInfo);


#ifdef __cplusplus
}
#endif