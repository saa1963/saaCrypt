//-----------------------------------------------------------------------------
// В данном примере производится зашифрование и расшифрование строки данных в
// ядре ОС Windows. Для шифрования используется случайный симметричный ключ.
//-----------------------------------------------------------------------------
#include <ntddk.h>
#include <windef.h>
#include <cpdrvlib.h>
#include <wincspc.h>
#include <lfmm.h>

#define MODNAME "cpdrvlibsample"
#define MAX_CONTEXTS 10
#define POOLS 16
#define DRVSAMPLE_POOL 'DRVS'

//-----------------------------------------------------------------------------
// Расширение драйвера, хранимое в резидентной памяти
typedef struct _DRVSAMPLE_EXTENSION {
    PFILE_OBJECT CPFile;
    IO_STATUS_BLOCK IoStatusBlock;
    CPCCreateProvider_t *CreateProvider;
    CPCInitMemoryLF_t *InitMemoryLF;
    CPCGetDefaultConfig_t *GetDefaultConfig;
} DRVSAMPLE_EXTENSION, *PDRVSAMPLE_EXTENSION;



//-----------------------------------------------------------------------------
// Прототипы функций
VOID NTAPI DrvSampleUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS GetFuncAddress(IN PDEVICE_OBJECT DeviceObject, IN ULONG IoControlCode,
			IN OUT LPVOID Func, IN IO_STATUS_BLOCK *pIoStatusBlock);
NTSTATUS TestCrypt(IN CPCCreateProvider_t *CreateProvider,
		   IN CPCInitMemoryLF_t *InitMemoryLF,
		   IN CPCGetDefaultConfig_t *GetDefaultConfig);



//-----------------------------------------------------------------------------
NTSTATUS NTAPI DriverEntry(IN PDRIVER_OBJECT DriverObject,
			   IN PUNICODE_STRING RegistryPath)
{
    NTSTATUS NtStatus;
    UNICODE_STRING deviceNameUnicodeString;
    PDEVICE_OBJECT CPDevice;
    PDRVSAMPLE_EXTENSION DriverExt;

    KdPrint((MODNAME ": entering DriverEntry = 0x%.8X\n", DriverEntry));

    DriverObject->DriverUnload = DrvSampleUnload;

    NtStatus = IoAllocateDriverObjectExtension(DriverObject, 0,
	sizeof(*DriverExt), &DriverExt);
    if (!NT_SUCCESS(NtStatus)) {
	KdPrint((MODNAME "! IoAllocateDriverObjectExtension failed with a return of 0x%.8X!\n", NtStatus));
	return NtStatus;
    }

    RtlInitUnicodeString(&deviceNameUnicodeString, CPDRVLIB_NT_DEVICE_NAME);

    NtStatus = IoGetDeviceObjectPointer(&deviceNameUnicodeString,
	FILE_READ_DATA,	&DriverExt->CPFile, &CPDevice);
    if (!NT_SUCCESS(NtStatus)) {
	KdPrint((MODNAME "! IoGetDeviceObjectPointer failed with a return of 0x%.8X!\n", NtStatus));
	return NtStatus;
    }
    KdPrint((MODNAME ": IoGetDeviceObjectPointer finished successfully\n"));
    KdPrint((MODNAME ": CPFile = 0x%.8X\n", DriverExt->CPFile));
    KdPrint((MODNAME ": CPDevice = 0x%.8X\n", CPDevice));

    NtStatus = GetFuncAddress(CPDevice, IOCTL_GETCPCCREATEPROVIDER,
	&DriverExt->CreateProvider, &DriverExt->IoStatusBlock);
    if (!NT_SUCCESS(NtStatus)) {
	DrvSampleUnload(DriverObject);
	return NtStatus;
    }
    KdPrint((MODNAME ": Address of CPCCreateProvider acquired: 0x%.8X\n",
	DriverExt->CreateProvider));

    NtStatus = GetFuncAddress(CPDevice, IOCTL_GETCPCINITMEMORYLF,
	&DriverExt->InitMemoryLF, &DriverExt->IoStatusBlock);
    if (!NT_SUCCESS(NtStatus)) {
	DrvSampleUnload(DriverObject);
	return NtStatus;
    }
    KdPrint((MODNAME ": Address of CPCInitMemoryLF acquired: 0x%.8X\n",
	DriverExt->InitMemoryLF));

    NtStatus = GetFuncAddress(CPDevice, IOCTL_GETCPCGETDEFAULTCONFIG,
	&DriverExt->GetDefaultConfig, &DriverExt->IoStatusBlock);
    if (!NT_SUCCESS(NtStatus)) {
	DrvSampleUnload(DriverObject);
	return NtStatus;
    }
    KdPrint((MODNAME ": Address of CPCGetDefaultConfig acquired: 0x%.8X\n",
	DriverExt->GetDefaultConfig));

    NtStatus = TestCrypt(DriverExt->CreateProvider, DriverExt->InitMemoryLF,
	DriverExt->GetDefaultConfig);
    if (!NT_SUCCESS(NtStatus))
	DrvSampleUnload(DriverObject);

    return NtStatus;
}



//-----------------------------------------------------------------------------
VOID NTAPI DrvSampleUnload(IN PDRIVER_OBJECT DriverObject)
{
    PDRVSAMPLE_EXTENSION DriverExt;

    KdPrint((MODNAME ": entering DriverUnload\n"));

    DriverExt = IoGetDriverObjectExtension(DriverObject, 0);
    if (DriverExt != NULL && DriverExt->CPFile != NULL)
	ObDereferenceObject(DriverExt->CPFile);

    KdPrint((MODNAME ": DriverUnload finished successfully\n"));
}



//-----------------------------------------------------------------------------
NTSTATUS GetFuncAddress(IN PDEVICE_OBJECT DeviceObject, IN ULONG IoControlCode,
			IN OUT LPVOID Func, IN IO_STATUS_BLOCK *pIoStatusBlock)
{
    KEVENT Event;
    PIRP pIrp;
    NTSTATUS NtStatus;

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    pIrp = IoBuildDeviceIoControlRequest(IoControlCode,
	DeviceObject, NULL, 0, Func, sizeof(LPVOID), TRUE,
	&Event, pIoStatusBlock);
    if (pIrp == NULL) {
	KdPrint((MODNAME "! IoBuildDeviceIoControlRequest failed!\n"));
	return STATUS_UNSUCCESSFUL;
    }
    NtStatus = IoCallDriver(DeviceObject, pIrp);
    if (!NT_SUCCESS(NtStatus)) {
	KdPrint((MODNAME "! IoCallDriver failed with a return of 0x%.8X!\n", NtStatus));
	return NtStatus;
    }
    if (NtStatus == STATUS_PENDING) {
	KdPrint((MODNAME ": IoCallDriver returned STATUS_PENDING\n"));
	NtStatus = KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE,
	    NULL);
	if (NtStatus != STATUS_SUCCESS) {
	    KdPrint((MODNAME "! KeWaitForSingleObject failed with a return of 0x%.8X!\n", NtStatus));
	    return NtStatus;
	}
    }

    return STATUS_SUCCESS;
}



//-----------------------------------------------------------------------------
NTSTATUS TestCrypt(IN CPCCreateProvider_t *CreateProvider,
		   IN CPCInitMemoryLF_t *InitMemoryLF,
		   IN CPCGetDefaultConfig_t *GetDefaultConfig)
{
    NTSTATUS NtStatus = STATUS_SUCCESS;
    CPC_LFMM_CONFIG LfmmConfig;
    LONG PoolSizes[POOLS];
    LONG i;
    LONG ArenaSize = 65536;
    LPCPC_MEMORY_ARENA MemoryArena;
    ULONG res;
    CPC_CONFIG CSPConfig;
    HCRYPTMODULE hCSP = NULL;
    HCRYPTPROV hProv = 0;
    VTABLEPROVSTRUC VTable = {3, NULL, NULL, PROV_GOST_2001_DH};
    HCRYPTKEY hKey = 0;
    BYTE *pbIV = NULL;
    BYTE pbData[] = "Hello, world!";
    BYTE pbEncrypted[256];
    DWORD dwDataLen;

    KdPrint((MODNAME ": entering TestCrypt\n"));

    RtlFillMemory(&LfmmConfig, sizeof(LfmmConfig), 0);
    LfmmConfig.fSMP = TRUE;

    RtlFillMemory(PoolSizes, sizeof(PoolSizes), 0);
    PoolSizes[MP_BIG] = PoolSizes[MP_WORK] = 10000 * (250 + MAX_CONTEXTS);
    PoolSizes[MP_PRIME_M] = PoolSizes[MP_SEC_M] = 1024 * (400 + MAX_CONTEXTS);
    PoolSizes[MP_WORK_M] = 2 * PoolSizes [MP_PRIME_M];

    LfmmConfig.PoolSizes = PoolSizes;
    LfmmConfig.nPools = POOLS;
    LfmmConfig.nCPUs = 2;

    for (i = 0; i < POOLS; ++i)
	ArenaSize += PoolSizes[i];

    LfmmConfig.Size = ArenaSize;
    LfmmConfig.Buffer = ExAllocatePoolWithTag(NonPagedPool, ArenaSize,
	DRVSAMPLE_POOL);
    if (LfmmConfig.Buffer == NULL) {
	KdPrint((MODNAME "! Couldn't allocate memory\n"));
	return STATUS_UNSUCCESSFUL;
    }

    res = InitMemoryLF(&MemoryArena, &LfmmConfig);
    if (res) {
	KdPrint((MODNAME "! InitMemoryLF failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    CSPConfig.cbSize = sizeof(CSPConfig);

    res = GetDefaultConfig(&CSPConfig, NULL);
    if (res) {
	KdPrint((MODNAME "! GetDefaultConfig failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    CSPConfig.pArena = MemoryArena;

    res = CreateProvider(&hCSP, &CSPConfig);
    if (res) {
	KdPrint((MODNAME "! CreateProvider failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    res = hCSP->AcquireContext(hCSP, &hProv, NULL,
	CRYPT_VERIFYCONTEXT | CRYPT_SILENT, &VTable);
    if (res) {
	KdPrint((MODNAME "! AcquireContext failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    res = hCSP->GenKey(hCSP, hProv, CALG_G28147, 0, &hKey);
    if (res) {
	KdPrint((MODNAME "! GenKey failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    res = hCSP->GetKeyParam(hCSP, hProv, hKey, KP_IV, NULL, &dwDataLen, 0);
    if (res) {
	KdPrint((MODNAME "! GetKeyParam failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    pbIV = ExAllocatePoolWithTag(PagedPool, dwDataLen, DRVSAMPLE_POOL);
    if (pbIV == NULL) {
	KdPrint((MODNAME "! Couldn't allocate memory\n"));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    res = hCSP->GetKeyParam(hCSP, hProv, hKey, KP_IV, pbIV, &dwDataLen, 0);
    if (res) {
	KdPrint((MODNAME "! GetKeyParam failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    dwDataLen = sizeof(pbData);
    RtlCopyMemory(pbEncrypted, pbData, dwDataLen);
    res = hCSP->Encrypt(hCSP, hProv, hKey, 0, TRUE, 0, pbEncrypted, &dwDataLen,
	sizeof(pbEncrypted));
    if (res) {
	KdPrint((MODNAME "! Encrypt failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    res = hCSP->SetKeyParam(hCSP, hProv, hKey, KP_IV, pbIV, dwDataLen);
    if (res) {
	KdPrint((MODNAME "! SetKeyParam failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }
    ExFreePoolWithTag(pbIV, DRVSAMPLE_POOL);
    pbIV = NULL;

    res = hCSP->Decrypt(hCSP, hProv, hKey, 0, TRUE, 0, pbEncrypted,
	&dwDataLen);
    if (res) {
	KdPrint((MODNAME "! Decrypt failed with a return of 0x%.8X!\n", res));
	NtStatus = STATUS_UNSUCCESSFUL;
	goto exit;
    }

    if (dwDataLen != sizeof(pbData) || dwDataLen !=
	RtlCompareMemory(pbData, pbEncrypted, dwDataLen)) {
	    KdPrint((MODNAME "! Decrypt returned bad data!\n"));
	    NtStatus = STATUS_UNSUCCESSFUL;
    } else
	KdPrint((MODNAME ": Encrypt/Decrypt test completed successfully\n"));

exit:
    if (pbIV != NULL)
	ExFreePoolWithTag(pbIV, DRVSAMPLE_POOL);

    if (hCSP) {
	if (hKey) {
	    res = hCSP->DestroyKey(hCSP, hProv, hKey);
	    if (res) {
		KdPrint((MODNAME "! DestroyKey failed with a return of 0x%.8X!\n", res));
		NtStatus = STATUS_UNSUCCESSFUL;
	    }
	}
	if (hProv) {
	    res = hCSP->ReleaseContext(hCSP, hProv, 0);
	    if (res) {
		KdPrint((MODNAME "! ReleaseContext failed with a return of 0x%.8X!\n", res));
		NtStatus = STATUS_UNSUCCESSFUL;
	    }
	}
	res = hCSP->DestroyProvider(hCSP);
	if (res) {
	    KdPrint((MODNAME "! DestroyProvider failed with a return of 0x%.8X!\n", res));
	    NtStatus = STATUS_UNSUCCESSFUL;
	}
    }

    ExFreePoolWithTag(LfmmConfig.Buffer, DRVSAMPLE_POOL);

    return NtStatus;
}
