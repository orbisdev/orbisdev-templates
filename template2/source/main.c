

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ps4sdk.h>
#include <orbislink.h>
#include <orbisGl2.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/stat.h>


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
	debugNetPrintf(DEBUGNET_INFO,"[TEMPLATE1] VanillaGl initialization done!!!\n");
	memset(handles, 0, sizeof(handles));

	ret=sceKernelGetModuleList(handles,(sizeof(handles)/sizeof((handles)[0])),&numModules);
	if(ret)
	{
		debugNetPrintf(DEBUGNET_ERROR,"[TEMPLATE1][%s][%d] error 0x%08X\n",__FUNCTION__,__LINE__,ret);
		goto finish;
	}
	debugNetPrintf(DEBUGNET_INFO,"[TEMPLATE1][%s][%d] %d modules already loaded\n",__FUNCTION__,__LINE__,numModules);

	for(i=1;i<numModules;i++)
	{
		ret=sceKernelGetModuleInfo(handles[i],&tmpInfo);
		if(ret)
		{
			debugNetPrintf(DEBUGNET_ERROR,"[TEMPLATE1][%s][%d] error 0x%08X\n",__FUNCTION__,__LINE__,ret);
			goto finish;
		}
		debugNetPrintf(DEBUGNET_INFO,"[TEMPLATE1][%s][%d] Module %s is loaded\n",__FUNCTION__,__LINE__,tmpInfo.name);
		
	}
finish:
	return;
}
bool initApp()
{
	int ret=initOrbisLinkAppVanillaGl();
	if(ret==0)
	{
		debugNetInit("192.168.1.12",18194,3);
		infoApp();
		debugNetPrintf(DEBUGNET_INFO,"[TEMPLATE1] Ready to have a lot of fun\n");

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
	while(flag)
	{

	}
	finishApp();
	sleep(3);
	exit(EXIT_SUCCESS);
}
