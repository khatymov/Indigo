#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#define DLOPEN(a) LoadLibrary(a)
#define DLSYM(a, b) GetProcAddress(a, b)
#define DLERROR ""
#define DLCLOSE(a) FreeLibrary(a)
#define HANDLE HMODULE
#else
#include <dlfcn.h>
#define DLOPEN(a) dlopen(a, RTLD_GLOBAL | RTLD_NOW)
#define DLSYM(a, b) dlsym(a, b)
#define DLERROR dlerror()
#define DLCLOSE(a) dlclose(a)
#define HANDLE void*
#endif

typedef int (*INT_RET_STR)(const char*);
typedef int (*INT_RET)();
typedef const char* (*STR_RET_INT)(int);
typedef const char* (*STR_RET_VOID)(void);
typedef int (*INT_RET_INT_INT)(int, int);
typedef int (*INT_RET_STR_STR)(const char*, const char*);
typedef int (*INT_RET_STR_STR_STR)(const char*, const char*, const char*);
typedef int (*INT_RET_INT)(int);
typedef unsigned long long (*QWORD_RET_VOID)(void);
typedef void (*VOID_RET_QWORD)(unsigned long long);
typedef int (*INT_RET_VOID)(void);

/* Try to dynamically load library and check load status. */
HANDLE dlOpenWithCheck(const char* libraryPath)
{
    HANDLE handle = DLOPEN(libraryPath);
#ifdef _WIN32
    /* On Windows error code is returned by LoadLibrary() and can be between 0 and 31 */
    if (handle < (HANDLE)32)
    {
        printf("Error in LoadLibrary. Error code: %p\n", handle);
        return NULL;
    }
#else
    if (!handle)
    {
        printf("Error in dlopen: %s\n", dlerror());
        return NULL;
    }
#endif
    return handle;
}

int main(int argc, char** argv)
{
    HANDLE indigoHandle;
    HANDLE indigoInChIHandle;
    HANDLE indigoRendererHandle;
    HANDLE bingoHandle;

    STR_RET_VOID indigoVersion;
    QWORD_RET_VOID indigoAllocSessionId;
    VOID_RET_QWORD indigoSetSessionId;
    VOID_RET_QWORD indigoReleaseSessionId;
    INT_RET_STR indigoLoadReactionFromString;
    INT_RET_STR indigoLoadMoleculeFromString;
    STR_RET_VOID indigoGetLastError;
    INT_RET_STR_STR indigoSetOption;
    INT_RET indigoWriteBuffer;
    INT_RET_INT_INT indigoRender;
    STR_RET_INT indigoInchiGetInchi;
    INT_RET_STR_STR_STR bingoCreateDatabaseFile;
    INT_RET_INT bingoCloseDatabase;
    STR_RET_VOID bingoVersion;
    INT_RET_INT indigoInchiInit;
    INT_RET_INT indigoInchiDispose;
    INT_RET_INT indigoRendererInit;
    INT_RET_INT indigoRendererDispose;

    int indigoTest = 0;
    int indigoInChITest = 0;
    int indigoRendererTest = 0;
    int bingoTest = 0;

    const char* indigoLibraryPath;
    const char* indigoInChILibraryPath;
    const char* indigoRendererLibraryPath;
    const char* bingoLibraryPath;
    unsigned long long session;

    int i = 0;

    /* Parse arguments and set variables*/
    for (i = 0; i < argc; i++)
    {
        if (strstr(argv[i], "indigo."))
        {
            indigoTest = 1;
            indigoLibraryPath = argv[i];
        }
        if (strstr(argv[i], "indigo-inchi"))
        {
            indigoInChITest = 1;
            indigoInChILibraryPath = argv[i];
        }
        if (strstr(argv[i], "indigo-renderer"))
        {
            indigoRendererTest = 1;
            indigoRendererLibraryPath = argv[i];
        }
        if (strstr(argv[i], "bingo"))
        {
            bingoTest = 1;
            bingoLibraryPath = argv[i];
        }
    }
    /* Tests */
    if (indigoTest)
    {
        printf("*** Testing libindigo ***\n");
        int r;
        /* Load Indigo */
        indigoHandle = dlOpenWithCheck(indigoLibraryPath);
        if (!indigoHandle)
        {
            printf("Cannot load %s\n", indigoLibraryPath);
            return 1;
        }
        printf("Indigo address: %p\n", indigoHandle);
        /* Execute Indigo function */
        indigoVersion = (STR_RET_VOID)DLSYM(indigoHandle, "indigoVersion");
        indigoAllocSessionId = (QWORD_RET_VOID)DLSYM(indigoHandle, "indigoAllocSessionId");
        indigoSetSessionId = (VOID_RET_QWORD)DLSYM(indigoHandle, "indigoSetSessionId");
        indigoLoadReactionFromString = (INT_RET_STR)DLSYM(indigoHandle, "indigoLoadReactionFromString");
        indigoGetLastError = (STR_RET_VOID)DLSYM(indigoHandle, "indigoGetLastError");
        indigoReleaseSessionId = (VOID_RET_QWORD)DLSYM(indigoHandle, "indigoReleaseSessionId");

        printf("Indigo version: %s\n", indigoVersion());
        session = indigoAllocSessionId();
        indigoSetSessionId(session);
        printf("Indigo session: %llu\n", session);
        r = indigoLoadReactionFromString("C");
        if (r < 0)
        {
            printf("Error handled: %s\n", indigoGetLastError());
        }
        indigoReleaseSessionId(session);
    }
    if (indigoInChITest)
    {
        printf("*** Testing libindigo-inchi ***\n");
        int m;
        /* Load IndigoInChI */
        indigoInChIHandle = dlOpenWithCheck(indigoInChILibraryPath);
        if (!indigoInChIHandle)
        {
            printf("Cannot load %s\n", indigoInChILibraryPath);
            return 1;
        }
        printf("IndigoInChI address: %p\n", indigoInChIHandle);
        indigoLoadMoleculeFromString = (INT_RET_STR)DLSYM(indigoHandle, "indigoLoadMoleculeFromString");
        indigoInchiGetInchi = (STR_RET_INT)DLSYM(indigoInChIHandle, "indigoInchiGetInchi");
        indigoInchiInit = (INT_RET_INT)DLSYM(indigoInChIHandle, "indigoInchiInit");
        indigoInchiDispose = (INT_RET_INT)DLSYM(indigoInChIHandle, "indigoInchiDispose");

        session = indigoAllocSessionId();
        indigoSetSessionId(session);
        indigoInchiInit(session);
        printf("Indigo session: %llu\n", session);
        m = indigoLoadMoleculeFromString("C");
        printf("indigoInChI InChI: %s\n", indigoInchiGetInchi(m));
        indigoInchiDispose(session);
        indigoReleaseSessionId(session);
    }
    if (indigoRendererTest)
    {
        printf("*** Testing libindigo-renderer ***\n");
        int m, buf, res;
        /* Load IndigoRenderer */
        indigoRendererHandle = dlOpenWithCheck(indigoRendererLibraryPath);
        if (!indigoRendererHandle)
        {
            printf("Cannot load %s\n", indigoRendererLibraryPath);
            return 1;
        }
        printf("IndigoRenderer address: %p\n", indigoRendererHandle);
        indigoLoadMoleculeFromString = (INT_RET_STR)DLSYM(indigoHandle, "indigoLoadMoleculeFromString");
        indigoWriteBuffer = (INT_RET)DLSYM(indigoHandle, "indigoWriteBuffer");
        indigoSetOption = (INT_RET_STR_STR)DLSYM(indigoHandle, "indigoSetOption");
        indigoRender = (INT_RET_INT_INT)DLSYM(indigoRendererHandle, "indigoRender");
        indigoRendererInit = (INT_RET_INT)DLSYM(indigoRendererHandle, "indigoRendererInit");
        indigoRendererDispose = (INT_RET_INT)DLSYM(indigoRendererHandle, "indigoRendererDispose");

        session = indigoAllocSessionId();
        indigoSetSessionId(session);
        indigoRendererInit(session);
        printf("Indigo session: %llu\n", session);
        indigoSetOption("render-output-format", "png");
        m = indigoLoadMoleculeFromString("C");
        buf = indigoWriteBuffer();
        res = indigoRender(m, buf);
        printf("indigoRender result: %d\n", res);
        indigoRendererDispose(session);
        indigoReleaseSessionId(session);
    }
    if (bingoTest)
    {
        printf("*** Testing libbingo-nosql ***\n");
        int db;
        /* Load Bingo */
        bingoHandle = dlOpenWithCheck(bingoLibraryPath);
        if (!bingoHandle)
        {
            printf("Cannot load %s\n", bingoLibraryPath);
            return 1;
        }
        printf("Bingo address: %p\n", bingoHandle);
        bingoVersion = (STR_RET_VOID)DLSYM(bingoHandle, "bingoVersion");
        printf("Bingo version: %s\n", bingoVersion());
        bingoCreateDatabaseFile = (INT_RET_STR_STR_STR)DLSYM(bingoHandle, "bingoCreateDatabaseFile");
        bingoCloseDatabase = (INT_RET_INT)DLSYM(bingoHandle, "bingoCloseDatabase");

        session = indigoAllocSessionId();
        indigoSetSessionId(session);
        printf("Indigo session: %llu\n", session);
        db = bingoCreateDatabaseFile("test.db", "molecule", "");
        printf("Bingo database ID: %d\n", db);
        printf("Bingo close database status: %d\n", bingoCloseDatabase(db));
        indigoReleaseSessionId(session);
    }
    /* Close libraries */
    if (bingoTest)
    {
        DLCLOSE(bingoHandle);
    }
    if (indigoRendererTest)
    {
        DLCLOSE(indigoRendererHandle);
    }
    if (indigoInChITest)
    {
        DLCLOSE(indigoInChIHandle);
    }
    if (indigoTest)
    {
        DLCLOSE(indigoHandle);
    }
    return 0;
}
