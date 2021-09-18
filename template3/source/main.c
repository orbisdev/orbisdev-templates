

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ps4sdk.h>
#include <orbislink.h>
#include <orbisGl2.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <signal.h>

#define MEM_SIZE (3UL * 1024 * 1024 * 1024) /* 2600 MiB */
#define MEM_ALIGN (16UL * 1024)

static OrbisMspace s_mspace = 0;
static void *address = 0;
static size_t s_mem_size = MEM_SIZE;

static int max_malloc(size_t initial_value, int increment, const char *desc)
{
    char *p_block;
    size_t chunk = initial_value;

    printf("Check maximum contigous block we can allocate (%s accurate)\n", desc);
    while ((p_block = sceLibcMspaceMalloc(s_mspace, ++chunk * increment)) != NULL) {
        sceLibcMspaceFree(s_mspace,p_block);
    }
    chunk--;
    printf("Maximum possible %s we can allocate is %i\n", desc, chunk);

    return chunk;
}




bool flag=true;

void finishApp()
{
	finishOrbisLinkApp();
	debugNetFinish();
}
void infoApp()
{
	int ret;
	SceKernelModuleInfo tmpInfo;
	SceKernelModule handles[256];
	size_t numModules;
	size_t i;
	debugNetPrintf(DEBUGNET_INFO,"[TEMPLATE3] VanillaGl initialization done!!!\n");
	memset(handles, 0, sizeof(handles));

	ret=sceKernelGetModuleList(handles,(sizeof(handles)/sizeof((handles)[0])),&numModules);
	if(ret)
	{
		debugNetPrintf(DEBUGNET_ERROR,"[TEMPLATE3][%s][%d] error 0x%08X\n",__FUNCTION__,__LINE__,ret);
		goto finish;
	}
	debugNetPrintf(DEBUGNET_INFO,"[TEMPLATE3][%s][%d] %d modules already loaded\n",__FUNCTION__,__LINE__,numModules);

	for(i=1;i<numModules;i++)
	{
		ret=sceKernelGetModuleInfo(handles[i],&tmpInfo);
		if(ret)
		{
			debugNetPrintf(DEBUGNET_ERROR,"[TEMPLATE3][%s][%d] error 0x%08X\n",__FUNCTION__,__LINE__,ret);
			goto finish;
		}
		debugNetPrintf(DEBUGNET_INFO,"[TEMPLATE3][%s][%d] Module %s is loaded\n",__FUNCTION__,__LINE__,tmpInfo.name);
		
	}
finish:
	return;
}
bool initApp()
{
	int ret=initOrbisLinkAppVanillaGl();
	if(ret==0)
	{
		debugNetInit(__DEBUGNET_PC_IP__,18194,3);
		infoApp();
		debugNetPrintf(DEBUGNET_INFO,"[TEMPLATE3] Ready to have a lot of fun\n");

		sceSystemServiceHideSplashScreen();
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{

	const int screenWidth = ATTR_ORBISGL_WIDTH;
    const int screenHeight = ATTR_ORBISGL_HEIGHT;

	flag=initApp();

	int res = sceKernelReserveVirtualRange(&address, MEM_SIZE, 0, MEM_ALIGN);
	printf("sceKernelReserveVirtualRange %x %x\n", res, address);
	res = sceKernelMapNamedSystemFlexibleMemory(&address, MEM_SIZE, 0x2, 0x0010, "TEST");
	printf("sceKernelMapNamedSystemFlexibleMemory %x %x\n", res, address);
	s_mspace = sceLibcMspaceCreate("User Mspace", address, MEM_SIZE, 0);
	printf("sceLibcMspaceCreate %p \n", s_mspace);
  
	printf("Total  %d %s\n", max_malloc(0, 1024 * 1024, "MB"), "MB");
	kill(getpid(), SIGTERM);
	//exit(1);
	return 0;
	
}
