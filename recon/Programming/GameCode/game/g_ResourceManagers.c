void G_InitMsgMngr();
void G_ReleaseMsgMngr();
#if	G_NEW_SYSTEM
void InitBoundingFormMngr();
void ReleaseBoundingFormMngr();
#endif

void G_InitResourceManagers()
{
	G_InitMsgMngr();
#if	G_NEW_SYSTEM
	InitBoundingFormMngr();
#endif
}

void G_ReleaseResourceManagers()
{
	G_ReleaseMsgMngr();
#if	G_NEW_SYSTEM
	ReleaseBoundingFormMngr();
#endif
}